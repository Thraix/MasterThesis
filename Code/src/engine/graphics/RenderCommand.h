#pragma once

#include <stack>
#include <math/Vec2.h>
#include <math/Vec4.h>

namespace Greet
{
  enum class CullFaceDirection
  {
    CW, CCW
  };

  class RenderCommand
  {

    private:
      RenderCommand() = delete;
      static std::stack<Vec4> viewportStack;
      static Vec4 clearColor;
    public:
      static void Init();
      /*
         Values relative to the current viewport
         position is from the top-left of the screen
         */
      static void PushViewportStack(float x, float y, float width, float height, bool ignoreParent=false);
      static void PushViewportStack(const Vec2& pos, const Vec2& size, bool ignoreParent=false);
      static void PushViewportStack(const Vec4& viewport, bool ignoreParent=false);
      static void PushViewportDefaultStack();

      static void PopViewportStack();
      static Vec4 TopViewportStack();
      static void ResetViewport();

      static float GetViewportWidth();
      static float GetViewportHeight();
      static float GetViewportAspect();

      static void SetClearColor(const Vec4& clearColor);
      static const Vec4& GetClearColor();
      static void Clear();

      static void EnableCulling(bool enabled);
      static void SetCullFace(CullFaceDirection face);
      static void ResetCulling();

      static void EnableDepthTest(bool enabled);
      static void ResetDepthTest();
  };
}

