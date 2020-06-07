#include "Frame.h"

#include <graphics/gui/GUIUtils.h>
#include <input/InputDefines.h>
#include <utils/Utils.h>
#include <utils/AABBUtils.h>

namespace Greet {

  REGISTER_FRAME_DEFINITION(Frame);

  byte Frame::RESIZING_LEFT = BIT(0);
  byte Frame::RESIZING_RIGHT = BIT(1);
  byte Frame::RESIZING_TOP = BIT(2);
  byte Frame::RESIZING_BOTTOM = BIT(3);
  uint Frame::RESIZING_MARGIN = 5;

  Frame::Frame()
    : Frame(XMLObject())
  {

  }

  // These are usually the top element so no parent is needed
  Frame::Frame(const XMLObject& object)
    : Container(object, nullptr), shouldCloseUnfocus{false}, m_stayInsideWindow{true}
  {
    m_resizableFlags = RESIZING_LEFT | RESIZING_RIGHT | RESIZING_TOP | RESIZING_BOTTOM;
    m_resizing = 0;
    minSize = Vec2(100, 100);
    m_isFocusable = true;

    minSize.w = GUIUtils::GetFloatFromXML(object,"minWidth", 100);
    minSize.h = GUIUtils::GetFloatFromXML(object,"minHeight", 100);

    // This can still look better
    if(!GUIUtils::GetBooleanFromXML(object,"resizeLeft", false))
      m_resizableFlags &= ~RESIZING_LEFT;
    if(!GUIUtils::GetBooleanFromXML(object,"resizeRight", false))
      m_resizableFlags &= ~RESIZING_RIGHT;
    if(!GUIUtils::GetBooleanFromXML(object,"resizeTop", false))
      m_resizableFlags &= ~RESIZING_TOP;
    if(!GUIUtils::GetBooleanFromXML(object,"resizeBottom", false))
      m_resizableFlags &= ~RESIZING_BOTTOM;
  }

  Frame::~Frame()
  {
  }

  bool Frame::CheckResize(const Vec2& mousePos)
  {
    m_resizingFlags = 0;
    if ((m_resizableFlags & RESIZING_LEFT) != 0 && mousePos.x >= pos.x - RESIZING_MARGIN && mousePos.x < pos.x + RESIZING_MARGIN)
    {
      m_resizingFlags |= RESIZING_LEFT;
    }
    else if ((m_resizableFlags & RESIZING_RIGHT) != 0 && mousePos.x >= pos.x + size.size.w - RESIZING_MARGIN && mousePos.x < pos.x + size.size.w + RESIZING_MARGIN)
    {
      m_resizingFlags |= RESIZING_RIGHT;
    }
    if ((m_resizableFlags & RESIZING_TOP) != 0 && mousePos.y >= pos.y - RESIZING_MARGIN && mousePos.y < pos.y + RESIZING_MARGIN)
    {
      m_resizingFlags |= RESIZING_TOP;
    }
    else if ((m_resizableFlags & RESIZING_BOTTOM) != 0 && mousePos.y >= pos.y + size.size.h - RESIZING_MARGIN && mousePos.y < pos.y + size.size.h + RESIZING_MARGIN)
    {
      m_resizingFlags |= RESIZING_BOTTOM;
    }
    m_resizing = m_resizingFlags != 0;
    return m_resizing;
  }

  void Frame::Resize(const Vec2& mousePos)
  {
    Vec2 oldSize = size.value;
    Vec2 diff = m_posOrigin - (m_clickPos - mousePos);
    if (m_resizingFlags & RESIZING_LEFT)
    {
      pos.x = m_posOrigin.x - (m_clickPos.x - mousePos.x);
      size.value.w = m_sizeOrigin.x + (m_clickPos.x - mousePos.x);
      if (size.value.w < minSize.w)
      {
        pos.x = m_posOrigin.x + (m_sizeOrigin.x - minSize.w);
        size.value.w = minSize.w;
      }
    }
    else if (m_resizingFlags & RESIZING_RIGHT)
    {
      size.value.w = m_sizeOrigin.x - (m_clickPos.x - mousePos.x);
      if (size.value.w < minSize.w)
        size.value.w = minSize.w;
    }
    if (m_resizingFlags & RESIZING_TOP)
    {
      pos.y = m_posOrigin.y - (m_clickPos.y - mousePos.y);
      size.value.h = m_sizeOrigin.y + (m_clickPos.y - mousePos.y);
      if (size.value.h < minSize.h)
      {
        pos.y = m_posOrigin.y + (m_sizeOrigin.y - minSize.h);
        size.value.h = minSize.h;
      }
    }
    else if (m_resizingFlags & RESIZING_BOTTOM)
    {
      size.value.h = m_sizeOrigin.y - (m_clickPos.y - mousePos.y);
      if (size.value.h < minSize.h)
        size.value.h = minSize.h;
    }
    ResizeScreenClamp();
    if(oldSize != size.value)
    {
      Remeasure();
    }
  }

  void Frame::ResizeScreenClamp()
  {
    if (m_stayInsideWindow)
    {
      if (pos.x < 0)
      {
        pos.x = 0;
        size.size.w = m_posOrigin.x + m_sizeOrigin.x;
      }
      else if (pos.x > guiScene->GetWidth() - size.size.w)
      {
        size.size.w = guiScene->GetWidth() - m_posOrigin.x;
      }
      if (pos.y < 0)
      {
        pos.y = 0;
        size.size.h = m_posOrigin.y + m_sizeOrigin.y;
      }
      else if (pos.y > guiScene->GetHeight() - size.size.h)
        size.size.h = guiScene->GetHeight() - m_posOrigin.y;
    }
  }

  void Frame::OnEvent(Event& event, const Vec2& translatedPos)
  {
    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      if (e.GetButton() == GREET_MOUSE_1)
      {
        m_posOrigin = pos;
        m_sizeOrigin = size.size;
        m_clickPos = translatedPos;
        CheckResize(translatedPos);
      }
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_RELEASE))
    {
      MouseReleaseEvent& e = static_cast<MouseReleaseEvent&>(event);
      if (e.GetButton() == GREET_MOUSE_1)
        m_resizing = false;
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      if (m_resizing)
        Resize(translatedPos);
    }
  }

  void Frame::SetGUIMouseListener(GUIMouseListener* listener)
  {
    m_mouseListener = listener;
  }

  void Frame::OnViewportResize(int width, int height)
  {
    Remeasure();
  }

  bool Frame::IsMouseInside(const Vec2& mousePos) const
  {
    Vec2 resizeMargin = Vec2(RESIZING_MARGIN, RESIZING_MARGIN);
    return AABBUtils::PointInsideBox(mousePos, -resizeMargin, GetSize() + resizeMargin*2);
  }

  void Frame::SetPosition(const Vec2& pos)
  {
    if(m_stayInsideWindow)
    {
      Vec2 p = pos;
      if(p.x + size.size.w > guiScene->GetWidth())
      {
        p.x = guiScene->GetWidth() - size.size.w;
      }
      if(p.y + size.size.h > guiScene->GetHeight())
      {
        p.y = guiScene->GetHeight() - size.size.h;
      }
      Component::SetPosition(p);
      return;
    }
    Component::SetPosition(pos);
  }

  void Frame::ChildChangedFocus(bool focused)
  {
    if(!GetParent() && !focused && shouldCloseUnfocus)
    {
      guiScene->RemoveFrame(name);
    }
  }
}
