#pragma once

#include <graphics/Window.h>
#include <utils/StringUtils.h>
#include <utils/ColorUtils.h>
#include <utils/xml/XML.h>

namespace Greet {

  struct ComponentSize
  {
    enum class Type
    {
      WRAP, WEIGHT, PIXELS
    };

    // Can become dirty if below values changes
    Vec2 size;

    Vec2 value = {1,1};
    Type heightType = Type::WRAP;
    Type widthType = Type::WRAP;
  };

  class GUIUtils
  {
    public:

      static bool GetBooleanFromXML(const XMLObject& object, const std::string& key, bool defaultValue);
      static Vec4 GetColorFromXML(const XMLObject& object, const std::string& key, const Vec4& defaultValue);
      static ComponentSize GetComponentSizeFromXML(const XMLObject& object, const std::string& widthKey, const std::string& heightKey, ComponentSize defaultValue);
      static std::string GetStringFromXML(const XMLObject& object, const std::string& key, const std::string& defaultValue);
      static int GetIntFromXML(const XMLObject& object, const std::string& key, int defaultValue);
      static float GetFloatFromXML(const XMLObject& object, const std::string& key, float defaultValue);

    private:
      static bool GetBoolean(const std::string& str);
      static void GetComponentSize(const std::string& size, float* retValue, ComponentSize::Type* retType);
    public:
      static Vec4 GetColor(const std::string& str);
  };
}
