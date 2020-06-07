#pragma once

#include <graphics/gui/Component.h>

namespace Greet
{
  // This class will ignore parent padding?
  class Divider : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(Divider);
    protected:
      bool vertical;
    public:
      Divider(const XMLObject& object, Component* parent);
      void PreRender(GUIRenderer* renderer, const Vec2& translation) const override;

      virtual void OnMeasured() override;

      // Will return a newly calculated value
      virtual Vec2 GetWrapSize() const override;
  };
}
