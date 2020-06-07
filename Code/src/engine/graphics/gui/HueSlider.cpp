#include "HueSlider.h"

#include <math/Maths.h>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(HueSlider);

  HueSlider::HueSlider(const std::string& name, Component* parent)
    : Slider{name, parent}
  {
    minValue = 0;
    maxValue = 1.0;
    SetValue(0.5);
  }

  HueSlider::HueSlider(const XMLObject& xmlObject, Component* parent)
    : Slider(xmlObject, parent)
  {
    minValue = 0;
    maxValue = 1.0;
    SetValue(0.5);
  }

  //void SubmitRect(const Vec2& pos, const Vec2& size, const Vec4& color1, const Vec4& color2, const Vec4& color3, const Vec4& color4, bool isHsv = true);
  void HueSlider::PreRender(GUIRenderer* renderer, const Vec2& translation) const
  {
    renderer->PushTranslation(translation);
    renderer->SubmitRect(pos, size.size, Vec4(0,1,1,1), Vec4(0,1,1,1), Vec4(1,1,1,1), Vec4(1,1,1,1),true);
  }
}
