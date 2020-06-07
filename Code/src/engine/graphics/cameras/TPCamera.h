#pragma once

#include <graphics/cameras/Camera.h>
#include <math/Maths.h>
#include <event/EventDispatcher.h>

namespace Greet {

  class TPCamera : public Camera
  {
    private:
      bool m_mouse1 = false;
      bool m_mouse2 = false;
      bool m_mouse3 = false;

      // Clamps to the camera position
      float m_distanceMin;
      float m_distanceMax;
      float m_heightMin;
      float m_heightMax;

      // Speed of the camera
      float m_rotationSpeed;
      float m_heightSpeed;
      float m_distanceSpeed;

      Vec3<float> m_position;
      float m_distance;
      float m_height;
      float m_rotation;

      Vec3<float> m_positionWanted;
      float m_distanceWanted;
      float m_heightWanted;
      float m_rotationWanted;

    private:
      void CalculateViewMatrix();
      void CalculateInformation();
    public:
      TPCamera(float fov, float near, float far);
      TPCamera(float fov, float near, float far, const Vec3<float>& position, float distance, float height, float rotation);
      TPCamera(float fov, float near, float far, const Vec3<float>& position, float distance, float height, float rotation, float distanceMin, float distanceMax, float heightMin, float heightMax);
      TPCamera(float fov, float near, float far, const Vec3<float>& position, float distance, float height, float rotation, float distanceMin, float distanceMax, float heightMin, float heightMax, float rotationSpeed, float heightSpeed, float distanceSpeed);
      virtual ~TPCamera();

      void Update(float timeElapsed) override;

      const Vec3<float>& GetPosition() const { return m_position; }
      float GetHeight() const { return m_height; }
      float GetRotation() const { return m_rotation; }
      float GetDistance() const { return m_distance; }
      float GetMinDistance() const { return m_distanceMin; }
      float GetMaxDistance() const { return m_distanceMax; }
      float GetMinHeight() const { return m_heightMin; }
      float GetMaxHeight() const { return m_heightMax; }


      void SetPosition(const Vec3<float>& pos);
      void SetHeight(float height);
      void SetRotation(float rotation);
      void SetDistance(float distance);
      void SetDistanceClamp(float min, float max);
      void SetHeightClamp(float min, float max);

      void SetInputRotation(const std::string& inputName);
      void SetInputZoom(const std::string& inputName);
      void SetInputXZ(const std::string& inputName);
      void SetInputY(const std::string& inputName);
      void SetInputMove(const std::string& inputName);

      void Move(const Vec2& delta);
      void Rotate(const Vec2& delta);
      void Zoom(float delta);
      void OnEvent(Event& event) override;
  };

}
