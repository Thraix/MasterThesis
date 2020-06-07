#include "Container.h"

#include <graphics/gui/GUIUtils.h>
#include <utils/Utils.h>
#include <utils/AABBUtils.h>
#include <graphics/gui/Component.h>
#include <cstdlib>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(Container);

  Container::Container()
    : Container(XMLObject(), nullptr)
  {

  }

  Container::Container(const std::string& name, Component* parent)
    : Component{name, parent}, vertical{true}, spacing{10}
  {

  }

  Container::Container(const XMLObject& object, Component* parent)
    : Component(object, parent), vertical(true)
  {
    vertical = GUIUtils::GetBooleanFromXML(object,"verticalAlign",true);
    spacing = GUIUtils::GetFloatFromXML(object, "spacing", 10);
    for (uint i = 0;i < object.GetObjectCount();i++)
    {
      Component* component = ComponentFactory::GetComponent(object.GetObject(i), this);
      if(StringUtils::StartsWith(component->GetName(),"Failed#"))
        Log::Error("Could not load Component, could not understand type. (type=",object.GetObject(i).GetName(),")");
      else
        AddComponent(component);
    }
  }

  Container::~Container()
  {
    for(auto it{m_components.begin()}; it != m_components.end();++it)
    {
      delete *it;
    }
  }

  void Container::PostConstruction()
  {
    for(auto&& comp : m_components)
    {
      comp->PostConstruction();
    }
  }

  void Container::Measure()
  {
    for(auto&& comp : m_components)
    {
      comp->Measure();
    }
    Component::Measure();
  }

  void Container::MeasureFill(float parentEmptyWidth, float parentEmptyHeight, float parentTotalWeight, bool vertical)
  {
    Component::MeasureFill(parentEmptyWidth, parentEmptyHeight, parentTotalWeight, vertical);

    Vec2 emptySpace = GetMeasureFillSize();

    for(auto&& comp : m_components)
    {
      comp->MeasureFill(emptySpace.w,emptySpace.h, GetMeasureTotalWeight(), this->vertical);
    }

    float offset = 0;
    for(auto&& comp : m_components)
    {
      if(this->vertical)
      {
        comp->SetPosition(Vec2(0.0f,offset));
        offset += comp->GetMargin().top + comp->GetSize().h + spacing;
      }
      else
      {
        comp->SetPosition(Vec2(offset,0.0f));
        offset += comp->GetMargin().left + comp->GetSize().w + spacing;
      }
    }
  }

  Vec2 Container::GetMeasureFillSize()
  {
    float usedSpace = 0;

    for(auto&& comp : m_components)
    {
      if(this->vertical)
        usedSpace += comp->GetMargin().GetHeight() + spacing;
      else
        usedSpace += comp->GetMargin().GetWidth() + spacing;

      if(this->vertical && comp->GetHeightSizeType() != ComponentSize::Type::WEIGHT)
        usedSpace += comp->GetSize().h;
      else if(!this->vertical && comp->GetWidthSizeType() != ComponentSize::Type::WEIGHT)
        usedSpace += comp->GetSize().w;
    }
    if(usedSpace > 0)
      usedSpace -= spacing; // Remove the spacing after the last one
    if(vertical)
    {
      float height = GetContentSize().h - usedSpace;
      if(height < 1)
        height = 1;
      return Vec2(GetContentSize().w, height);
    }
    else
    {
      float width = GetContentSize().w - usedSpace;
      if(width < 1)
        width = 1;
      return Vec2(width, GetContentSize().h);
    }
  }

  float Container::GetMeasureTotalWeight()
  {
    float totalWeight = 0;
    for(auto&& comp : m_components)
    {
      if(vertical && comp->GetHeightSizeType() == ComponentSize::Type::WEIGHT)
        totalWeight += comp->GetSizeValue().h;
      else if(!vertical && comp->GetWidthSizeType() == ComponentSize::Type::WEIGHT)
        totalWeight += comp->GetSizeValue().w;
    }
    return totalWeight;
  }

  void Container::RenderHandle(GUIRenderer* renderer) const
  {
    renderer->PushTranslation(pos);
    // Render rest of component
    Render(renderer);

    for(auto it{m_components.begin()}; it != m_components.end();++it)
    {
      (*it)->PreRender(renderer, (*it)->GetMargin().LeftTop() + GetTotalPadding());
      (*it)->RenderHandle(renderer);
      (*it)->PostRender(renderer);
    }
    renderer->PopTranslation();
  }

  void Container::UpdateHandle(float timeElapsed)
  {
    Update(timeElapsed);

    for(auto it{m_components.begin()}; it != m_components.end();++it)
    {
      (*it)->UpdateHandle(timeElapsed);
    }
  }

  void Container::AddComponent(Component* component)
  {
    if (component == nullptr)
    {
      Log::Warning("Cannot add NULL to component");
      return;
    }
    m_components.push_back(component);
  }

  Component* Container::RemoveComponent(uint index)
  {
    if (index >= m_components.size())
    {
      Log::Warning("Index out of bound.");
      return NULL;
    }
    auto it = m_components.begin() + index;
    m_components.erase(it);
    return *it;
  }

  Component* Container::RemoveComponent(Component* component)
  {
    for (auto it = m_components.begin(); it != m_components.end();++it)
    {
      if (*it == component)
      {
        m_components.erase(it);
        return component;
      }
    }
    Log::Warning("Couldn't find component");
    return NULL;
  }

  Component* Container::GetComponent(uint index)
  {
    if (index >= m_components.size())
    {
      Log::Warning("Index out of bound.");
      return NULL;
    }
    return *(m_components.begin() + index);
  }

  Component* Container::GetComponentByNameNoCast(const std::string& name)
  {
    Component* comp = Component::GetComponentByNameNoCast(name);
    if(comp)
      return comp;
    for(auto it{m_components.begin()}; it != m_components.end();++it)
    {
      comp = (*it)->GetComponentByNameNoCast(name);
      if(comp)
        return comp;
    }
    return nullptr;
  }

  size_t Container::GetComponentCount() const
  {
    return m_components.size();
  }

  Vec2 Container::GetWrapSize() const
  {
    Vec2 wrapSize{0,0};
    for(auto&& comp : m_components)
    {
      if(vertical)
      {
        if(wrapSize.w < comp->GetSize().w)
          wrapSize.w = comp->GetSize().w;
        wrapSize.h += comp->GetSize().h + comp->GetMargin().top + spacing;
      }
      else
      {
        if(wrapSize.h < comp->GetSize().h)
          wrapSize.h = comp->GetSize().h;
        wrapSize.w += comp->GetSize().w + comp->GetMargin().left + spacing + GetPadding().GetWidth();
      }
    }
    if(vertical && wrapSize.h > 0)
      wrapSize.h -= spacing;
    else if(!vertical && wrapSize.w > 0)
      wrapSize.w -= spacing;

    return wrapSize+GetBorder().GetSize() + GetPadding().GetSize();
  }

  bool Container::IsVertical() const
  {
    return vertical;
  }

  Container& Container::SetVertical(bool v)
  {
    vertical = v;
    Remeasure();
    return *this;
  }

  Container& Container::SetSpacing(float spacing)
  {
    this->spacing = spacing;
    Remeasure();
    return *this;
  }

  void Container::SetGUIScene(GUIScene* scene)
  {
    Component::SetGUIScene(scene);
    for(auto&& component : m_components)
      component->SetGUIScene(scene);
    guiScene = scene;
  }

  void Container::OnMousePressEventHandler(MousePressEvent& event, const Vec2& componentPos)
  {
    for(auto it = m_components.rbegin(); it != m_components.rend();++it)
    {
      Component* c{*it};
      if(c->IsMouseInside(event.GetPosition() - (componentPos + GetTotalPadding() + c->GetMargin().LeftTop() + c->GetPosition())))
      {
        c->OnEventHandler(event, componentPos + GetTotalPadding() + c->GetMargin().LeftTop() + c->GetPosition());
        return;
      }
    }
    Component::OnMousePressEventHandler(event, componentPos);
  }

  void Container::OnMouseMoveEventHandler(MouseMoveEvent& event, const Vec2& componentPos)
  {
    Component::OnMouseMoveEventHandler(event, componentPos);
    if(!UsingMouse())
    {
      for(auto it = m_components.rbegin(); it != m_components.rend();++it)
      {
        (*it)->OnEventHandler(event, componentPos + GetTotalPadding() + (*it)->GetMargin().LeftTop() + (*it)->GetPosition());
      }
    }
  }
}
