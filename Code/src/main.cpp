#include <Greet.h>

#include "CommandLine.h"
#include "Voxelizer.h"
#include "Cam.h"
#include "Axes.h"
#include <thread>
#include <chrono>
#include <algorithm>

using namespace Greet;

class Application : public App
{
  bool viewMesh = false;
  bool viewRaycast = true;
  Mat4 modelMatrix;
  Ref<GLuint> voxelGridTex;

  // Rendering the model
  Ref<Shader> meshShader;
  Ref<Mesh> mesh;

  // Rendering the raycasting
  Ref<Shader> raycastShader;
  Ref<Mesh> raycastMesh;

  Cam camera;
  CamController camController;
  Ref<Axes> cursor;
  float time = 0;
  CommandLineData commandLine;

  public:
    Application(const CommandLineData& data)
      : App("Voxelizer", 720, 720),
      camera{Mat4::ProjectionMatrix(Window::GetAspect(), 90, 0.01, 4000.0f)},
      camController{camera}, commandLine{data}
    {
      SetFrameCap(30);
    }

    void Init() override
    {
      cursor.reset(new Axes(camera));
      RenderCommand::SetClearColor({0.4f, 0.4f, 0.4f, 1.0f});
      camController.SetMoveSpeed(commandLine.voxelGridSize / 2);

      camera.SetPosition(Vec3<float>{0.654972, 0.951773, 1.21714} * commandLine.voxelGridSize);
      camera.SetRotation({-36.1529, 18.066, 0});
      /* camera.SetPosition(Vec3<float>{0.908684, 0.38362, 0.639754} * voxelGridSize); */
      /* camera.SetRotation({-3.5053, -270.041, 0}); */

      // ---- Setup raycasting
      raycastShader = Shader::FromFile("res/shaders/raycast.glsl");

      Pointer<Vec3<float>> vertices{
        { 1.0,  1.0, 0.0},
        {-1.0,  1.0, 0.0},
        {-1.0, -1.0, 0.0},
        { 1.0, -1.0, 0.0}};

      Pointer<uint> indices{0, 1, 2, 0, 2, 3};
      MeshData raycastMeshData{vertices, indices};
      raycastMesh.reset(new Mesh(raycastMeshData));

      // ---- Setup OpenGL 3D texture
      GLuint tex;
      glGenTextures(1, &tex);
      voxelGridTex.reset(new GLuint(tex));
      glBindTexture(GL_TEXTURE_3D, *voxelGridTex);
      GLCall(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
      GLCall(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
      GLCall(glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, commandLine.voxelGridSize, commandLine.voxelGridSize, commandLine.voxelGridSize, 0, GL_RED, GL_FLOAT, nullptr));

      // ---- Setup CUDA
      Voxelizer::InitCuda();

      // ---- Setup Voxelization kernel
      Voxelizer::InitKernel(commandLine.voxelGridSize, commandLine.primaryAlg, commandLine.secondaryAlg, commandLine.runCompare);
      Voxelizer::BindTexture(*voxelGridTex);

      // ---- Create mesh to voxelize
      float p = commandLine.voxelGridSize / 2;
      MeshData meshData({},{});
      if(commandLine.model == "sphere")
        meshData = MeshFactory::Sphere({0,0,0},p, 100, 100);
      else if(commandLine.model == "tetra")
        meshData = MeshFactory::Tetrahedron(p,p,p,p);
      else if(commandLine.model == "cube")
        meshData = MeshFactory::Cube(p,p,p,p,p,p);
      else if(commandLine.model == "quad")
        meshData = MeshFactory::Quad(p,p,p,p,p);
      else if(commandLine.model == "dragon")
        meshData = OBJUtils::LoadObj("res/objs/dragon.obj");
      else if(commandLine.model == "bunny")
        meshData = OBJUtils::LoadObj("res/objs/bunny.obj");
      else if(commandLine.model == "monkey")
        meshData = OBJUtils::LoadObj("res/objs/monkey.obj");
      else
        meshData = OBJUtils::LoadObj(commandLine.model);
      if(meshData.GetIndexCount() == 0)
      {
        Log::Info("Failed to load mesh");
        Window::Close();
        return;
      }

      Vec3<float> min = meshData.GetVertices()[0];
      Vec3<float> max = meshData.GetVertices()[0];
      for(auto&& vec : meshData.GetVertices())
      {
        min = Vec::Min(vec, min);
        max = Vec::Max(vec, max);
      }
      Vec3<float> diff = max - min;
      float maxDiff = Math::Max(diff.x, Math::Max(diff.y, diff.z));
      for(auto&& vec : meshData.GetVertices())
      {
        vec = Vec3<float>(vec - min + (maxDiff - diff) / 2) / maxDiff * (commandLine.voxelGridSize - 1) + 0.5;
      }
      meshData.GenerateNormals();
      mesh.reset(new Mesh(meshData));
      meshShader = Shader::FromFile("res/shaders/3dshader.glsl");
      modelMatrix = Mat4(1.0f);
      /* modelMatrix = Mat4::Translate(voxelGridSize/2) * Mat4::RotateY(90) * Mat4::Translate(-voxelGridSize/2); */
      /* modelMatrix = Mat4::Translate(voxelGridSize/2) * Mat4::Scale(0.5) * Mat4::RotateY(114) * Mat4::RotateX(1) * Mat4::Translate(-voxelGridSize/2); */
      /* modelMatrix = */
      /*   Mat4::Translate(voxelGridSize/2) * */
      /*   Mat4::RotateX(45) * */
      /*   Mat4::Translate(-voxelGridSize/2); */

      Log::Info("Triangle Count: ", meshData.GetIndexCount() / 3);
      Voxelizer::UploadMat4(modelMatrix, "modelMatrix");
      Voxelizer::UploadMesh(meshData);
      float time = Voxelizer::RunVoxelize(commandLine.runCompare);
      if(commandLine.runPerformance)
        RunPerformance();
      else
        Log::Info("Voxelization (",commandLine.voxelGridSize,") took ", time, " ms");
      if(commandLine.runCompare)
        PrintError();
    }

    void PrintError()
    {
      std::ofstream file(commandLine.logFile);
      if(commandLine.logFile != "")
        Log::AddLogStream(new LogStream(file, Greet::LogLevel::Information(), "performance"));

      Log::Info("Jaccard:  ", round(Voxelizer::GetJaccard()*10000) / 100, "%");
      Log::Info("Relative: ", round(Voxelizer::GetRelative()*10000) / 100, "%");

      if(commandLine.logFile != "")
        Log::RemoveLogStream("performance");
      /* Window::Close(); */
    }

    void RunPerformance()
    {
      float timeElapsed = 0.0f;
      int runs = commandLine.runs;
      std::stringstream ss;
      for(int i = 0;i<runs+900;i++)
      {
        float time = Voxelizer::RunVoxelize(false);
        // For some reason the first like 900 runs
        // might have vastly different performance
        // So these are ignored
        if(i > 900)
        {
          timeElapsed += time;
          ss << "(" << i << "," << time << ")";
        }
      }
      std::ofstream file(commandLine.logFile);
      if(commandLine.logFile != "")
        Log::AddLogStream(new LogStream(file, Greet::LogLevel::Information(), "performance"));

      Log::Info(ss.str());
      Log::Info("Voxelization (",commandLine.voxelGridSize,") took an average of ", timeElapsed/runs, " ms");
      if(commandLine.logFile != "")
      {
        Log::RemoveLogStream("performance");
      }
      Window::Close();
    }

    void Tick() override
    {
    }

    void Render() override
    {

      // Force position for screenshots
      if(commandLine.screenshot)
      {
        if(commandLine.model == "dragon")
        {
          camera.SetPosition(Vec3<float>{0.149507, 0.785084, 1.00146} * commandLine.voxelGridSize);
          camera.SetRotation({-24.1514, -23.9309, 0});
        }
        else if(commandLine.model == "monkey")
        {
          camera.SetPosition(Vec3<float>{0.904071, 0.935106, 0.976017} * commandLine.voxelGridSize);
          camera.SetRotation({-36.1529, 36.0677, 0});
        }
        else
        {
          camera.SetPosition(Vec3<float>{0.654972, 0.951773, 1.21714} * commandLine.voxelGridSize);
          camera.SetRotation({-36.1529, 18.066, 0});
        }
      }
      /* modelMatrix = Mat4::Translate(voxelGridSize/2) * Mat4::Scale(0.5) * Mat4::RotateY(time*30) * Mat4::RotateX(5) * Mat4::Translate(-voxelGridSize/2); */
      /* GreetCuda::UploadMat4(modelMatrix, "modelMatrix"); */
      /* GreetCuda::RunVoxelize(); */
      Vec3<float> sunDir = Vec3<float>(0.8, 0.7, 0.2);
      if(viewRaycast)
      {
        RenderCommand::EnableDepthTest(false);
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_3D, *voxelGridTex));
        raycastShader->Enable();
        raycastShader->SetUniformMat4("u_PVInvMatrix", camera.GetInvPVMatrix());
        raycastShader->SetUniformMat4("u_ViewMatrix", camera.GetViewMatrix());
        raycastShader->SetUniform1i("u_Size", commandLine.voxelGridSize);
        raycastShader->SetUniform1i("u_ChunkTexUnit", 0);
        raycastShader->SetUniform3f("u_SunDir", sunDir);
        raycastMesh->Bind();
        raycastMesh->Render();
        raycastMesh->Unbind();
        RenderCommand::EnableDepthTest(true);
      }
      /* GLCall(glClearTexImage(*voxelGridTex, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL)); */
      /* GreetCuda::BindTexture(*voxelGridTex); */
      if(viewMesh)
      {
        meshShader->Enable();
        meshShader->SetUniformMat4("u_ModelMatrix", modelMatrix);
        meshShader->SetUniformMat4("u_ViewMatrix", camera.GetViewMatrix());
        meshShader->SetUniformMat4("u_ProjectionMatrix", camera.GetProjectionMatrix());
        meshShader->SetUniform1f("u_Size", commandLine.voxelGridSize);
        meshShader->SetUniform3f("u_SunDir", sunDir);
        meshShader->SetUniform3f("u_Color", {0.8, 0.2, 0.2});
        mesh->Bind();
        mesh->Render();
        meshShader->SetUniform3f("u_Color", {0.2, 0.8, 0.8});
        RenderCommand::SetCullFace(CullFaceDirection::CW);
        mesh->Render();
        mesh->Unbind();
        meshShader->Disable();
        RenderCommand::ResetCulling();
      }
      if(commandLine.screenshot)
      {
        Utils::Screenshot(Window::GetWidth(), Window::GetHeight(), commandLine.screenshotName);
        Window::Close();
      }
      cursor->Render(camera.GetViewMatrix(), camera.GetProjectionMatrix());
    }

    void Update(float timeElapsed) override
    {
      time += timeElapsed;
      camController.Update(timeElapsed);
    }

    void OnEvent(Event& event) override
    {
      if(EVENT_IS_TYPE(event, EventType::KEY_PRESS))
      {
        KeyPressEvent& e = static_cast<KeyPressEvent&>(event);
        if(e.GetButton() == GREET_KEY_KP_1)
        {
          camera.SetPosition({0, (float)commandLine.voxelGridSize, 0});
          camera.SetRotation({-45, -135, 0});
        }
        else if(e.GetButton() == GREET_KEY_KP_2)
        {
          camera.SetPosition({0, (float)commandLine.voxelGridSize, (float)commandLine.voxelGridSize/2});
          camera.SetRotation({-45, -90, 0});
        }
        else if(e.GetButton() == GREET_KEY_KP_3)
        {
          camera.SetPosition({0, (float)commandLine.voxelGridSize, (float)commandLine.voxelGridSize});
          camera.SetRotation({-45, -45, 0});
        }
        else if(e.GetButton() == GREET_KEY_KP_4)
        {
          camera.SetPosition({(float)commandLine.voxelGridSize/2, (float)commandLine.voxelGridSize, 0});
          camera.SetRotation({-45, 180, 0});
        }
        else if(e.GetButton() == GREET_KEY_KP_6)
        {
          camera.SetPosition({(float)commandLine.voxelGridSize/2, (float)commandLine.voxelGridSize, (float)commandLine.voxelGridSize});
          camera.SetRotation({-45, 0, 0});
        }
        else if(e.GetButton() == GREET_KEY_KP_7)
        {
          camera.SetPosition({(float)commandLine.voxelGridSize, (float)commandLine.voxelGridSize, 0});
          camera.SetRotation({-45, 135, 0});
        }
        else if(e.GetButton() == GREET_KEY_KP_8)
        {
          camera.SetPosition({(float)commandLine.voxelGridSize, (float)commandLine.voxelGridSize, (float)commandLine.voxelGridSize/2});
          camera.SetRotation({-45, 90, 0});
        }
        else if(e.GetButton() == GREET_KEY_KP_9)
        {
          camera.SetPosition({(float)commandLine.voxelGridSize, (float)commandLine.voxelGridSize, (float)commandLine.voxelGridSize});
          camera.SetRotation({-45, 45, 0});
        }
        else if(e.GetButton() == GREET_KEY_1)
        {
          viewRaycast = !viewRaycast;
        }
        else if(e.GetButton() == GREET_KEY_2)
        {
          viewMesh = !viewMesh;
        }
        else if(e.GetButton() == GREET_KEY_F1)
        {
          Utils::Screenshot(Window::GetWidth(), Window::GetHeight());
        }
        else if(e.GetButton() == GREET_KEY_C)
        {
          Log::Info(camera.GetPosition()/commandLine.voxelGridSize, " ", camera.GetRotation());
        }
        else if(e.GetButton() == GREET_KEY_Q)
        {
          Window::Close();
        }
      }
      else if(EVENT_IS_TYPE(event, EventType::VIEWPORT_RESIZE))
      {
        ViewportResizeEvent& e = static_cast<ViewportResizeEvent&>(event);
        camera.SetProjectionMatrix(Mat4::ProjectionMatrix(e.GetWidth() / e.GetHeight(), 90, 0.01f, 4000.0f));
      }
    }
};

int main(int argc, char **argv)
{
  CommandLineData data{argc, argv};
  if(!data.Valid())
    return 1;
  Application app{data};
  app.Start();
}
