#pragma once

#include <map>
#include <graphics/textures/Texture2D.h>
#include <graphics/textures/CubeMap.h>
#include <common/Memory.h>

namespace Greet {
  class TextureManager
  {
    private:
      static std::map<std::string, Ref<CubeMap>> cubeMaps;
      static std::map<std::string, Ref<Texture2D>> texture2Ds;
    public:

      static void AddTexture2D(const std::string& id, const Ref<Texture2D>& texture);
      static void AddCubeMap(const std::string& id, const Ref<CubeMap>& texture);

      static Ref<Texture2D>& LoadTexture2D(const std::string& metaFile);
      static Ref<CubeMap>& LoadCubeMap(const std::string& metaFile);

      static void CleanupUnused();
    private:
      TextureManager(){}
  };
}
