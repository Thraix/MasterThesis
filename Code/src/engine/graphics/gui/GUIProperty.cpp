#if 0
#include "GUIProperty.h"

namespace Greet
{
  float GUIProperty::GetAttributeFloat(const Property& property, const XMLObject& xmlObject, Content* content, float defaultValue)
  {
    if(xmlObject.HasAttribute(property))
    {
      switch(property)
      {
        case WIDTH: case MARGIN_LEFT: case MARGIN_RIGHT: case BORDER_LEFT: case BORDER_RIGHT:
          return GUIUtils::CalcSize(xmlObject.GetAttribute(property), content->GetPotentialWidth());
        case HEIGHT: case MARGIN_TOP: case MARGIN_BOTTOM: case BORDER_TOP: case BORDER_BOTTOM:
          return GUIUtils::CalcSize(xmlObject.GetAttribute(property), content->GetPotentialHeight());
        default:
          Log::Warning("Invalid property, not float: ", property);
          return defaultValue;
      }
    }
    return defaultValue;
  }
  Vec4 GUIProperty::GetAttributeVec4(const Property& property, Content* content, const Vec4& defaultValue)
  {
    if(xmlObject.HasAttribute(property))
    {
      switch(property)
      {
        case BACKGROUND_COLOR: case BORDER_COLOR:
          return GUIUtils::GetColor(xmlObject.GetAttribute(property));
        default:
          Log::Warning("Invalid property, not Vec4: ", property);
          return defaultValue;
      }
    }
    return defaultValue;
  }
  std::string GUIProperty::GetAttributeName(const Property& property)
  {
    switch(property)
    {

    }
  }
}
#endif
