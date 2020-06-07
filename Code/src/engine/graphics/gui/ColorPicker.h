#pragma once

#include <graphics/gui/Component.h>
#include <graphics/gui/ColorPickerWindow.h>

namespace Greet
{
  class ColorPicker : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(ColorPicker);
    public:
      typedef std::function<void(Component* comp, const Vec3<float>& previous, const Vec3<float>& current)> OnColorChangeCallback;
    public:
      OnColorChangeCallback onColorChangeCallback;
    private:
      ColorPickerWindow* picker;
    public:
      ColorPicker(const XMLObject& object, Component* parent);
      virtual ~ColorPicker();

      void OnEvent(Event& event, const Vec2& translatedPos) override;
      void OnColorChanged(const Vec3<float>& previous, const Vec3<float>& current);

      void SetOnColorChangeCallback(OnColorChangeCallback callback);
      void CallOnColorChangeCallback(const Vec3<float>& previous, const Vec3<float>& current);
  };

}
