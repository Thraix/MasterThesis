#include "Resource.h"

#include <utils/HotSwapping.h>

namespace Greet
{
  uint Resource::s_Id = 0;

  Resource::Resource(const std::string& filePath)
    : id{++s_Id}, modDate{FileUtils::GetTimeModified(filePath)}, filePath{filePath}
  {
#if defined(_DEBUG) && defined(_GREET_HOTSWAP)
    HotSwapping::AddHotswapResource(this);
#endif
  }

  Resource::Resource(Resource&& res)
  {
    id = res.id;
    filePath = std::move(res.filePath);
    modDate = std::move(res.modDate);
    HotSwapping::MoveHotswapResource(this);
    res.id = 0;
  }

  Resource& Resource::operator=(Resource&& res)
  {
    id = res.id;
    filePath = std::move(res.filePath);
    modDate = std::move(res.modDate);
    HotSwapping::MoveHotswapResource(this);
    res.id = 0;
    return *this;
  }

  Resource::~Resource()
  {
#if defined(_DEBUG) && defined(_GREET_HOTSWAP)
    HotSwapping::RemoveHotswapResource(this);
#endif
  }


  void Resource::CheckResource()
  {
    FileUtils::TimeModified newDate = FileUtils::GetTimeModified(filePath);
    if(modDate < newDate)
    {
      modDate = newDate;
      ReloadResource();
    }
  }
}
