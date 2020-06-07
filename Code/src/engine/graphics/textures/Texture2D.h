#pragma once

#include <graphics/textures/Texture.h>
#include <utils/ImageUtils.h>
#include <utils/HotSwapping.h>
#include <common/Memory.h>

namespace Greet {

  class Texture2D : public Texture, public Resource
  {
    public:
      friend class FrameBufferObject;
    private:
      uint width;
      uint height;
      TextureParams params;

    private:
      Texture2D(uint width, uint height, TextureParams params = TextureParams());
      Texture2D(const std::string& filename, TextureParams params = TextureParams());
      Texture2D(const std::vector<byte>& bits, uint width, uint height, TextureParams params = TextureParams());
      Texture2D(uint texID, uint width, uint height, TextureParams params = TextureParams());
      Texture2D(TextureParams params = TextureParams());

    public:
      Texture2D(Texture2D&&) = default;
      Texture2D& operator=(Texture2D&&) = default;

      virtual ~Texture2D();

      void SetPixels(const std::vector<byte>& pixels) const;
      void ReloadResource() override;
      inline uint GetWidth() const { return width; }
      inline uint GetHeight() const { return height; }

      static Ref<Texture2D> Create(uint width, uint height, TextureParams params = TextureParams());
      static Ref<Texture2D> Create(const std::string& filename, TextureParams params = TextureParams());
      static Ref<Texture2D> Create(const std::vector<byte>& bits, uint width, uint height, TextureParams params = TextureParams());
      static Ref<Texture2D> Create(uint texID, uint width, uint height, TextureParams params = TextureParams());
      static Ref<Texture2D> Create(TextureParams params = TextureParams());
    private:
      void LoadTexture(const std::string& filename);
      void GenTexture(const std::vector<byte>& bits);

  };
}
