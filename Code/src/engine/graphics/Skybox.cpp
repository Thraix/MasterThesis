#include "Skybox.h"

#include <graphics/models/MeshFactory.h>
#include <graphics/shaders/ShaderFactory.h>


namespace Greet {

  Skybox::Skybox(const Ref<CubeMap>& cubemap)
    : Skybox{cubemap, ShaderFactory::ShaderSkybox()}
  {
  }

  Skybox::Skybox(const Ref<CubeMap>& map, const Ref<Shader>& shader)
    : m_map(map), m_shader(shader)
  {
    MeshData data{MeshFactory::Cube(0,0,0, 1,1,1)};
    m_mesh = new Mesh(data);
    m_mesh->SetClockwiseRender(true);
  }

  Skybox::~Skybox()
  {
    delete m_mesh;
  }

  void Skybox::Render(const Mat4& projectionMatrix, const Mat4& viewMatrix) const
  {
    RenderCommand::EnableDepthTest(false);
    m_shader->Enable();
    m_shader->SetUniformMat4("projectionMatrix", projectionMatrix);
    m_shader->SetUniformMat4("viewMatrix", viewMatrix);
    m_map->Enable(0);
    m_mesh->Bind();
    m_mesh->Render();
    m_mesh->Unbind();
    m_map->Disable();
    m_shader->Disable();
    RenderCommand::ResetDepthTest();
  }

  void Skybox::Render(const Camera& camera) const
  {
    Render(camera.GetProjectionMatrix(), camera.GetViewMatrix());
  }
}
