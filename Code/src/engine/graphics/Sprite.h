#pragma once

#include <common/Types.h>
#include <math/Maths.h>
#include <graphics/textures/Texture2D.h>
#include <graphics/textures/TextureManager.h>

namespace Greet{
  class Sprite
  {
    protected:
      Ref<Texture2D> texture;
      Vec2 texPos;
      Vec2 texSize;
    public:
      Sprite()
        : texture(nullptr), texPos(Vec2(0, 0)), texSize(Vec2(1, 1))
      {

      }

      virtual bool Update(float timeElapsed) { return false; }
      Sprite(const Ref<Texture2D>& texture)
        : texture(texture), texPos(Vec2(0, 0)), texSize(Vec2(1, 1))
      {

      }

      Sprite(const Ref<Texture2D>& texture, Vec2 texPos, Vec2 texSize)
        : texture(texture), texPos(texPos), texSize(texSize)
      {

      }

      virtual ~Sprite() {}


      inline uint GetTextureID() const { return texture->GetTexId(); }

      inline Sprite* FromSpriteSheet(Vec2 texPos, Vec2 texSize) const
      {
        return new Sprite(texture, texPos + texPos*texSize, texSize*texSize);
      }
      virtual inline const Vec2& GetTexPos() const { return texPos; }
      inline const Vec2& GetTexSize() const { return texSize; }
  };
}
