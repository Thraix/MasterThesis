#pragma once

#include <math/Maths.h>
#include <event/Event.h>

namespace Greet {

  class KeyEvent : public Event
  {
    protected:
      uint button;
    protected:
      KeyEvent(uint button)
        : button(button) {}
    public:
      uint GetButton() const { return button; }
      uint GetCategory() const {return INPUT_EVENT | KEYBOARD_EVENT;}
  };

  class KeyPressEvent : public KeyEvent
  {
    protected:
      uint repeat;
    public:
      KeyPressEvent(uint button, uint repeat)
        : KeyEvent(button), repeat{repeat} {}
      EventType GetType() const {return EventType::KEY_PRESS;}
  };

  class KeyReleaseEvent : public KeyEvent
  {
    public:
      KeyReleaseEvent(uint button)
        : KeyEvent(button) {}
      EventType GetType() const {return EventType::KEY_RELEASE;}
  };

  class KeyTypeEvent : public KeyEvent
  {
    private:
      uint charCode;

    public:
      KeyTypeEvent(uint charCode) : KeyEvent(charCode) {}
      EventType GetType() const {return EventType::KEY_TYPE;}
  };

}
