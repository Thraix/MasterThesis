#include "Material.h"

#include <graphics/textures/TextureManager.h>
#include <utils/UUID.h>

namespace Greet {

  Material::Material(const Ref<Shader>& shader, const Ref<Texture2D>& texture)
    : m_shader{shader}, m_texture(texture), color(Vec4(1,1,1,1)), uuid{UUID::GetInstance().GetUUID()}
  {
    UpdateTexture();
  }

  Material::Material(const Ref<Shader>& shader)
    : m_shader(shader), m_texture(nullptr), color(Vec4(1,1,1,1)), uuid{UUID::GetInstance().GetUUID()}
  {
    UpdateTexture();
  }

  Material::~Material()
  {
  }

  void Material::Bind(const Camera* camera) const
  {
    m_shader->Enable();
    m_shader->SetUniform1f("specularStrength", specularStrength);
    m_shader->SetUniform1f("specularExponent", specularExponent);
    m_shader->SetUniform1f("diffuseStrength", diffuseStrength);
    m_shader->SetUniform4f("mat_color", color);
    m_shader->SetUniformMat4("projectionMatrix", camera->GetProjectionMatrix());
    m_shader->SetUniformMat4("viewMatrix", camera->GetViewMatrix());
    if(m_texture)
      m_texture->Enable();
  }

  void Material::Unbind() const
  {
    if(m_texture)
      m_texture->Disable();
    m_shader->Disable();
  }

  void Material::SetShader(const Ref<Shader>& shader)
  {
    m_shader = shader;
    UpdateTexture();
  }

  void Material::UpdateTexture()
  {
    m_shader->Enable();
    m_shader->SetUniformBoolean("hasTexture", m_texture.get() != nullptr);
    m_shader->Disable();
  }
}
