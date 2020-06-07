#include "ImageFactory.h"

#include <logging/Log.h>
#include <utils/Utils.h>


namespace Greet {
  std::vector<BYTE> ImageFactory::GetBadFormatImage(uint* width, uint* height)
  {
    return GetErrorImage(width,height,0xff00ff,0xaa00aa);
  }

  std::vector<BYTE> ImageFactory::GetBadBPPImage(uint* width, uint* height)
  {
    return GetErrorImage(width,height,0xffff00,0xaaaa00);
  }

  std::vector<BYTE> ImageFactory::GetCantReadImage(uint* width, uint* height)
  {
    return GetErrorImage(width,height,0x00ffff,0x00aaaa);
  }

  std::vector<BYTE> ImageFactory::GetCropErrorImage(uint* width, uint* height)
  {
    return GetErrorImage(width,height,0xff0000,0xaa0000);
  }

  std::vector<BYTE> ImageFactory::GetErrorImage(uint* width, uint* height, uint lightColor, uint darkColor)
  {
    *width = 16;
    *height = 16;
    std::vector<BYTE> bits(*width * *height * 4);
    size_t offset = 0;
    for(uint y = 0;y<*height;y++)
    {
      for(uint x = 0;x<*width;x++)
      {
        uint color = lightColor;
        if(x == 0 || y == *height-1)
          color = darkColor;
        bits[offset + FI_RGBA_RED] = (color & 0xff0000) >> 16;
        bits[offset + FI_RGBA_GREEN] = (color & 0xff00) >> 8;
        bits[offset + FI_RGBA_BLUE] = (color & 0xff) >> 0;
        bits[offset + FI_RGBA_ALPHA] = 0xff;
        offset += 4;
      }
    }
    return bits;
  }
}
