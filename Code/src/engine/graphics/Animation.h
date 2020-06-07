#pragma once

#include <math/Maths.h>
#include <graphics/Sprite.h>

namespace Greet {
  class Animation : public Sprite
  {
    private:
      Vec2 add;

      uint image;
      uint images;
      bool horizontalRead;
      float frameTime;
      float frameTimer;
      Vec2 texPos2;
    public:
      Animation(const Ref<Texture2D>& texture, Vec2 texPos, Vec2 texSize, float frameTimer, uint images, bool horizontalRead);
      Animation(const Sprite& sprite, float frameTimer, uint images, bool horizontalRead);
      virtual ~Animation();

      bool Update(float elapsedTime) override;

      void SetTexPos(Vec2 texSize);

      inline const Vec2& GetTexPos() const override { return texPos2; }
  };
}
