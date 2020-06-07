#include "Texture.h"

#include <internal/GreetGL.h>

namespace Greet {

  Texture::Texture(uint textureType, bool generateTexture)
    : texId{0}, m_textureType{textureType}
  {
    ASSERT(m_textureType == GL_TEXTURE_2D || m_textureType == GL_TEXTURE_CUBE_MAP, "Invalid texture enum");
    if(generateTexture)
    {
      uint id;
      glGenTextures(1, &id);
      texId = id;
      Enable();
    }
  }

  Texture::Texture(uint texId, uint textureType)
    : texId{texId}, m_textureType{textureType}
  {
    ASSERT(m_textureType == GL_TEXTURE_2D || m_textureType == GL_TEXTURE_CUBE_MAP, "Invalid texture enum");
    Enable();
  }

  Texture::Texture()
    : texId{0}, m_textureType{0}
  {
  }

  Texture::~Texture()
  {
    glDeleteTextures(1, &texId);
  }

  void Texture::Enable() const
  {
    GLCall(glBindTexture(m_textureType, texId));
  }

  void Texture::Enable(uint index) const
  {
    GLCall(glActiveTexture(GL_TEXTURE0 + index));
    GLCall(glBindTexture(m_textureType, texId));
  }

  void Texture::Disable() const
  {
    GLCall(glBindTexture(m_textureType, 0));
  }

  uint Texture::GetTexId() const
  {
    return texId;
  }
}
