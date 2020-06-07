#pragma once

#include <graphics/gui/Component.h>

namespace Greet
{
  class Slider : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(Slider);
    public:
      typedef std::function<void(Component*, float oldValue, float newValue)> OnValueChangeCallback;
    protected:
      static uint SLIDER_FLAG_FORCE_INSIDE;
      static uint SLIDER_FLAG_SNAP;
      static uint SLIDER_FLAG_VERTICAL;

      // The slider indicator component can be any component really
      Component* sliderComponent;
      float sliderPos;
      uint flags;
      float minValue;
      float maxValue;
      float stepSize;

      float minPos;
      float maxPos;

      OnValueChangeCallback onValueChangeCallback;



    public:
      Slider(const std::string& name, Component* parent);
      Slider(const XMLObject& xmlObject, Component* parent);

      virtual void Measure() override;
      virtual void MeasureFill(float parentEmptyWidth, float parentEmptyHeight, float parentWeight, bool vertical) override;
      virtual void OnMeasured() override;

      virtual void Render(GUIRenderer* renderer) const override;

      virtual void OnEvent(Event& event, const Vec2& translatedPos) override;

      // Callbacks
      virtual void SetOnValueChangeCallback(OnValueChangeCallback callback);

      Slider& SetVertical(bool vertical);

      Component* GetSliderComponent();

      float GetValue() const;
      void SetValue(float value);
      float GetSnappedSlider(float sliderValue) const;
      float GetSliderValueFromPos(float pos) const;
      float GetSliderPosFromValue(float value) const;
    protected:
      float GetSliderDirectionSize() const;
      virtual void CallOnValueChangeCallback(float oldValue, float newValue);
  };
}
