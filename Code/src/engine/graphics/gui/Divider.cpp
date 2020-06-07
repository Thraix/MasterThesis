#include "Divider.h"

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(Divider);

  Divider::Divider(const XMLObject& xmlObject, Component* parent)
    : Component(xmlObject,parent), vertical(false)
  {
    vertical = GUIUtils::GetBooleanFromXML(xmlObject,"vertical", false);

    if(!xmlObject.HasAttribute("width"))
    {
      size.value.w = 1;
      size.size.w = 1;
      if(!vertical)
        size.widthType = ComponentSize::Type::WEIGHT;
      else
        size.widthType = ComponentSize::Type::PIXELS;
    }
    if(!xmlObject.HasAttribute("height"))
    {
      size.value.h = 1;
      size.size.h = 1;
      if(vertical)
        size.heightType = ComponentSize::Type::WEIGHT;
      else
        size.heightType = ComponentSize::Type::PIXELS;
    }
  }

  void Divider::PreRender(GUIRenderer* renderer, const Vec2& translation) const
  {
    Vec2 trans = translation;
    if(parent)
    {
      if(vertical)
        trans.y -= parent->GetPadding().top;
      else
        trans.x -= parent->GetPadding().left;
    }
    Component::PreRender(renderer, trans);
  }

  void Divider::OnMeasured()
  {
    if(parent)
    {
      if(vertical)
        size.size.h += parent->GetPadding().GetHeight();
      else
        size.size.w += parent->GetPadding().GetWidth();
    }
  }

  Vec2 Divider::GetWrapSize() const
  {
    if(parent && !vertical)
      return Vec2(100, 1);
    return Vec2(1, 100);
  }
}
