#include "Style.h"

#include <graphics/gui/Component.h>
#include <graphics/gui/GUIScene.h>

namespace Greet
{
  void Style::Load(const std::string& prefix, const XMLObject& object, Style const * inherit)
  {
    margin.Load(prefix+"margin", object);
    padding.Load(prefix+"padding", object);
    border.Load(prefix+"border", object);

    radius = GUIUtils::GetFloatFromXML(object, prefix+"radius",0);
    borderRadius = GUIUtils::GetFloatFromXML(object, prefix+"borderRadius",radius);
    roundedPrecision = GUIUtils::GetIntFromXML(object, prefix+"roundedPrecision", 3);

    if(object.HasAttribute(prefix+"backgroundColor"))
      hasBackgroundColor = true;
    if(object.HasAttribute(prefix+"borderColor"))
      hasBorderColor = true;

    backgroundColor = GUIUtils::GetColorFromXML(object, prefix+"backgroundColor",Vec4(0,0,0,1));
    borderColor = GUIUtils::GetColorFromXML(object, prefix+"borderColor",Vec4(0,0,0,1));
  }

  Style& Style::SetMargin(const TLBR& margin)
  {
    this->margin = margin;
    return *this;
  }

  Style& Style::SetPadding(const TLBR& padding)
  {
    this->padding = padding;
    return *this;
  }

  Style& Style::SetBorder(const TLBR& border)
  {
    this->border= border;
    return *this;
  }

  Style& Style::SetHasBackgroundColor(bool hasColor)
  {
    hasBackgroundColor = hasColor;
    return *this;
  }

  Style& Style::SetHasBorderColor(bool hasColor)
  {
    hasBorderColor = hasColor;
    return *this;
  }

  Style& Style::SetBackgroundColor(const Vec4& color)
  {
    backgroundColor = color;
    hasBackgroundColor = true;
    return *this;
  }

  Style& Style::SetBorderColor(const Vec4& color)
  {
    borderColor = color;
    hasBorderColor = true;
    return *this;
  }

  Style& Style::SetBorderRadius(float radius)
  {
    borderRadius = radius;
    return *this;
  }

  Style& Style::SetRadius(float radius)
  {
    this->radius = radius;
    return *this;
  }

  Style& Style::SetRoundedPrecision(uint precision)
  {
    roundedPrecision = precision;
    return *this;
  }

}
