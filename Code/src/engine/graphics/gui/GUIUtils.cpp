#include "GUIUtils.h"

#include <utils/xml/XMLException.h>
#include <logging/Log.h>
#include <utils/LogUtils.h>

namespace Greet
{
  bool GUIUtils::GetBooleanFromXML(const XMLObject& object, const std::string& key, bool defaultValue)
  {
    if(object.HasAttribute(key))
      return GetBoolean(object.GetAttribute(key));
    return defaultValue;
  }

  Vec4 GUIUtils::GetColorFromXML(const XMLObject& object, const std::string& key, const Vec4& defaultValue)
  {
    if(object.HasAttribute(key))
      return GetColor(object.GetAttribute(key));
    return defaultValue;
  }

  ComponentSize GUIUtils::GetComponentSizeFromXML(const XMLObject& object, const std::string& widthKey, const std::string& heightKey, ComponentSize defaultValue)
  {
    ComponentSize ret = defaultValue;
    if(object.HasAttribute(widthKey))
      GetComponentSize(object.GetAttribute(widthKey), &ret.value.w, &ret.widthType);
    if(object.HasAttribute(heightKey))
      GetComponentSize(object.GetAttribute(heightKey), &ret.value.h, &ret.heightType);
    return ret;
  }

  std::string GUIUtils::GetStringFromXML(const XMLObject& object, const std::string& key, const std::string& defaultValue)
  {
    if(object.HasAttribute(key))
      return object.GetAttribute(key);
    return defaultValue;
  }

  int GUIUtils::GetIntFromXML(const XMLObject& object, const std::string& key, int defaultValue)
  {
    if(object.HasAttribute(key))
    {
      const std::string& str = object.GetAttribute(key);
      char* endPos;
      int value = std::strtol(str.c_str(), &endPos, 10);
      if(endPos != &*str.end())
        Log::Warning("Invalid string in int property \"",key,"\"=\"", str, "\"");
      return value;
    }
    return defaultValue;
  }

  float GUIUtils::GetFloatFromXML(const XMLObject& object, const std::string& key, float defaultValue)
  {
    if(object.HasAttribute(key))
    {
      const std::string& str = object.GetAttribute(key);
      char* endPos;
      float value = std::strtof(str.c_str(), &endPos);
      if(endPos != &*str.end())
        Log::Warning("Invalid string in float property \"",key,"\"=\"", str, "\"");
      return value;
    }
    return defaultValue;
  }


  bool GUIUtils::GetBoolean(const std::string& str)
  {
    if (str == "true")
      return true;
    else if (str != "false")
      Log::Warning("Invalid string in boolean property \"", str, "\"");
    return false;
  }

  Vec4 GUIUtils::GetColor(const std::string& str)
  {
    if (str[0] == '#')
    {
      std::string color = str.substr(1);
      if (color.length() != 6 && color.length() != 8)
      {
        Log::Error("Invalid length for color: ", str);
        return Vec4(1, 0, 1, 1); // Invalid color pink since its very visible
      }
      if (color.length() == 6)
        color = "FF" + color; // Add alpha to color
      uint colori = LogUtils::HexToDec(color);
      return ColorUtils::ColorHexToVec4(colori);
    }
    Log::Error("Invalid starting character for color: ", str);
    return Vec4(1, 0, 1, 1); // Invalid color pink since its very visible
  }

  void GUIUtils::GetComponentSize(const std::string& size, float* retValue, ComponentSize::Type* retType)
  {
    char* endPos;
    *retValue = std::strtof(size.c_str(), &endPos);
    if(size == "fill_parent")
    {
      *retType = ComponentSize::Type::WEIGHT;
      *retValue = 1;
    }
    else if(size == "wrap_content")
      *retType = ComponentSize::Type::WRAP;
    else if(*endPos == 'w')
      *retType = ComponentSize::Type::WEIGHT;
    else
    {
      if(endPos != &*size.end())
        Log::Warning("Invalid string in size property \"", size, "\"");
      *retType = ComponentSize::Type::PIXELS;
    }
  }
}
