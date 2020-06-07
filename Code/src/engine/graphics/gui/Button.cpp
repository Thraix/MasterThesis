#include "Button.h"

#include <graphics/fonts/FontManager.h>

namespace Greet {

  REGISTER_COMPONENT_DEFINITION(Button);

  Button::Button(const XMLObject& object, Component* parent)
    : Component(object, parent)
  {
    m_isFocusable = true;
    label = new Label("Label", this,object.GetText(),GUIUtils::GetStringFromXML(object, "font",""),GUIUtils::GetFloatFromXML(object,"fontSize",20) );
    label->SetColor(GUIUtils::GetColorFromXML(object, "color", Vec4(0,0,0,1)))
      .SetSize(1,1, ComponentSize::Type::WRAP, ComponentSize::Type::WEIGHT);

    fontSize = GUIUtils::GetFloatFromXML(object, "fontSize", 20);
  }

  Button::~Button()
  {
    delete label;
  }

  void Button::OnMeasured()
  {
    label->Measure();
    label->MeasureFill(GetContentSize().w,GetContentSize().h,1,true);
  }

  void Button::Render(GUIRenderer* renderer) const
  {
    Vec2 p = pos + GetTotalPadding() +  Vec2(
        (GetContentSize().w-label->GetWidth())/2, 0);
    renderer->PushTranslation(p);
    label->Render(renderer);
    renderer->PopTranslation();
  }

  void Button::OnFocused()
  {
  }

  void Button::OnUnfocused()
  {
  }

  void Button::MouseEntered()
  {
    currentStyle = &hoverStyle;
  }

  void Button::MouseExited()
  {
    currentStyle = &normalStyle;
  }
}
