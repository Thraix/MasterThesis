#include "ProgressBar.h"

#include <stdlib.h>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(ProgressBar);

  ProgressBar::ProgressBar(const XMLObject& object, Component* parent)
    : Component(object,parent), min(0), max(1), value(0), valueReference(nullptr), vertical(false), reverse(false)
  {
    m_isFocusable = true;
    if(object.HasAttribute("minValue"))
      min = atof(object.GetAttribute("minValue").c_str());
    if(object.HasAttribute("maxValue"))
      max = atof(object.GetAttribute("maxValue").c_str());

    vertical = GUIUtils::GetBooleanFromXML(object,"vertical", false);
    reverse = GUIUtils::GetBooleanFromXML(object,"reverseProgress", false);
    progressColor = GUIUtils::GetColorFromXML(object, "progressColor", Vec4(1,1,1,1));
  }

  void ProgressBar::Render(GUIRenderer* renderer) const
  {
    Vec2 localSize = GetContentSize();
    float percentage = (value-min)/(max-min);
    if(vertical)
      localSize.h *= percentage;
    else
      localSize.w *= percentage;

    Vec2 progressPos = pos + GetBorder().LeftTop() + GetPadding().LeftTop();

    if(reverse)
      renderer->SubmitRoundedRect(progressPos + (GetContentSize() - localSize), localSize, progressColor, currentStyle->radius,currentStyle->roundedPrecision,false);
    else
      renderer->SubmitRoundedRect(progressPos,  localSize, progressColor, currentStyle->radius,currentStyle->roundedPrecision,false);
  }

  void ProgressBar::Update(float timeElapsed)
  {
    if(valueReference != nullptr)
      value = *valueReference;
    Math::Clamp(&value,min,max);
  }

  void ProgressBar::AttachValueReference(float* reference)
  {
    valueReference = reference;
  }

  float ProgressBar::GetValue() const
  {
    return value;
  }

  float* ProgressBar::GetValueReference() const
  {
    return valueReference;
  }

  void ProgressBar::SetValue(float value)
  {
    if(!valueReference)
      Log::Warning("Setting value to ProgressBar when value reference exists");
    this->value = value;
    Math::Clamp(&this->value,min,max);

  }
}
