#include "EventDispatcher.h"

#include <functional>
#include <utils/Utils.h>
#include <logging/Log.h>
#include <graphics/GlobalSceneManager.h>

namespace Greet {

  std::map<std::string, EventDispatcher::EventFn> EventDispatcher::globalEventReceivers;

  void EventDispatcher::OnEvent(Event&& event)
  {
    GlobalSceneManager::GetSceneManager().OnEvent(event);
    for(auto&& receivers : globalEventReceivers)
      receivers.second(event);
  }

  void EventDispatcher::AddGlobalEventReceiver(const std::string& name, EventFn fn)
  {
    if(!fn)
    {
      Log::Error("EventDispatcher received nullptr global event receiver");
      return;
    }
    if(globalEventReceivers.find(name) != globalEventReceivers.end())
    {
      Log::Error("Global event receiver already exists in EventDispatcher (name=",name,")");
      return;
    }
    globalEventReceivers.emplace(name,fn);
  }

  void EventDispatcher::RemoveGlobalEventReceiver(const std::string& name)
  {
    auto it = globalEventReceivers.find(name);
    if(it == globalEventReceivers.end())
    {
      Log::Error("EventDispatcher could not find global event receiver (name=",name,")");
      return;
    }
    globalEventReceivers.erase(it);
  }
}
