#include "GUIScene.h"

#include <event/EventDispatcher.h>
#include <graphics/RenderCommand.h>
#include <graphics/gui/Frame.h>
#include <graphics/gui/GUIUtils.h>
#include <graphics/shaders/ShaderFactory.h>

namespace Greet {

  GUIScene::GUIScene(GUIRenderer* renderer)
    : renderer(renderer), projectionMatrix(Mat3::OrthographicViewport())
  {
    focused = nullptr;

    renderer->GetShader()->Enable();
    renderer->GetShader()->SetUniformMat3("projectionMatrix", projectionMatrix);
    renderer->GetShader()->Disable();
}

  void GUIScene::OnEvent(Event& event)
  {
    Scene::OnEvent(event);
    if(EVENT_IS_CATEGORY(event, INPUT_EVENT))
    {
      if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
        OnMousePressEventHandler((MousePressEvent&)event);
      else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
        OnMouseMoveEventHandler((MouseMoveEvent&)event);
      else if(EVENT_IS_TYPE(event, EventType::MOUSE_RELEASE))
        OnMouseReleaseEventHandler((MouseReleaseEvent&)event);
      else if(focused)
      {
        focused->OnEventHandler(event, focused->GetPosition());
        event.AddFlag(EVENT_HANDLED);
      }
    }
  }

  void GUIScene::OnMousePressEventHandler(MousePressEvent& event)
  {
    Vec2 mousePos = ~projectionMatrix * event.GetPosition();
    MousePressEvent transformedEvent{mousePos, event.GetButton()};

    for (auto it = frames.rbegin(); it != frames.rend(); ++it)
    {
      if((*it)->IsMouseInside(transformedEvent.GetPosition() - (*it)->GetMargin().LeftTop() - (*it)->GetPosition()))
      {
        (*it)->OnEventHandler(transformedEvent, (*it)->GetPosition());
        frames.splice(frames.end(), frames, std::next(it).base());
        event.AddFlag(EVENT_HANDLED | transformedEvent.GetFlags());
        return;
      }
    }
    RequestFocus(nullptr);
    event.AddFlag(EVENT_UNFOCUSED);
  }

  void GUIScene::OnMouseMoveEventHandler(MouseMoveEvent& event)
  {
    Vec2 mousePos = ~projectionMatrix * event.GetPosition();
    Vec2 mouseDelta  = ~projectionMatrix * (event.GetPosition() + event.GetDeltaPosition()) - mousePos;
    MouseMoveEvent transformedEvent{mousePos, mouseDelta};

    if (focused && focused->UsingMouse())
    {
      focused->OnEventHandler(transformedEvent, focused->GetRealPosition());
      event.AddFlag(EVENT_HANDLED | transformedEvent.GetFlags());
      return;
    }

    for (auto it = frames.rbegin(); it != frames.rend(); ++it)
    {
      (*it)->OnEventHandler(transformedEvent, (*it)->GetPosition());
    }
  }

  void GUIScene::OnMouseReleaseEventHandler(MouseReleaseEvent& event)
  {
    Vec2 mousePos = ~projectionMatrix * event.GetPosition();
    MouseReleaseEvent transformedEvent{mousePos, event.GetButton()};
    if (focused)
    {
      focused->OnEventHandler(transformedEvent, focused->GetRealPosition());
      event.AddFlag(EVENT_HANDLED | transformedEvent.GetFlags());
    }
  }

  void GUIScene::ViewportResize(ViewportResizeEvent& event)
  {
    projectionMatrix = Mat3::OrthographicViewport();
    renderer->GetShader()->Enable();
    renderer->GetShader()->SetUniformMat3("projectionMatrix", projectionMatrix);
    renderer->GetShader()->Disable();
    for (auto it = frames.begin(); it != frames.end(); ++it)
    {
      (*it)->OnViewportResize(event.GetWidth(), event.GetHeight());
    }
  }

  void GUIScene::PreRender()
  {
    renderer->Begin();
  }

  void GUIScene::Render() const
  {
    for (auto it = frames.begin(); it != frames.end(); ++it)
    {
      (*it)->PreRender(renderer, Vec2(0,0));
      (*it)->RenderHandle(renderer);
      (*it)->PostRender(renderer);
    }
  }

  void GUIScene::PostRender()
  {
    renderer->End();
    renderer->Draw();
  }

  void GUIScene::Update(float timeElapsed)
  {
    for (auto it = frames.begin(); it != frames.end(); ++it)
    {
      (*it)->UpdateHandle(timeElapsed);
    }
  }

  // Could do checks if a popup is refusing to give request or something
  bool GUIScene::RequestFocus(Component* component)
  {
    // Unfocus the currently focused component
    if(focused)
      focused->OnUnfocused();

    // Focus the requested one
    if(component && component != focused)
      component->OnFocused();

    // TODO: Maybe clean this code up.
    Component* unfocused = focused;
    Component* tmp = component;

    uint unfocusedDepth = unfocused ? unfocused->GetComponentDepth() : 0;
    uint focusedDepth = tmp ? tmp->GetComponentDepth() : 0;

    while(unfocusedDepth != focusedDepth)
    {
      if(unfocusedDepth > focusedDepth)
      {
        unfocused->ChildChangedFocus(false);
        unfocused = unfocused->GetParent();
        --unfocusedDepth;
      }
      else
      {
        tmp->ChildChangedFocus(true);
        tmp = tmp->GetParent();
        --focusedDepth;
      }
    }

    while(unfocused != tmp)
    {
      unfocused->ChildChangedFocus(false);
      unfocused = unfocused->GetParent();
      tmp->ChildChangedFocus(true);
      tmp = tmp->GetParent();
    }

    focused = component;

    return true;
  }

  void GUIScene::AddFrame(Frame* frame)
  {
    if (frame == nullptr)
    {
      Log::Warning("Cannot add nullptr to frames");

      return;
    }
    frame->Measure();
    frame->MeasureFill(GetWidth(),GetHeight(), 1, true);
    frame->SetGUIScene(this);
    frames.push_back(frame);
    frame->PostConstruction();
  }

  Frame* GUIScene::RemoveFrame(const std::string& name)
  {
    for(auto it = frames.begin(); it!=frames.end();++it)
    {
      if((*it)->GetName() == name)
      {
        frames.erase(it);
        return (*it);
      }
    }
    return nullptr;
  }

  Frame* GUIScene::RemoveFrame(Frame* frame)
  {
    for (auto it = frames.begin(); it != frames.end();++it)
    {
      if ((*it) == frame)
      {
        frames.erase(it);
        return frame;
      }
    }
    return nullptr;
  }

  Frame* GUIScene::GetFrame(const std::string& name)
  {
    for(auto it = frames.begin(); it!=frames.end();++it)
    {
      if((*it)->GetName() == name)
      {
        return (*it);
      }
    }
    return nullptr;
  }
  float GUIScene::GetWidth()
  {
    return RenderCommand::GetViewportWidth();
  }
  float GUIScene::GetHeight()
  {
    return RenderCommand::GetViewportHeight();
  }
}
