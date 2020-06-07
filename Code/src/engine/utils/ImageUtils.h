#pragma once

#include <common/Types.h>

#include <vector>
#include <string>

namespace Greet {
  struct ImageUtils
  {
    static void PrintImage(byte* bits, uint width, uint height);
    static std::vector<byte> FlipImage(const std::vector<byte>& bits, uint width, uint height);
    static std::pair<bool, std::vector<byte>> LoadImage(const std::string& filepath, uint* width, uint* height);
    static std::vector<byte> CropImage(const std::vector<byte>& bits, uint width,  uint height,  uint cx,  uint cy,  uint cwidth,  uint cheight);
    static void SaveImageBytes(const std::string& filepath, const std::string& output);
    static std::vector<byte> CreateHeightmapImage(const std::vector<float>& heightMap, uint width, uint height);
  };
}
