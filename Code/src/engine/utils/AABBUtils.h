#pragma once

#include <math/Maths.h>

namespace Greet {
  struct AABBUtils
  {
    static bool PointInsideBox(const Vec2& pointer, const Vec2& boxPos, const Vec2& boxSize)
    {
      return pointer.x >= boxPos.x && pointer.x < boxPos.x + boxSize.x && pointer.y >= boxPos.y && pointer.y < boxPos.y + boxSize.y;
    }
  };
}
