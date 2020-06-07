#pragma once
#if 0

#include <graphics/gui/Content.h>

namespace Greet
{

  class GUIProperty
  {
    public:
      enum Property{
        WIDTH = "width", HEIGHT = "height", SPACING = "spacing",
        MARGIN_TOP = "marginTop", MARGIN_LEFT = "marginLeft", MARGIN_BOTTOM = "marginBottom", MARGIN_RIGHT = "marginRight",
        BORDER_TOP = "borderTop", BORDER_LEFT = "borderLeft", BORDER_BOTTOM = "merginBottom", BORDER_RIGHT = "borderRight",
        BACKGROUND_COLOR = "backgroundColor", BORDER_COLOR = "borderColor"
      };

      static float GetPropertyFloat(const Property& property, Content* content, float defaultValue);
      static Vec4 GetPropertyVec4(const Property& property, Content* content, const Vec4& defaultValue);
    private:
      static std::string GetPropertyName(const Property& property);
  };

}
#endif
