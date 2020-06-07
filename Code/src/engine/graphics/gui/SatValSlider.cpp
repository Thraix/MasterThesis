#include "SatValSlider.h"

#include <input/InputDefines.h>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(SatValSlider);

  SatValSlider::SatValSlider(const std::string& name, Component* parent)
    : Component{name, parent}, hue{0}, sat{0.5}, val{0.5}
  {
    m_isFocusable = true;
    Style normal{};
    normal.SetBackgroundColor(Vec4(1,1,1,1))
      .SetBorderColor(Vec4(0,0,0,1))
      .SetBorder(TLBR(2,2,2,2));
    Component* component = new Component{"SatValSliderComponent", this};
    component->SetSize(7,7,ComponentSize::Type::PIXELS, ComponentSize::Type::PIXELS, false)
      .SetNormalStyle(normal);
    sliderComponent = component;
  }

  SatValSlider::SatValSlider(const XMLObject& xmlObject, Component* parent)
    : Component(xmlObject, parent)
  {
    m_isFocusable = true;
    if(xmlObject.GetObjectCount() > 0)
      sliderComponent = ComponentFactory::GetComponent(xmlObject.GetObject(0), this);
    else
    {
      Style normal{};
      normal.SetBackgroundColor(Vec4(1,1,1,1)).SetBorderColor(Vec4(0,0,0,1));
      Component* component = new Component{"SatValSliderComponent", this};
      component->SetSize(7,7,ComponentSize::Type::PIXELS, ComponentSize::Type::PIXELS, false).SetNormalStyle(normal);
      sliderComponent = component;
    }
  }

  SatValSlider::~SatValSlider()
  {
    delete sliderComponent;
  }

  void SatValSlider::Measure()
  {
    sliderComponent->Measure();
    Component::Measure();
  }

  void SatValSlider::MeasureFill(float parentEmptyWidth, float parentEmptyHeight, float parentWeight, bool vertical)
  {
    Component::MeasureFill(parentEmptyWidth, parentEmptyHeight, parentWeight, vertical);
    sliderComponent->MeasureFill(GetContentSize().w,GetContentSize().h,1, false);
  }

  void SatValSlider::PreRender(GUIRenderer* renderer, const Vec2& translation) const
  {
    renderer->PushTranslation(translation);
    renderer->SubmitRect(pos, size.size, Vec4(hue,0,0,1), Vec4(hue,1,0,1), Vec4(hue,0,1,1), Vec4(hue,1,1,1),true);
  }

  void SatValSlider::Render(GUIRenderer* renderer) const
  {
    Vec2 sliderOffset = pos + GetTotalPadding() + sliderComponent->GetMargin().LeftTop() - sliderComponent->GetSize()/2;

    sliderComponent->PreRender(renderer,  sliderOffset + Vec2{sat,val} * GetContentSize());
    sliderComponent->RenderHandle(renderer);
    sliderComponent->PostRender(renderer);
  }

  void SatValSlider::OnEvent(Event& event, const Vec2& translatedPos)
  {
    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      if(e.GetButton() == GREET_MOUSE_1 && pressed)
      {
        float oldSat = sat;
        float oldVal = val;

        SetSat(GetSliderSatFromPos(translatedPos.x));
        SetVal(GetSliderValFromPos(translatedPos.y));

        //if(oldSat != newSat || oldVal != newVal)
        //CallOnValueChangeCallback(oldSat, newSat, oldVal, newVal);
      }
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      if(pressed)
      {
        float oldSat = GetSliderSatFromPos(sat);
        float oldVal = GetSliderValFromPos(val);

        SetSat(GetSliderSatFromPos(translatedPos.x));
        SetVal(GetSliderValFromPos(translatedPos.y));

        if(oldSat != sat)
          CallOnSatChangeCallback(oldSat, sat);
        if(oldVal != val)
          CallOnValChangeCallback(oldVal, val);
      }
    }
  }

  void SatValSlider::SetOnSatChangeCallback(OnSatChangeCallback callback)
  {
    onSatChangeCallback = callback;
  }

  void SatValSlider::SetOnValChangeCallback(OnValChangeCallback callback)
  {
    onValChangeCallback = callback;
  }

  void SatValSlider::CallOnSatChangeCallback(float oldSat, float newSat)
  {
    if(onSatChangeCallback)
      onSatChangeCallback(this, oldSat, newSat);
  }

  void SatValSlider::CallOnValChangeCallback(float oldVal, float newVal)
  {
    if(onValChangeCallback)
      onValChangeCallback(this, oldVal, newVal);
  }

  void SatValSlider::SetSat(float sat)
  {
    this->sat= sat;
    Math::Clamp(&this->sat, 0.0f,1.0f);
  }

  void SatValSlider::SetHue(float hue)
  {
    this->hue = hue;
  }

  void SatValSlider::SetVal(float val)
  {
    this->val = val;
    Math::Clamp(&this->val, 0.0f,1.0f);
  }

  float SatValSlider::GetSat() const
  {
    return sat;
  }

  float SatValSlider::GetVal() const
  {
    return val;
  }

  float SatValSlider::GetSliderSatFromPos(float pos) const
  {
    return pos / GetContentSize().w;
  }

  float SatValSlider::GetSliderPosFromSat(float value) const
  {
    return GetContentSize().w * value;
  }

  float SatValSlider::GetSliderValFromPos(float pos) const
  {
    return pos / GetContentSize().h;
  }

  float SatValSlider::GetSliderPosFromVal(float value) const
  {
    return GetContentSize().h * value;
  }
}
