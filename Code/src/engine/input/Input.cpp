#include "Input.h"

#include <graphics/RenderCommand.h>
#include <graphics/Window.h>

namespace Greet
{
  Vec2 Input::mousePos;
  Vec2 Input::mouseLastPos;
  std::vector<bool> Input::mouseButtonsDown(16);
  std::vector<bool> Input::keyButtonsDown(256);
  float Input::mouseScrollX;
  float Input::mouseScrollY;

  void Input::SetMousePos(const Vec2& pos)
  {
    mousePos = pos;
  }

  void Input::SetKeyButton(int keyCode, bool value)
  {
    while(keyCode >= keyButtonsDown.size())
      keyButtonsDown.push_back(false);
    keyButtonsDown[keyCode] = value;
  }

  void Input::SetMouseButton(int mouseCode, bool value)
  {
    while(mouseCode >= mouseButtonsDown.size())
      mouseButtonsDown.push_back(false);
    mouseButtonsDown[mouseCode] = value;
  }

  void Input::AddMouseScroll(float scrollX, float scrollY)
  {
    mouseScrollX += scrollX;
    mouseScrollY += scrollY;
  }

  bool Input::IsKeyDown(int keyCode)
  {
    if(keyCode < keyButtonsDown.size())
      return keyButtonsDown[keyCode];
    return false;
  }

  bool Input::IsMouseDown(int mouseCode)
  {
    if(mouseCode < mouseButtonsDown.size())
      return mouseButtonsDown[mouseCode];
    return false;
  }

  Vec2 Input::GetMousePos(bool ignoreViewport)
  {
    if(ignoreViewport)
      return mousePos;
    return ConvertMousePosToViewport(mousePos);
  }

  Vec2 Input::GetMousePosDelta(bool ignoreViewport)
  {
    if(ignoreViewport)
      return mousePos - mouseLastPos;
    return ConvertMousePosToViewport(mousePos) - ConvertMousePosToViewport(mouseLastPos);
  }

  Vec2 Input::ConvertMousePosToViewport(const Vec2& pos)
  {
    const Vec4& viewport = RenderCommand::TopViewportStack();
    // Transform viewport to screen pos
    Vec2 pos1 {
      2 * viewport.x / Window::GetWidth() - 1.0f,
        2 * viewport.y / Window::GetHeight() - 1.0f};
    Vec2 pos2{
      2 * (viewport.x + viewport.z) / Window::GetWidth() - 1.0f,
        2 * (viewport.y + viewport.w) / Window::GetHeight() - 1.0f};

    return {
      (pos.x - pos1.x) * 2 / (pos2.x - pos1.x) - 1,
        (pos.y - pos1.y) * 2 / (pos2.y - pos1.y) - 1
    };
  }

  Vec2 Input::GetMousePosPixel(bool ignoreViewport)
  {
    Vec2 pos = GetMousePos(ignoreViewport);
    Window::TransformScreenToWindowPos(pos);
    return pos;
  }

  float Input::GetMouseScrollX()
  {
    return mouseScrollX;
  }

  float Input::GetMouseScrollY()
  {
    return mouseScrollY;
  }

  void Input::Update()
  {
    // Reset scroll amount
    mouseScrollX = 0;
    mouseScrollY = 0;
    mouseLastPos = mousePos;
  }
}
