#pragma once

#include <graphics/gui/TLBR.h>

namespace Greet
{

  class Component;

  struct Style
  {
    public:
      TLBR margin;
      TLBR padding;
      TLBR border;
      bool hasBackgroundColor;
      bool hasBorderColor;
      Vec4 backgroundColor;
      Vec4 borderColor;
      float borderRadius;
      float radius;
      uint roundedPrecision;

      Style()
        : margin(0,0,0,0), padding(0,0,0,0), border(0,0,0,0), backgroundColor(Vec4(1,1,1,1)), borderColor(Vec4(0,0,0,0)), borderRadius(0.0), radius(0.0), roundedPrecision(3), hasBackgroundColor(false), hasBorderColor(false)
      {

      }

      void Load(const std::string& prefix, const XMLObject& object)
      {
        Load(prefix,object,nullptr);
      }

      void Load(const std::string& prefix, const XMLObject& object, Style const * inherit);

      Style& SetMargin(const TLBR& margin);
      Style& SetPadding(const TLBR& padding);
      Style& SetBorder(const TLBR& border);
      Style& SetHasBackgroundColor(bool hasColor);
      Style& SetHasBorderColor(bool hasColor);
      Style& SetBackgroundColor(const Vec4& color);
      Style& SetBorderColor(const Vec4& color);
      Style& SetBorderRadius(float radius);
      Style& SetRadius(float radius);
      Style& SetRoundedPrecision(uint precision);
  };
}
