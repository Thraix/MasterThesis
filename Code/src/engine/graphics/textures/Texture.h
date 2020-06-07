#pragma once

#include <string>
#include <memory>
#include <common/Types.h>

namespace Greet {

  enum class TextureFilter
  {
    NONE,
    LINEAR = 0x2601,  //GL_LINEAR
    NEAREST = 0x2600, //GL_NEAREST
  };

  enum class TextureWrap
  {
    NONE,
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER,
    MIRRORED_REPEAT,
    REPEAT,
    MIRROR_CLAMP_TO_EDGE
  };

  enum class TextureInternalFormat
  {
    DEPTH_COMPONENT = 0x1902, // GL_DEPTH_COMPONENT
    DEPTH_STENCIL = 0x84F9,   // GL_DEPTH_STENCIL
    RED = 0x1903,             // GL_RED
    RGB = 0x1907,             // GL_RGB
    RGBA = 0x1908             // GL_RGBA
  };

  struct TextureParams
  {
    TextureParams()
      : filter(TextureFilter::LINEAR), wrap(TextureWrap::CLAMP_TO_EDGE), internalFormat(TextureInternalFormat::RGBA)
    {}

    TextureParams(TextureFilter filter, TextureWrap wrap, TextureInternalFormat internalFormat)
      : filter(filter), wrap(wrap), internalFormat(internalFormat)
    {}

    TextureFilter filter;
    TextureWrap wrap;
    TextureInternalFormat internalFormat;
  };


  struct TextureDeleter final
  {
    void operator()(uint* id);
  };

  class Texture
  {
    protected:
      uint texId;
      uint m_textureType;
    public:
      Texture(uint textureType, bool generateTexture = true);
      Texture(uint texId, uint textureType);
      Texture();
      virtual ~Texture();

      virtual void Enable() const;
      virtual void Enable(uint index) const;
      virtual void Disable() const;

      uint GetTexId() const;
      bool Invalid() const { return texId == 0;};
      friend bool operator<(const Texture& tex1, const Texture& tex2)
      {
        return tex1.texId< tex2.texId;
      }
      friend bool operator==(const Texture& tex1, const Texture& tex2)
      {
        return tex1.texId == tex2.texId;
      }
  };

}
