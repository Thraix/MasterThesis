#pragma once

#include <map>
#include <event/Event.h>
#include <functional>

namespace Greet {
  class EventDispatcher
  {
    public:
      typedef std::function<void(Event& e)> EventFn;
    private:
      // Global Event Receivers, these will receive all events
      // event if they are handled or caused focus.
      // Generally used for Application level stuff (screenshots, etc.)
      static std::map<std::string, EventFn> globalEventReceivers;
    public:
      static void OnEvent(Event&& e);
      static void AddGlobalEventReceiver(const std::string& name, EventFn fn);
      static void RemoveGlobalEventReceiver(const std::string& name);
  };
}
