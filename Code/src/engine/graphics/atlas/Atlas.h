#pragma once

#include <utils/ImageUtils.h>
#include <graphics/textures/Texture.h>
#include <graphics/Sprite.h>
#include <list>
#include <vector>

namespace Greet{
  struct AtlasCoords
  {
    Vec2 coord1;
    Vec2 coord2;
  };

  class Atlas
  {

    private:
      Ref<Texture2D> texture;

      std::map<std::string, uint> textureMap;
      std::vector<bool> occupied;

      uint textureSize;
      uint textureCountSide;
      uint textureCountTotal;

      byte* bits;
    public:
      Atlas(uint atlasSize, uint textureSize);
      virtual ~Atlas();

      bool AddTexture(const std::string& name, const std::string& filePath);
      void RemoveTexture(const std::string& textureName);
      bool AddTexture(const std::vector<byte>& bits, const std::string& name);
      AtlasCoords GetTextureCoords(const std::string& sheetName) const;
      int GetTextureSize() const { return textureSize; }
      int GetAtlasSize() const { return texture->GetWidth(); }

      void Enable(int index = 0) const;
      void Disable() const;
    private:
      void GenTexture(byte* bits);
  };
}
