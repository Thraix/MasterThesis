#pragma once

#include <graphics/gui/Slider.h>

namespace Greet
{
  class HueSlider : public Slider
  {
    private:
      REGISTER_COMPONENT_DECLARATION(HueSlider);
    public:
      HueSlider(const std::string& name, Component* parent);
      HueSlider(const XMLObject& xmlObject, Component* parent);
      void PreRender(GUIRenderer* renderer, const Vec2& translation) const override;
  };
}
