#pragma once

#include <utility>
#include <string>
#include <utils/Utils.h>
#include <utils/FileUtils.h>
#include <functional>
#include <map>
#include <memory>
#include <utils/Resource.h>

namespace Greet
{
  struct HotSwapping
  {
    private:
      static std::map<uint, Resource*> resources;

    public:
      static void AddHotswapResource(Resource* res)
      {
        resources.emplace(res->id, res);
      }

      static void RemoveHotswapResource(Resource* res)
      {
        auto result = resources.erase(res->id);
      }

      static void MoveHotswapResource(Resource* res)
      {
        auto it = resources.find(res->id);
        if(it != resources.end())
        {
          it->second = res;
        }
      }

      static void CheckResources()
      {
        for(auto&& resource : resources)
        {
          resource.second->CheckResource();
        }
      }
  };
}
