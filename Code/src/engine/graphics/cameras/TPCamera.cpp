#include "TPCamera.h"

#include <graphics/Window.h>
#include <event/MouseEvent.h>
#include <input/InputDefines.h>

namespace Greet {

  TPCamera::TPCamera(float fov, float near, float far)
    : TPCamera(fov, near, far, Vec3<float>(0,0,0), 1, 0, 0)
  {
  }

  TPCamera::TPCamera(float fov, float near, float far, const Vec3<float>& position, float distance, float height, float rotation)
    : TPCamera(fov, near, far, position, distance, height, rotation, 1, 100, -1,1)
  {

  }

  TPCamera::TPCamera(float fov, float near, float far, const Vec3<float>& position, float distance, float height, float rotation, float distanceMin, float distanceMax, float heightMin, float heightMax)
    : TPCamera(fov, near, far, position, distance, height, rotation, distanceMin, distanceMax, heightMin, heightMax, 1000, 10, 10)
  {
  }

  TPCamera::TPCamera(float fov, float near, float far, const Vec3<float>& position, float distance, float height, float rotation, float distanceMin, float distanceMax, float heightMin, float heightMax, float rotationSpeed, float heightSpeed, float distanceSpeed)
    : Camera(fov, near, far), m_position(position), m_distance(distance), m_height(height), m_rotation(rotation), m_rotationSpeed(rotationSpeed), m_heightSpeed(heightSpeed), m_distanceSpeed(distanceSpeed)
  {
    m_rotationWanted = m_rotation;
    m_distanceWanted = m_distance;
    m_heightWanted = m_height;
    m_positionWanted = m_position;
    SetDistanceClamp(distanceMin, distanceMax);
    SetHeightClamp(heightMin, heightMax);
    CalculateInformation();
  }

  TPCamera::~TPCamera()
  {
  }

  void TPCamera::Update(float timeElapsed)
  {
    float factor1 = (timeElapsed * 10.0f); // 0.33 ish
    float factor2 = (timeElapsed * 30.0f);
    m_rotation += (m_rotationWanted - m_rotation) * factor1;
    m_position += (m_positionWanted - m_position) * factor2;
    m_distance += (m_distanceWanted - m_distance) * factor1;
    m_height += (m_heightWanted - m_height) * factor2;
    CalculateInformation();
  }

  void TPCamera::SetPosition(const Vec3<float>& pos)
  {
    m_positionWanted = pos;
  }

  void TPCamera::SetHeight(float height)
  {
    m_heightWanted = height;
    Math::Clamp(&m_heightWanted, m_heightMin, m_heightMax);
    if (m_heightWanted != height)
    {
      Log::Warning("Height outside of clamp, clamping.");
    }
  }

  void TPCamera::SetRotation(float rotation)
  {
    //m_rotation = rotation;
    m_rotationWanted = rotation; // smooth transition
  }

  void TPCamera::SetDistance(float distance)
  {
    m_distanceWanted = distance;
    Math::Clamp(&m_distanceWanted, m_distanceMin, m_distanceMax);
    if (m_distanceWanted != distance)
    {
      Log::Warning("Distance outside of clamp, clamping.");
    }
  }

  void TPCamera::SetDistanceClamp(float min, float max)
  {
    if (min > max)
    {
      Log::Error("Distance clamp: min greater than max.");
      return;
    }
    m_distanceMin = min < 0 ? 0 : min;
    m_distanceMax = max < 0 ? 0 : max;
    if (m_distanceWanted < m_distanceMin)
    {
      Log::Warning("Distance outside of clamp, reclamping.");
      m_distanceWanted = m_distanceMin;
    }
    else if (m_distanceWanted > m_distanceMax)
    {
      Log::Warning("Distance outside of clamp, reclamping.");
      m_distanceWanted = m_distanceMax;
    }
  }

  void TPCamera::SetHeightClamp(float min, float max)
  {
    if (min > max)
    {
      Log::Error("Height clamp: min greater than max.");
      return;
    }
    Math::Clamp(&min, -1.0f, 1.0f);
    Math::Clamp(&max, -1.0f, 1.0f);
    m_heightMin = min;
    m_heightMax = max;
    if (m_heightWanted < m_heightMin)
    {
      Log::Warning("Height outside of clamp, reclamping.");
      m_heightWanted = m_heightMin;
    }
    else if (m_heightWanted > m_heightMax)
    {
      Log::Warning("Height outside of clamp, reclamping.");
      m_heightWanted = m_heightMax;
    }
  }

  void TPCamera::CalculateViewMatrix()
  {
    viewMatrix = Mat4::TPCamera(m_position, m_distance, m_height, m_rotation);
  }

  void TPCamera::CalculateInformation()
  {
    CalculateViewMatrix();
  }

  void TPCamera::Move(const Vec2& delta) {
    if (m_mouse3) {
      Rotate(delta);
    }
    if (m_mouse2)
    {
      m_positionWanted.y += delta.y * 0.1f;
    }
    else if (m_mouse1)
    {
      Vec2 deltaR = Vec2(delta).Rotate(-m_rotation);
      m_positionWanted.x -= deltaR.y * m_distanceSpeed * m_distance /RenderCommand::GetViewportAspect();
      m_positionWanted.z -= deltaR.x * m_distanceSpeed * m_distance;
    }
  }

  void TPCamera::Rotate(const Vec2& delta)
  {
    m_heightWanted -= delta.y * m_heightSpeed;
    Math::Clamp(&m_heightWanted, m_heightMin, m_heightMax);
    m_rotationWanted += m_rotationSpeed * delta.x;
  }

  void TPCamera::Zoom(float delta)
  {
    m_distanceWanted -= delta;
    Math::Clamp(&m_distanceWanted, m_distanceMin, m_distanceMax);
  }


  void TPCamera::OnEvent(Event& event)
  {
    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent mEvent = (MousePressEvent&)event;
      if(mEvent.GetButton() == GREET_MOUSE_1)
        m_mouse1 = true;
      else if(mEvent.GetButton() == GREET_MOUSE_2)
        m_mouse2 = true;
      else if(mEvent.GetButton() == GREET_MOUSE_3)
        m_mouse3 = true;
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_RELEASE))
    {
      MousePressEvent mEvent = (MousePressEvent&)event;
      if(mEvent.GetButton() == GREET_MOUSE_1)
        m_mouse1 = false;
      else if(mEvent.GetButton() == GREET_MOUSE_2)
        m_mouse2 = false;
      else if(mEvent.GetButton() == GREET_MOUSE_3)
        m_mouse3 = false;
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      MouseMoveEvent&  mEvent = (MouseMoveEvent&)event;
      Move(Vec2(mEvent.GetDX(), mEvent.GetDY()));
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_SCROLL))
    {
      MouseScrollEvent&  mEvent = (MouseScrollEvent&)event;
      Zoom(mEvent.GetScrollVertical());
    }
    if(m_mouse1 | m_mouse2 | m_mouse3)
      event.AddFlag(EVENT_HANDLED | EVENT_FOCUSED);
    else
      event.AddFlag(EVENT_UNFOCUSED);
  }

}
