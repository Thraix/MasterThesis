#pragma once

#include <Greet.h>

#include "Cam.h"

class Axes
{
  private:
    Greet::Ref<Greet::Shader> shader;
    Cam& cam;
    Greet::Ref<Greet::Mesh> mesh;
  public:
    Axes(Cam& cam)
      : cam{cam}
    {
      shader = Greet::Shader::FromFile("res/shaders/cursor.glsl");
      Greet::MeshData data = Greet::OBJUtils::LoadObj("res/objs/arrow.obj");
      Greet::Vec3<float> min = data.GetVertices()[0];
      Greet::Vec3<float> max = data.GetVertices()[0];
      for(auto&& vec : data.GetVertices())
      {
        min = Greet::Vec::Min(vec, min);
        max = Greet::Vec::Max(vec, max);
      }
      Greet::Vec3<float> diff = max - min;
      float maxDiff = Greet::Math::Max(diff.x, Greet::Math::Max(diff.y, diff.z));
      for(auto&& vec : data.GetVertices())
      {
        vec = vec / maxDiff;
      }
      data.GenerateNormals();
      mesh.reset(new Greet::Mesh(data));
    }

    void Render(const Greet::Mat4& view, const Greet::Mat4& proj)
    {
      Greet::Vec3<float> sunDir = Greet::Vec3<float>(0.8, 0.7, 0.2);
      Greet::Mat4 rotation = Greet::Mat4::Translate({-8 * Greet::Window::GetAspect(),-8,-10}) * Greet::Mat4::RotateX(-cam.GetRotation().x) * Greet::Mat4::RotateY(-cam.GetRotation().y);

      shader->Enable();
      shader->SetUniformMat4("u_ViewMatrix", rotation);
      shader->SetUniformMat4("u_ProjectionMatrix", proj);
      shader->SetUniform3f("u_SunDir", sunDir);

      Greet::Vec3<float> size = {1.0};
      Greet::Vec3<float> translation = {-0.5};
      mesh->Bind();
      shader->SetUniformMat4("u_ModelMatrix", Greet::Mat4::Translate(translation) * Greet::Mat4::Scale(size) * Greet::Mat4::RotateZ(-90));
      shader->SetUniform3f("u_Color", {1,0,0});
      mesh->Render();
      shader->SetUniformMat4("u_ModelMatrix", Greet::Mat4::Translate(translation) * Greet::Mat4::Scale(size) * Greet::Mat4(1.0f));
      shader->SetUniform3f("u_Color", {0,1,0});
      mesh->Render();
      shader->SetUniformMat4("u_ModelMatrix", Greet::Mat4::Translate(translation) * Greet::Mat4::Scale(size) * Greet::Mat4::RotateX(90));
      shader->SetUniform3f("u_Color", {0,0,1});
      mesh->Render();
      mesh->Unbind();
      shader->Disable();
    }
};
