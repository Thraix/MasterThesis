#pragma once

#include <Greet.h>

class Cam
{
  private:
    Greet::Vec3<float> position;
    Greet::Vec3<float> rotation;
    Greet::Mat4 viewMatrix;
    Greet::Mat4 projectionMatrix;
    Greet::Mat4 invPVMatrix;

  public:
    Cam(const Greet::Mat4& projectionMatrix)
      : position{0}, rotation{0}, viewMatrix{Greet::Mat4::Identity()}, projectionMatrix{projectionMatrix}, invPVMatrix{Greet::Mat4::Identity()}
    {
      RecalcViewMatrix();
    }

    void SetProjectionMatrix(const Greet::Mat4& _projectionMatrix)
    {
      projectionMatrix = _projectionMatrix;
      RecalcViewMatrix();
    }

    const Greet::Mat4& GetInvPVMatrix() const
    {
      return invPVMatrix;
    }

    const Greet::Mat4& GetViewMatrix() const
    {
      return viewMatrix;
    }

    const Greet::Mat4& GetProjectionMatrix() const
    {
      return projectionMatrix;
    }

    void SetPosition(const Greet::Vec3<float>& _position)
    {
      position = _position;
      RecalcViewMatrix();
    }

    const Greet::Vec3<float>& GetPosition() const
    {
      return position;
    }

    void SetRotation(const Greet::Vec3<float>& _rotation)
    {
      rotation = _rotation;
      RecalcViewMatrix();
    }

    const Greet::Vec3<float>& GetRotation() const
    {
      return rotation;
    }

    inline void RecalcViewMatrix()
    {
      viewMatrix = Greet::Mat4::RotateX(-rotation.x) * Greet::Mat4::RotateY(-rotation.y) * Greet::Mat4::Translate(-position);
      RecalcInvPVMatrix();
    }

    inline void RecalcInvPVMatrix()
    {
      invPVMatrix = ~(projectionMatrix * viewMatrix);
    }
};

class CamController
{
  private:
    Cam& cam;
    float moveSpeed = 5;

  public:
    CamController(Cam& cam)
      : cam{cam} {}

    void Update(float timeElapsed)
    {
      Greet::Vec3<float> rot = cam.GetRotation();
      Greet::Vec3<float> lastRot = rot;
      float rotationSpeed = 180 * timeElapsed;
      if (Greet::Input::IsKeyDown(GREET_KEY_UP))
        rot.x += rotationSpeed;
      if (Greet::Input::IsKeyDown(GREET_KEY_DOWN))
        rot.x -= rotationSpeed;
      if (Greet::Input::IsKeyDown(GREET_KEY_LEFT))
        rot.y += rotationSpeed;
      if (Greet::Input::IsKeyDown(GREET_KEY_RIGHT))
        rot.y -= rotationSpeed;

      Greet::Vec2 posDelta{0};
      float zDelta = 0;
      float moveDist = moveSpeed * timeElapsed;
      if (Greet::Input::IsKeyDown(GREET_KEY_W))
        posDelta.y -= moveDist;
      if (Greet::Input::IsKeyDown(GREET_KEY_S))
        posDelta.y += moveDist;
      if (Greet::Input::IsKeyDown(GREET_KEY_A))
        posDelta.x -= moveDist;
      if (Greet::Input::IsKeyDown(GREET_KEY_D))
        posDelta.x += moveDist;
      if (Greet::Input::IsKeyDown(GREET_KEY_LEFT_SHIFT))
        zDelta -= moveDist;
      if (Greet::Input::IsKeyDown(GREET_KEY_SPACE))
        zDelta += moveDist;

      posDelta.Rotate(-rot.y);

      if(posDelta != Greet::Vec2{0} || zDelta != 0)
        cam.SetPosition(cam.GetPosition() + Greet::Vec3<float>{posDelta.x, zDelta, posDelta.y});
      if(rot != lastRot)
        cam.SetRotation(rot);
    }

    void SetMoveSpeed(float _moveSpeed)
    {
      moveSpeed = _moveSpeed;
    }

    float GetMoveSpeed() const
    {
      return moveSpeed;
    }
};
