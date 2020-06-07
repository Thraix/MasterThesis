#include "Slider.h"

#include <input/InputDefines.h>
#include <math/Maths.h>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(Slider);

  uint Slider::SLIDER_FLAG_FORCE_INSIDE = BIT(0);
  uint Slider::SLIDER_FLAG_SNAP = BIT(1);
  uint Slider::SLIDER_FLAG_VERTICAL = BIT(2);

  Slider::Slider(const std::string& name, Component* parent)
    : Component{name, parent}, flags{0}
  {
    m_isFocusable = true;
    Style normal{};
    normal.SetBackgroundColor(Vec4(1,1,1,1))
      .SetBorderColor(Vec4(0,0,0,1))
      .SetBorder(TLBR(2,2,2,2));
    sliderComponent = new Component(name+"#SliderComponent", this);
    sliderComponent->SetSize(7,1,ComponentSize::Type::PIXELS, ComponentSize::Type::WEIGHT)
      .SetNormalStyle(normal);

    minValue = 0;
    maxValue = 100;
    stepSize = 0;
    flags |= SLIDER_FLAG_VERTICAL;
    // Bit of a hack to make the initial value correct
    minPos = 0;
    maxPos = 1;

    SetValue((maxValue-minValue)/2.0f);
  }

  Slider::Slider(const XMLObject& xmlObject, Component* parent)
    : Component(xmlObject, parent), flags{0}
  {
    m_isFocusable = true;
    if(xmlObject.GetObjectCount() > 0)
      sliderComponent = ComponentFactory::GetComponent(xmlObject.GetObject(0), this);

    minValue = GUIUtils::GetFloatFromXML(xmlObject, "minValue", 0.0f);
    maxValue = GUIUtils::GetFloatFromXML(xmlObject, "maxValue", 100.0f);
    stepSize = GUIUtils::GetFloatFromXML(xmlObject, "stepSize", 0.0f);

    flags |= GUIUtils::GetBooleanFromXML(xmlObject, "indicatorInside", false) ? SLIDER_FLAG_FORCE_INSIDE : false;
    flags |= GUIUtils::GetBooleanFromXML(xmlObject, "vertical", false) ? SLIDER_FLAG_VERTICAL : false;

    if(maxValue - minValue < 0)
    {
      Log::Warning("Min value in slider is greater than max value");
      std::swap(minValue,maxValue);
    }

    //if((maxValue - minValue) / stepSize > 1.0001)
    //  Log::Warning("Step size in Slider doesn't divide max - min evenly");


    if(stepSize < 0)
    {
      stepSize = -stepSize;
      Log::Warning("Step size in Slider is negative");
    }

    if(stepSize > 0)
      flags |= SLIDER_FLAG_SNAP;


    // Bit of a hack to make the initial value correct
    minPos = 0;
    maxPos = 1;
    // Default defaultValue should be in the middle
    SetValue(GUIUtils::GetFloatFromXML(xmlObject, "defaultValue", (maxValue-minValue)/2.0f));
  }

  void Slider::Measure()
  {
    sliderComponent->Measure();
    Component::Measure();
  }

  void Slider::MeasureFill(float parentEmptyWidth, float parentEmptyHeight, float parentWeight, bool vertical)
  {
    Component::MeasureFill(parentEmptyWidth, parentEmptyHeight, parentWeight, vertical);
    sliderComponent->MeasureFill(GetContentSize().w,GetContentSize().h,1,flags & SLIDER_FLAG_VERTICAL);
  }

  void Slider::OnMeasured()
  {
    float value = GetValue();

    // vi will be the y value of the sizes if vertical and x value if not
    uint vi = (flags & SLIDER_FLAG_VERTICAL) ? 1 : 0;

    if(flags & SLIDER_FLAG_FORCE_INSIDE)
    {
      minPos = sliderComponent->GetSize()[vi]/2;
      maxPos = size.size[vi]-GetBorder().GetSize()[vi]-minPos;
    }
    else
    {
      minPos = 0;
      maxPos = size.size[vi];
    }

    SetValue(value);
  }

  void Slider::Render(GUIRenderer* renderer) const
  {
    Vec2 sliderOffset = pos + GetTotalPadding() + sliderComponent->GetMargin().LeftTop() - sliderComponent->GetSize()/2;
    if(flags & SLIDER_FLAG_VERTICAL)
      sliderComponent->PreRender(renderer,  sliderOffset + Vec2{GetContentSize().w/2, sliderPos});
    else
      sliderComponent->PreRender(renderer,  sliderOffset + Vec2{sliderPos, GetContentSize().h/2});
    sliderComponent->RenderHandle(renderer);
    sliderComponent->PostRender(renderer);
  }

  void Slider::OnEvent(Event& event, const Vec2& translatedPos)
  {
    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      if(e.GetButton() == GREET_MOUSE_1 && pressed)
      {
        float oldValue = GetSliderValueFromPos(sliderPos);
        if(flags & SLIDER_FLAG_VERTICAL)
          SetValue(GetSliderValueFromPos(translatedPos.y));
        else
          SetValue(GetSliderValueFromPos(translatedPos.x));
        float newValue = GetSliderValueFromPos(sliderPos);
        if(oldValue != newValue)
          CallOnValueChangeCallback(oldValue, newValue);
      }
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      MouseMoveEvent& e = static_cast<MouseMoveEvent&>(event);
      if(pressed)
      {
        float oldValue = GetSliderValueFromPos(sliderPos);
        if(flags & SLIDER_FLAG_VERTICAL)
          SetValue(GetSliderValueFromPos(translatedPos.y));
        else
          SetValue(GetSliderValueFromPos(translatedPos.x));
        float newValue = GetSliderValueFromPos(sliderPos);
        if(oldValue != newValue)
          CallOnValueChangeCallback(oldValue, newValue);
      }
    }

  }

  void Slider::SetOnValueChangeCallback(OnValueChangeCallback callback)
  {
    onValueChangeCallback = callback;
  }

  void Slider::CallOnValueChangeCallback(float oldValue, float newValue)
  {
    if(onValueChangeCallback)
      onValueChangeCallback(this, oldValue, newValue);
  }

  Slider& Slider::SetVertical(bool vertical)
  {
    if(vertical)
      flags |= SLIDER_FLAG_VERTICAL;
    else
      flags &= ~SLIDER_FLAG_VERTICAL;
    return *this;
  }

  Component* Slider::GetSliderComponent()
  {
    return sliderComponent;
  }

  float Slider::GetValue() const
  {
    return GetSliderValueFromPos(sliderPos);
  }

  void Slider::SetValue(float value)
  {
    Math::Clamp(&value, minValue, maxValue);
    if(flags & SLIDER_FLAG_SNAP)
      value = GetSnappedSlider(value);
    sliderPos = GetSliderPosFromValue(value);
  }

  float Slider::GetSnappedSlider(float sliderValue) const
  {
    return Math::RoundClose(sliderValue,stepSize);
  }

  float Slider::GetSliderValueFromPos(float pos) const
  {
    return ((pos - minPos) / (maxPos - minPos)) * (maxValue-minValue) + minValue;
  }

  float Slider::GetSliderPosFromValue(float value) const
  {
    return (value - minValue) / (maxValue - minValue) * (maxPos-minPos) + minPos;
  }
}
