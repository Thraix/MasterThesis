#include "Texture2D.h"

#include <internal/GreetGL.h>

namespace Greet {

  Texture2D::Texture2D(uint width, uint height, TextureParams params)
    : Texture(GL_TEXTURE_2D,true), width{width}, height{height}, params(params)
    {
      GenTexture({});
    }

  Texture2D::Texture2D(const std::string& filename, TextureParams params)
    : Resource(filename), Texture(GL_TEXTURE_2D,true), params(params)
  {
    LoadTexture(filename);
  }

  Texture2D::Texture2D(const std::vector<byte>& bits, uint width, uint height, TextureParams params)
    : Texture(GL_TEXTURE_2D,true), width(width), height(height), params(params)
  {
    GenTexture(bits);
  }

  Texture2D::Texture2D(TextureParams params)
    : Texture(GL_TEXTURE_2D,false), params(params)
  {
  }

  Texture2D::Texture2D(uint texId, uint width, uint height, TextureParams params)
    : Texture(texId, (uint)GL_TEXTURE_2D), width(width), height(height), params(params)
  {
  }

  Texture2D::~Texture2D()
  {
  }

  void Texture2D::LoadTexture(const std::string& filename)
  {
    auto res = ImageUtils::LoadImage(filename.c_str(), &width,&height);
    GenTexture(res.second);
  }

  void Texture2D::GenTexture(const std::vector<byte>& bits)
  {
    Enable();
    if(params.filter != TextureFilter::NONE)
    {
      GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  (uint)params.filter));
      GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (uint)params.filter));
    }
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, (uint)params.internalFormat, width, height, 0, (uint)params.internalFormat, GL_UNSIGNED_BYTE, bits.data()));
    Disable();
  }

  void Texture2D::SetPixels(const std::vector<byte>& pixels) const
  {
    Enable();
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, (uint)params.internalFormat, width, height, 0, (uint)params.internalFormat, GL_UNSIGNED_BYTE, pixels.data()));
  }

  void Texture2D::ReloadResource()
  {
    if(texId)
      LoadTexture(filePath);
  }

  Ref<Texture2D> Texture2D::Create(uint width, uint height, TextureParams params)
  {
    return Ref<Texture2D>(new Texture2D(width, height, params));
  }

  Ref<Texture2D> Texture2D::Create(const std::string& filename, TextureParams params)
  {
    return Ref<Texture2D>(new Texture2D(filename, params));
  }

  Ref<Texture2D> Texture2D::Create(const std::vector<byte>& bits, uint width, uint height, TextureParams params)
  {
    return Ref<Texture2D>(new Texture2D(bits, width, height, params));
  }

  Ref<Texture2D> Texture2D::Create(uint texID, uint width, uint height, TextureParams params)
  {
    return Ref<Texture2D>(new Texture2D(texID, width, height, params));
  }

  Ref<Texture2D> Texture2D::Create(TextureParams params)
  {
    return Ref<Texture2D>(new Texture2D(params));
  }
}
