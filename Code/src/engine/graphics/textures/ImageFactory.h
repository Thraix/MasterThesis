#pragma once

#include <vector>
#include <FreeImage.h>
#include <common/Types.h>

namespace Greet {

  struct ImageFactory
  {
    static std::vector<BYTE> GetBadFormatImage(uint* width, uint* height);
    static std::vector<BYTE> GetBadBPPImage(uint* width, uint* height);
    static std::vector<BYTE> GetCantReadImage(uint* width, uint* height);
    static std::vector<BYTE> GetCropErrorImage(uint* width, uint* height);
    static std::vector<BYTE> GetErrorImage(uint* width, uint* height, uint lightColor, uint darkColor);
    static std::vector<BYTE> GetFrameImage(uint* width, uint* height);
  };
}
