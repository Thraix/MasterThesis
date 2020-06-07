#pragma once

#include <math/Maths.h>
#include <event/Event.h>

namespace Greet {

  class JoystickButtonEvent : public Event
  {
    protected:
      int joystick;
      int button;
      JoystickButtonEvent (int joystick, int button)
        : joystick{joystick}, button{button} {}
    public:
      int GetJoystick() const { return joystick; }
      int GetButton() const { return button; }
      uint GetCategory() const {return INPUT_EVENT | JOYSTICK_EVENT;}
  };

  class JoystickPressEvent : public JoystickButtonEvent
  {
    public:
      JoystickPressEvent(int joystick, int button)
        : JoystickButtonEvent(joystick, button)
      {}

      EventType GetType() const {return EventType::JOYSTICK_PRESS;}
  };

  class JoystickReleaseEvent : public JoystickButtonEvent
  {
    public:
      JoystickReleaseEvent(int joystick, int button)
        : JoystickButtonEvent(joystick, button)
      {}

      EventType GetType() const {return EventType::JOYSTICK_RELEASE;}
  };

  class JoystickStickMoveEvent : public Event
  {
    private:
      int joystick;
      int stickXNum;
      int stickYNum;
      float x;
      float y;
      float dx;
      float dy;

    public:
      JoystickStickMoveEvent(int joystick, int stickXNum, int stickYNum, float x, float y, float dx, float dy)
        : joystick{joystick}, stickXNum{stickXNum}, stickYNum{stickYNum}, x{x}, y{y}, dx{dx}, dy{dy}
      {}

      int GetJoystick() const { return joystick; }
      int GetStickXNum() const { return stickXNum; }
      int GetStickYNum() const { return stickYNum; }
      float GetX() const { return x; }
      float GetY() const { return y; }
      float GetDX() const { return dx; }
      float GetDY() const { return dy; }

      EventType GetType() const {return EventType::JOYSTICK_STICK_MOVE;}
      uint GetCategory() const {return INPUT_EVENT | JOYSTICK_EVENT;}
  };

  class JoystickTriggerMoveEvent : public Event
  {
    private:
      int joystick;
      int triggerNum;
      float val;
      float dVal;

    public:
      JoystickTriggerMoveEvent(int joystick, int triggerNum, float val, float dVal)
        : joystick{joystick}, triggerNum{triggerNum}, val{val}, dVal{dVal}
      {}

      int GetJoystick() const { return joystick; }
      int GetTriggerNum() const { return triggerNum; }
      float GetValue() const { return val; }
      float GetDeltaValue() const { return dVal; }

      EventType GetType() const {return EventType::JOYSTICK_STICK_MOVE;}
      uint GetCategory() const {return INPUT_EVENT | JOYSTICK_EVENT;}
  };

  class JoystickConnectionEvent : public Event
  {
    private:
      int joystick;

    protected:
      JoystickConnectionEvent(int joystick)
        : joystick{joystick}
      {}
    public:
      int GetJoystick() const { return joystick; }
      uint GetCategory() const {return JOYSTICK_EVENT;}
  };

  class JoystickConnectEvent : public JoystickConnectionEvent
  {
    public:
      JoystickConnectEvent(int joystick)
        : JoystickConnectionEvent{joystick}
      {}
      EventType GetType() const { return EventType::JOYSTICK_CONNECT;}
  };

  class JoystickDisconnectEvent : public JoystickConnectionEvent
  {
    public:
      JoystickDisconnectEvent(int joystick)
        : JoystickConnectionEvent{joystick}
      {}
      EventType GetType() const { return EventType::JOYSTICK_DISCONNECT;}
  };

}
