#pragma once

#include <graphics/renderers/Renderer2D.h>

namespace Greet {
  class Renderable
  {
    public:

      bool render = true;

      Renderable()
      {

      }

      virtual ~Renderable()
      {

      }
      virtual void Begin(Renderer2D* renderer) const {}
      virtual void Submit(Renderer2D* renderer) const = 0;
      virtual void End(Renderer2D* renderer) const {}

      virtual bool Update(float timeElapsed) = 0;

      virtual void SetColor(uint color) = 0;
      virtual inline uint GetColor() const = 0;
      virtual void SetPosition(const Vec2& pos) = 0;
      virtual inline const Vec2& GetPosition() const = 0;
      virtual void SetSize(const Vec2& size) = 0;
      virtual inline const Vec2& GetSize() const = 0;
  };
}
