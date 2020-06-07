#pragma once

#include <graphics/gui/GUIUtils.h>
#include <logging/Log.h>
#include <math/Vec4.h>
#include <utils/xml/XMLObject.h>

#include <cstdlib>

namespace Greet
{
  struct TLBR
  {
    public:
      float top;
      float left;
      float bottom;
      float right;

      TLBR(float top, float left, float bottom, float right)
        : top(top), left(left), bottom(bottom),right(right)
      {}

      void Load(const std::string& prefix, const XMLObject& object)
      {
        if (object.HasAttribute(prefix))
        {
          std::vector<std::string> strings = StringUtils::SplitString(object.GetAttribute(prefix)," ");
          if(strings.size() == 4)
          {
            top = atof(strings.at(0).c_str());
            left = atof(strings.at(1).c_str());
            bottom = atof(strings.at(2).c_str());
            right = atof(strings.at(3).c_str());
          }
          else if(strings.size() == 2)
          {
            top = bottom = atof(strings.at(0).c_str());
            left = right = atof(strings.at(1).c_str());
          }
          else if(strings.size() == 1)
          {
            top = left = bottom = right = atof(strings.at(0).c_str());
          }
          else
          {
            Log::Error("TLBR:: Invalid parameter for ", prefix);
          }
        }
        else
        {
          top = GUIUtils::GetFloatFromXML(object, prefix+"Top", top);
          left = GUIUtils::GetFloatFromXML(object, prefix+"Left", left);
          bottom = GUIUtils::GetFloatFromXML(object, prefix+"Bottom", bottom);
          right = GUIUtils::GetFloatFromXML(object, prefix+"Right", right);
        }

      }

      friend std::ostream& operator<<(std::ostream& stream, const TLBR& tlbr)
      {
        return stream<< "(" << tlbr.top << ", " << tlbr.left << ", " << tlbr.bottom  << ", "<< tlbr.right << ")";
      }

      Vec2 LeftTop() const
      {
        return Vec2(left, top);
      }

      Vec2 RightBottom() const
      {
        return Vec2(right, bottom);
      }

      float GetWidth() const
      {
        return left + right;
      }

      float GetHeight() const
      {
        return top + bottom;
      }

      Vec2 GetSize() const
      {
        return Vec2(GetWidth(), GetHeight());
      }
  };
}
