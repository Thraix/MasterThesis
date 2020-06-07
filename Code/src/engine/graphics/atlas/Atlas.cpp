#include "Atlas.h"

#include <internal/GreetGL.h>

namespace Greet {

  Atlas::Atlas(uint atlasSize, uint textureSize)
    : texture(Texture2D::Create(atlasSize, atlasSize, TextureParams(TextureFilter::NEAREST,TextureWrap::NONE,TextureInternalFormat::RGBA))), textureSize(textureSize)
  {
    ASSERT(atlasSize > textureSize, "ATLAS", "Atlas size must be greater than the textures sizes");
    //ASSERT(!(atlasSize == 0) && !(atlasSize & (atlasSize - 1)),"ATLAS", "Atlas size must be a power of two");
    //ASSERT(!(textureSize == 0) && !(textureSize & (textureSize - 1)), "ATLAS", "Texture size must be a power of two");
    textureCountSide = atlasSize / textureSize;
    textureCountTotal = textureCountSide * textureCountSide;
    uint bitCount = atlasSize * atlasSize * 4;

    occupied.resize(textureCountTotal);
    std::fill(occupied.begin(), occupied.end(), false);

    std::vector<byte> bits(bitCount);
    for (uint i = 0; i < bitCount; i+=4)
    {
      bits[i]   = 255;
      bits[i+1] = 0;
      bits[i+2] = 255;
      bits[i+3] = 255;
    }

    texture->SetPixels(bits);
  }

  Atlas::~Atlas()
  {
  }

  bool Atlas::AddTexture(const std::string& name, const std::string& filePath)
  {
    uint width;
    uint height;
    auto res = ImageUtils::LoadImage(filePath.c_str(), &width, &height);
    if (width != textureSize || height != textureSize)
    {
      Log::Error("The given textures size is not valid: ",name.c_str()," (",width,",",height,")");
      return false;
    }
    bool success = AddTexture(res.second,name);

    return success;
  }

  bool Atlas::AddTexture(const std::vector<byte>& bits, const std::string& name)
  {
    if (textureMap.size() >= textureCountTotal)
    {
      Log::Error("There is no more room in the Atlas. Increase size or create a new one.");
      return false;
    }
    uint x = textureCountSide;
    uint y = textureCountSide;

    for (uint i = 0; i < textureCountTotal; i++)
    {
      if (!occupied[i])
      {
        textureMap.emplace(name,i);
        occupied[i] = true;
        x = i % textureCountSide;
        y = (i - x) / textureCountSide;
        if(bits.size() == 0)
        {
          GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, x*textureSize,textureSize*textureCountSide -textureSize - y*textureSize,textureSize,textureSize,GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
        }
        else
        {
          GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, x*textureSize,textureSize*textureCountSide -textureSize - y*textureSize,textureSize,textureSize,GL_RGBA, GL_UNSIGNED_BYTE, bits.data()));
        }
        return true;
      }
    }
    ASSERT(false, "AtlasManager failed to find unoccupied space, even though it passed a space check");
  }

  AtlasCoords Atlas::GetTextureCoords(const std::string& sheetName) const
  {
    auto it = textureMap.find(sheetName);
    if(it != textureMap.end())
    {
      uint j = it->second;
      uint x = j % textureCountSide;
      uint y = (j - x) / textureCountSide;

      float size = textureSize / (float)texture->GetWidth();
      Vec2 coord1 = Vec2(size, size) * Vec2(x, y);
      Vec2 coord2 = coord1 + Vec2(size, size);
      return AtlasCoords{coord1, coord2};
    }

    Log::Error("No texture found in Atlas: (", sheetName.c_str(), ")");
    return AtlasCoords{{0,0}, {1,1}};
  }

  void Atlas::RemoveTexture(const std::string& textureName)
  {
    auto it = textureMap.find(textureName);
    if(it != textureMap.end())
    {
      occupied[it->second] = false;
      textureMap.erase(it);
    }
  }

  void Atlas::Enable(int index) const
  {
    texture->Enable(index);
  }

  void Atlas::Disable() const
  {
    texture->Disable();
  }
}
