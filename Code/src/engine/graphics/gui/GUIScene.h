#pragma once

#include <graphics/shaders/Shader.h>
#include <graphics/layers/Scene.h>
#include <graphics/renderers/GUIRenderer.h>
#include <event/Event.h>
#include <event/KeyEvent.h>
#include <event/MouseEvent.h>
#include <string>
#include <list>

namespace Greet {

  class Frame;
  class Component;

  class GUIScene : public Scene
  {
    private:
      Mat3 projectionMatrix;
      std::list<Frame*> frames;
      Component* focused;
      GUIRenderer* renderer;

    public:
      GUIScene(GUIRenderer* renderer);

      void OnEvent(Event& event);
      void OnMousePressEventHandler(MousePressEvent& event);
      void OnMouseMoveEventHandler(MouseMoveEvent& event);
      void OnMouseReleaseEventHandler(MouseReleaseEvent& event);
      void ViewportResize(ViewportResizeEvent& event);

      void PreRender();
      void Render() const;
      void PostRender();
      void Update(float timeElapsed);

      bool RequestFocus(Component* component);
      void AddFrame(Frame* frame);
      Frame* RemoveFrame(const std::string& name);
      Frame* RemoveFrame(Frame* frame);
      Frame* GetFrame(const std::string& name);

      float GetWidth();
      float GetHeight();

  };
}
