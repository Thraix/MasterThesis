#pragma once

#include <event/Event.h>
#include <event/ViewportEvent.h>
#include <event/WindowEvent.h>
#include <graphics/RenderCommand.h>
#include <graphics/Window.h>
#include <logging/Log.h>
#include <math/Maths.h>

namespace Greet {
  class Camera {

    protected:
      float fov;
      float near;
      float far;
      Mat4 projectionMatrix;
      Mat4 viewMatrix;

    public:
      Camera(float fov, float near, float far) :
        projectionMatrix{Mat4::ProjectionMatrix(RenderCommand::GetViewportAspect(), fov, near, far)},
        fov{fov}, near{near}, far{far}
      {}
      virtual const Mat4& GetViewMatrix() const { return viewMatrix;}
      virtual const Mat4& GetProjectionMatrix() const { return projectionMatrix;}
      virtual void SetProjectionMatrix(const Mat4& projectionMatrix)
      {
        Camera::projectionMatrix = projectionMatrix;
      }
      virtual void Update(float timeElapsed) {};
      virtual void ViewportResize(ViewportResizeEvent& event)
      {
        projectionMatrix = Mat4::ProjectionMatrix(event.GetWidth() / event.GetHeight(), fov, near, far);
      }
      virtual void OnEvent(Event& event) {};

      Vec3<float> GetWorldToScreenCoordinate(const Vec3<float>& coordinate) const
      {
        return projectionMatrix * (viewMatrix * coordinate);
      }

      void GetScreenToWorldCoordinate(const Vec2& screenPos, Vec3<float>* near, Vec3<float>* direction) const
      {
        if (near == NULL)
          return Log::Error("Near vector is NULL");
        if (direction == NULL)
          return Log::Error("Direction vector is NULL");

        Mat4 pvInv = ~(projectionMatrix * viewMatrix);
        *near = pvInv * Vec3<float>(screenPos.x, screenPos.y, -1.0);
        Vec3<float> far = pvInv * Vec3<float>(screenPos.x, screenPos.y, 1.0);
        *direction = (far - *near).Normalize();
      }
  };
}
