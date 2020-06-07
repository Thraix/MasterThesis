#pragma once

#include <functional>
#include <map>

#include <string>
#include <common/Types.h>
#include <graphics/textures/Texture.h>
#include <utils/ImageUtils.h>
#include <utils/HotSwapping.h>

namespace Greet {

  class CubeMap : public Texture, public Resource
  {
    public:
      CubeMap(const std::string& top, const std::string& bottom, const std::string& left, const std::string& right, const std::string& front, const std::string& back);
      CubeMap(const std::string& map);
      CubeMap(const std::vector<byte>& data, uint width, uint height);
      CubeMap(uint texId);
      CubeMap();
      CubeMap(CubeMap&&) = default;
      CubeMap& operator=(CubeMap&&) = default;
      void ReloadResource() override;
    private:
      void LoadCubeMap(const std::vector<byte>& data, uint width, uint height, bool printDimensionError);
      void LoadCubeMap(const std::string& top, const std::string& bottom, const std::string& left, const std::string& right, const std::string& front, const std::string& back);
      void LoadParameters();
      void LoadImage(const std::string& image, uint mapLocation);
      void LoadImage(const std::vector<byte>& bits, uint width, uint height, uint mapLocation, bool flip);
  };
}
