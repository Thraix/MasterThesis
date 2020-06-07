#pragma once

#include <map>
#include <utility>
#include <functional>
#include <utils/xml/XMLObject.h>

#define REGISTER_COMPONENT_DECLARATION(COMPONENT_NAME) \
  static ComponentRegistry<COMPONENT_NAME> reg
#define REGISTER_COMPONENT_DEFINITION(COMPONENT_NAME) \
  ComponentRegistry<COMPONENT_NAME> COMPONENT_NAME::reg(#COMPONENT_NAME)

#define REGISTER_FRAME_DECLARATION(FRAME_NAME) \
  static FrameRegistry<FRAME_NAME> reg
#define REGISTER_FRAME_DEFINITION(FRAME_NAME) \
  FrameRegistry<FRAME_NAME> FRAME_NAME::reg(#FRAME_NAME)

namespace Greet
{
  class Component;
  class Frame;

  class ComponentFactory
  {
    template <typename T>
    friend class ComponentRegistry;
    friend class Window;

    private:
      typedef std::map<std::string, std::function<Component*(const XMLObject& xmlObject, Component*)>> ComponentMap;
      static ComponentMap* guiComponents;
    private:
      static ComponentMap* GetMap()
      {
        if(guiComponents)
          return guiComponents;
        guiComponents = new ComponentMap{};
        return guiComponents;
      }

      static void Cleanup()
      {
        delete guiComponents;
      }

      template <typename T>
      static T* CreateComponent(const XMLObject& xmlObject, Component* parent)
      {
        return new T(xmlObject,parent);
      }

    public:
      static Component* GetComponent(const XMLObject& xmlObject, Component* parent);
  };

  class FrameFactory
  {
    template <typename T>
    friend class FrameRegistry;
    friend class Window;

    private:
      typedef std::map<std::string, std::function<Frame*(const XMLObject& xmlObject)>> FrameMap;

    private:
      static FrameMap* guiFrames;

    private:
      static FrameMap* GetMap()
      {
        if(guiFrames)
          return guiFrames;
        guiFrames = new FrameMap{};
        return guiFrames;
      }

      static void Cleanup()
      {
        delete guiFrames;
      }

      template <typename T>
      static T* CreateFrame(const XMLObject& xmlObject)
      {
        return new T(xmlObject);
      }

    public:
      static Frame* GetFrame(const std::string& filePath);
      static Frame* GetFrame(const XMLObject& xmlObject);

  };

  template <typename T>
  struct ComponentRegistry
  {
    ComponentRegistry(const std::string& s)
    {
      ComponentFactory::GetMap()->emplace(s, &ComponentFactory::CreateComponent<T>);
    }
  };

  template <typename T>
  struct FrameRegistry
  {
    FrameRegistry(const std::string& s)
    {
      FrameFactory::GetMap()->emplace(s, &FrameFactory::CreateFrame<T>);
    }
  };
}
