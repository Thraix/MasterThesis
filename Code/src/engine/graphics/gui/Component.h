#pragma once

#include <graphics/gui/Style.h>
#include <graphics/renderers/GUIRenderer.h>
#include <event/MouseEvent.h>
#include <event/KeyEvent.h>
#include <graphics/gui/ComponentFactory.h>
#include <graphics/gui/GUIScene.h>

namespace Greet
{
  class Component
  {
    public:
      friend class GUIScene;
    private:
      REGISTER_COMPONENT_DECLARATION(Component);
    public:

      // Typedef callbacks
      typedef std::function<void(Component* c)> OnClickCallback;
      typedef std::function<void(Component* c)> OnPressCallback;
      typedef std::function<void(Component* c)> OnReleaseCallback;

    protected:
      Style* currentStyle;

      Style normalStyle;
      Style hoverStyle;
      Style pressStyle;

      std::string name;

      GUIScene* guiScene;
      Component* parent;

      Vec2 pos;
      ComponentSize size;

      bool m_isFocusable;
      bool isFocused;
      bool isHovered;
      bool pressed;

      // Callbacks
      OnClickCallback onClickCallback;
      OnPressCallback onPressCallback;
      OnReleaseCallback onReleaseCallback;

    public:
      Component(const std::string& name, Component* parent);
      Component(const XMLObject& object, Component* parent);

      virtual void PostConstruction(){}

      // Measurements that doesn't depend on the parent
      virtual void Measure();
      // Measurements that does depend on the parent
      void MeasureFill();
      virtual void MeasureFill(float parentEmptyWidth, float parentEmptyHeight, float parentTotalWeight, bool vertical);
      void Remeasure();

      // Size has updated
      virtual void OnMeasured(){}

      virtual Vec2 GetMeasureFillSize();
      virtual float GetMeasureTotalWeight();

      // Push translation to renderer
      virtual void PreRender(GUIRenderer* renderer, const Vec2& translation) const;

      // Render component
      virtual void RenderHandle(GUIRenderer* renderer) const;
      virtual void Render(GUIRenderer* renderer) const{}

      // Pop translation from renderer
      virtual void PostRender(GUIRenderer* renderer) const;

      virtual void UpdateHandle(float timeElapsed);
      virtual void Update(float timeElapsed){}

      // These methods generally handle internal stuff and shouldn't be overriden if
      // not necessary use their non EventHandler counterparts instead.
      virtual void OnEventHandler(Event& event, const Vec2& componentPos);
      virtual void OnMousePressEventHandler(MousePressEvent& event, const Vec2& componentPos);
      virtual void OnMouseMoveEventHandler(MouseMoveEvent& event, const Vec2& componentPos);
      virtual void OnMouseReleaseEventHandler(MouseReleaseEvent& event, const Vec2& componentPos);

      virtual void OnEvent(Event& event, const Vec2& translatedMousePos) {}


      // Callbacks
      void SetOnClickCallback(OnClickCallback callback);
      void SetOnPressCallback(OnPressCallback callback);
      void SetOnReleaseCallback(OnReleaseCallback callback);

      Vec2 GetPosition() const;
      Vec2 GetRealPosition() const;
      Vec2 GetSize() const;
      Component* GetParent() const;

      virtual Vec2 GetWrapSize() const;

      // Will return the stored value
      virtual float GetWidth() const;
      virtual float GetHeight() const;
      ComponentSize::Type GetWidthSizeType() const;
      ComponentSize::Type GetHeightSizeType() const;

      // Value of the sizeType, 1w will return 1 and not the size of the component
      const Vec2& GetSizeValue() const;

      // Sets the sizeType size and not the actuall component size.
      Component& SetWidth(float width);
      Component& SetHeight(float height);
      Component& SetWidthSizeType(ComponentSize::Type width);
      Component& SetHeightSizeType(ComponentSize::Type height);

      // Faster to use since it sets all values and does a single remeasure.
      Component& SetSize(float width, float height, ComponentSize::Type widthType, ComponentSize::Type heightType, bool remeasure = true);

      const Style& GetNormalStyle() const;
      const Style& GetHoverStyle() const;
      const Style& GetPressStyle() const;

      Component& SetNormalStyle(const Style& style);
      Component& SetHoverStyle(const Style& style);
      Component& SetPressStyle(const Style& style);

      virtual bool IsMouseInside(const Vec2& parentMouse) const;
      virtual void SetPosition(const Vec2& pos);
      void SetSize(const Vec2& size);

      virtual void SetGUIScene(GUIScene* scene);

      // These four will only be called if the component is focusable
      virtual void OnFocused();
      virtual void OnUnfocused();
      virtual void Focused(){}
      virtual void Unfocused(){}
      virtual void MouseEntered(){}
      virtual void MouseExited(){}

      virtual void ChildChangedFocus(bool focused) {}

      virtual bool UsingMouse();

      Component* GetRootNode();
      Vec2 GetTotalPadding() const;
      const TLBR& GetMargin() const;
      const TLBR& GetPadding() const;
      const TLBR& GetBorder() const;
      Vec2 GetContentSize() const;
      const std::string& GetName() const;

      uint GetComponentDepth() const;

      template <typename T>
      T* GetComponentByName(const std::string& name)
      {
        T* component = dynamic_cast<T*>(GetComponentByNameNoCast(name));
        if(component == nullptr)
          Log::Warning("Could not find Component with given type (name = ", name,")");
        return component;
      }

      virtual Component* GetComponentByNameNoCast(const std::string& name);

      friend std::ostream& operator<<(std::ostream& stream, const Component& component)
      {
        return stream << component.GetName();
      }

    protected:
      virtual void CallOnClickCallback();
      virtual void CallOnPressCallback();
      virtual void CallOnReleaseCallback();
  };
}
