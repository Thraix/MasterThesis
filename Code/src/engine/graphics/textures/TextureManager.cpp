#include "TextureManager.h"

#include <utils/ErrorHandle.h>
#include <utils/MetaFile.h>
#include <graphics/textures/ImageFactory.h>

namespace Greet{

  std::map<std::string, Ref<Texture2D>> TextureManager::texture2Ds;
  std::map<std::string, Ref<CubeMap>> TextureManager::cubeMaps;

  void TextureManager::AddTexture2D(const std::string& id, const Ref<Texture2D>& texture)
  {
    auto it = texture2Ds.find(id);
    if(it != texture2Ds.end())
    {
      Log::Warning("Texture2D already exists: ", id);
      return;
    }
    texture2Ds.emplace(id, texture);
  }

  void TextureManager::AddCubeMap(const std::string& id, const Ref<CubeMap>& texture)
  {
    auto it = cubeMaps.find(id);
    if(it != cubeMaps.end())
    {
      Log::Warning("CubeMap already exists: ", id);
      return;
    }
    cubeMaps.emplace(id, texture);
  }

  TextureParams GetMetaParams(const MetaFileClass& file)
  {
    TextureParams params;
    std::string filter = file.GetValue("filter", "linear");
    if(filter == "none")
      params.filter = TextureFilter::NONE;
    else if(filter == "linear")
      params.filter = TextureFilter::LINEAR;
    else if(filter == "nearest")
      params.filter = TextureFilter::NEAREST;

    std::string wrap = file.GetValue("wrap", "clampEdge");
    if(wrap == "none")
      params.wrap = TextureWrap::NONE;
    else if(wrap == "clampEdge")
      params.wrap = TextureWrap::CLAMP_TO_EDGE;
    else if(wrap == "clampBorder")
      params.wrap = TextureWrap::CLAMP_TO_BORDER;
    else if(wrap == "mirrorRepeat")
      params.wrap = TextureWrap::MIRRORED_REPEAT;
    else if(wrap == "repeat")
      params.wrap = TextureWrap::REPEAT;
    else if(wrap == "mirrorClampEdge")
      params.wrap = TextureWrap::MIRROR_CLAMP_TO_EDGE;

    std::string format = file.GetValue("format", "RGBA");
    if(format == "depth")
      params.internalFormat = TextureInternalFormat::DEPTH_COMPONENT;
    else if(format == "stencil")
      params.internalFormat = TextureInternalFormat::DEPTH_STENCIL;
    else if(format == "red")
      params.internalFormat = TextureInternalFormat::RED;
    else if(format == "rgb")
      params.internalFormat = TextureInternalFormat::RGB;
    else if(format == "rgba")
      params.internalFormat = TextureInternalFormat::RGBA;

    return params;
  }

  Ref<CubeMap>& TextureManager::LoadCubeMap(const std::string& metaFile)
  {
    auto it = cubeMaps.find(metaFile);
    if(it != cubeMaps.end())
    {
      return it->second;
    }

    MetaFile meta{metaFile};
    const std::vector<MetaFileClass>& metaClasses = meta.GetMetaClass("cubemap");

    if(metaClasses.size() > 0)
    {
      if(metaClasses.size() > 1)
      {
        Log::Warning("Multiple textures defined in meta file: ", metaFile);
      }
      return cubeMaps.emplace(metaFile, new CubeMap(metaClasses[0].GetValue("filepath", metaFile))).first->second;
    }
    else
    {
      uint width, height;
      auto data = ImageFactory::GetCantReadImage(&width, &height);
      Log::Error("No texture found in meta file: ", metaFile);
      static Ref<CubeMap> invalid{new CubeMap(data, width, height)};
      return invalid;
    }
  }

  Ref<Texture2D>& TextureManager::LoadTexture2D(const std::string& metaFile)
  {
    auto it = texture2Ds.find(metaFile);
    if(it != texture2Ds.end())
    {
      return it->second;
    }

    MetaFile meta{metaFile};
    const std::vector<MetaFileClass>& metaClasses = meta.GetMetaClass("texture2d");

    if(metaClasses.size() > 0)
    {
      if(metaClasses.size() > 1)
      {
        Log::Warning("Multiple textures defined in meta file: ", metaFile);
      }
      TextureParams params = GetMetaParams(metaClasses[0]);
      return texture2Ds.emplace(metaFile, Texture2D::Create(metaClasses[0].GetValue("filepath", metaFile), params)).first->second;
    }
    else
    {
      Log::Error("No texture found in meta file: ", metaFile);
      uint width, height;
      auto data = ImageFactory::GetCantReadImage(&width, &height);
      static Ref<Texture2D> invalid{Texture2D::Create(data, width, height)};
      return invalid;
    }
  }

  void TextureManager::CleanupUnused()
  {
    for(auto it = cubeMaps.begin(); it != cubeMaps.end();)
    {
      // Only used by the map
      if(it->second.use_count() == 1)
        it = cubeMaps.erase(it);
      else
        ++it;
    }

    for(auto it = texture2Ds.begin(); it != texture2Ds.end();)
    {
      // Only used by the map
      if(it->second.use_count() == 1)
        it = texture2Ds.erase(it);
      else
        ++it;
    }
  }
}
