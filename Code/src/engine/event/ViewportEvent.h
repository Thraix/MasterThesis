#pragma once

#include <event/Event.h>
#include <math/Vec2.h>

namespace Greet
{
  class ViewportResizeEvent : public Event
  {
    protected:
      Vec2 pos;
      Vec2 size;
    public:
      ViewportResizeEvent(float x, float y, float width, float height)
        : pos{x,y}, size{width, height}
      {}

      ViewportResizeEvent(const Vec2& pos, const Vec2& size)
        : pos{pos}, size{size}
      {}

      float GetX() const { return pos.w; }
      float GetY() const { return pos.h; }
      float GetWidth() const { return size.w; }
      float GetHeight() const { return size.h; }
      const Vec2& GetPosition() const { return pos; }
      const Vec2& GetSize() const { return size; }

      uint GetCategory() const {return VIEWPORT_EVENT;}
      EventType GetType() const {return EventType::VIEWPORT_RESIZE;}
  };
}
