#pragma once

#include <event/Event.h>

namespace Greet
{
  class WindowResizeEvent : public Event
  {
    protected:
      int width;
      int height;
    public:
      WindowResizeEvent(int width, int height)
        : width{width}, height{height}
      {}

      inline const int GetWidth() const { return width; }
      inline const int GetHeight() const { return height; }
      uint GetCategory() const {return WINDOW_EVENT;}
      EventType GetType() const {return EventType::WINDOW_RESIZE;}
  };

  class WindowMoveEvent : public Event
  {
    protected:
      int x;
      int y;
      int dx;
      int dy;

    public:
      WindowMoveEvent(int x, int y,int dx, int dy)
        : x{x}, y{y}
      {}

      inline const int GetX() const { return x; }
      inline const int GetY() const { return y; }
      inline const int GetDX() const { return x; }
      inline const int GetDY() const { return y; }
      uint GetCategory() const {return WINDOW_EVENT;}
      EventType GetType() const {return EventType::WINDOW_MOVE;}
  };
  class WindowFocusChangeEvent : public Event
  {
    protected:
      WindowFocusChangeEvent()
      {}
    public:
      uint GetCategory() const {return WINDOW_EVENT;}
      virtual bool IsFocused() const = 0;
  };

  class WindowFocusEvent : public WindowFocusChangeEvent
  {
    public:
      WindowFocusEvent()
      {}

      EventType GetType() const { return EventType::WINDOW_FOCUS; }
      bool IsFocused() const { return true; }
  };

  class WindowUnfocusEvent : public WindowFocusChangeEvent
  {
    public:
      WindowUnfocusEvent()
      {}

      EventType GetType() const { return EventType::WINDOW_UNFOCUS; }
      bool IsFocused() const { return false; }
  };
}
