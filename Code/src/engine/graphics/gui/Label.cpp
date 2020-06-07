#include "Label.h"

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(Label);

  Label::Label(const std::string& name, Component* parent)
    : Label{name,parent, "", "", 20}
  {
  }

  Label::Label(const std::string& name, Component* parent, const std::string& text, const std::string& fontName, float fontSize)
    : Component{name, parent}, str{text}, fontSize{fontSize}, color{0,0,0,1}, hasMaxWidth{false}
  {
    font = FontManager::Get(fontName, fontSize);
    gravity = Gravity::CENTER;
  }

  Label::Label(const XMLObject& object, Component* parent)
    : Component(object, parent), str{object.GetText()}, color{0,0,0,1}, hasMaxWidth{false}
  {
    fontSize = GUIUtils::GetFloatFromXML(object, "fontSize", 20);
    font = FontManager::Get(GUIUtils::GetStringFromXML(object,"font",""),fontSize);

    std::string grav = object.GetAttribute("gravity", "center");
    if(grav == "top")
      gravity = Gravity::TOP;
    else if(grav == "bottom")
      gravity = Gravity::BOTTOM;
    else// if(grav == "center") or invalid
      gravity = Gravity::CENTER;

    color = GUIUtils::GetColorFromXML(object,"color",Vec4(0,0,0,1));
  }

  void Label::Render(GUIRenderer* renderer) const
  {
    if(gravity == Gravity::TOP)
      renderer->SubmitString(str, pos + GetTotalPadding() + Vec2(0, font->GetBaselineOffset()), font, color, false);
    else if(gravity == Gravity::CENTER)
      renderer->SubmitString(str, pos + GetTotalPadding() + Vec2(0, GetContentSize().h + font->GetBaselineOffset())/2, font, color, false);
    else// if(gravity == Gravity::BOTTOM)
      renderer->SubmitString(str, pos + GetTotalPadding() + Vec2(0, GetContentSize().h), font, color, false);
  }

  Label& Label::SetText(const std::string& text)
  {
    str = text;
    Remeasure();
    return *this;
  }

  const std::string& Label::GetText() const
  {
    return str;
  }

  Vec2 Label::GetWrapSize() const
  {
    float width = font->GetWidthOfText(str);
    return Vec2(hasMaxWidth ? Math::Min(width, maxWidth) : width, font->GetSize());
  }

  const Vec4& Label::GetColor() const
  {
    return color;
  }

  const Font* Label::GetFont() const
  {
    return font;
  }

  float Label::GetFontSize() const
  {
    return fontSize;
  }

  Label& Label::SetGravity(Gravity grav)
  {
    gravity = grav;
    return *this;
  }

  Label& Label::SetFont(const std::string& font)
  {
    this->font = FontManager::Get(font, fontSize);
    return *this;
  }

  Label& Label::SetFontSize(float fontSize)
  {
    font = FontManager::Get(font->GetFontContainer()->GetName(), fontSize);
    this->fontSize = fontSize;
    Remeasure();
    return *this;
  }

  Label& Label::SetColor(const Vec4& color)
  {
    this->color = color;
    return *this;
  }
}
