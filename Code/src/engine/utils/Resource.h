#pragma once

#include <string>
#include <utils/FileUtils.h>
#include <utils/UUID.h>

namespace Greet
{
  class Resource
  {
    friend class HotSwapping;

    private:
      static uint s_Id;
      uint id;
      FileUtils::TimeModified modDate;
    protected:
      std::string filePath;
    public:
      Resource()
        : id{++s_Id}
      {}
      Resource(const std::string& filePath);
      Resource(Resource&& res);
      Resource& operator=(Resource&& res);
      virtual ~Resource();
      void CheckResource();
      virtual void ReloadResource() {}

      friend bool operator<(const Resource& r1, const Resource& r2)
      {
        return r1.id < r2.id;
      }
  };
}

