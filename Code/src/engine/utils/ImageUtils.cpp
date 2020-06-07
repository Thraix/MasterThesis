#include "ImageUtils.h"

#include <logging/Log.h>
#include <iostream>
#include <graphics/textures/ImageFactory.h>
#include <fstream>
#include <utils/ErrorHandle.h>
#include <utils/StringUtils.h>

namespace Greet {

  void ImageUtils::PrintImage(byte* bits, uint width, uint height)
  {
    for (uint y = 0;y<height;y++)
    {
      byte* pixel = (byte*)bits;
      for (uint x = 0;x<width;x++)
      {
        std::string s = "pixel(" + StringUtils::to_string(x) + "," + StringUtils::to_string(y) + ")";
        s += "(" + StringUtils::to_string((uint)pixel[FI_RGBA_RED]) + ",";
        s += StringUtils::to_string((uint)pixel[FI_RGBA_GREEN]) + ",";
        s += StringUtils::to_string((uint)pixel[FI_RGBA_BLUE]) + ")";
        pixel += 4;
        Log::Info(s);
      }
      bits += 4*width;
    }
  }

  std::vector<byte> ImageUtils::FlipImage(const std::vector<byte>& bits, uint width, uint height)
  {
    std::vector<byte> result(width*height*4);
    size_t offset = 4*width*height;
    size_t rowOffset = 0;
    for (uint y = 0;y < height;y++)
    {
      for (uint x = 0;x < width;x++)
      {
        offset -= 4;
        memcpy(result.data()+offset,bits.data() + rowOffset,4);
        rowOffset += 4;
      }
    }

    return result;
  }

  std::pair<bool, std::vector<byte>> ImageUtils::LoadImage(const std::string& filepath, uint* width, uint* height)
  {
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

    FIBITMAP *dib = nullptr;

    fif = FreeImage_GetFileType(filepath.c_str(), 0);

    if (fif == FIF_UNKNOWN)
      fif = FreeImage_GetFIFFromFilename(filepath.c_str());

    if (fif == FIF_UNKNOWN)
    {
      Log::Error("FreeImage file format is not supported or file not exist: ", filepath);
      ErrorHandle::SetErrorCode(GREET_ERROR_IMAGE_FORMAT);
      return {false, ImageFactory::GetBadFormatImage(width,height)};
    }

    if (FreeImage_FIFSupportsReading(fif))
      dib = FreeImage_Load(fif, filepath.c_str());
    if (!dib)
    {
      Log::Error("FreeImage file Cannot be read: ", filepath);
      ErrorHandle::SetErrorCode(GREET_ERROR_IMAGE_READ);
      return {false,ImageFactory::GetCantReadImage(width,height)};
    }


    byte* fiBits = FreeImage_GetBits(dib);

    *width = FreeImage_GetWidth(dib);
    *height = FreeImage_GetHeight(dib);
    uint bpp = FreeImage_GetBPP(dib);
    bpp >>= 3;
    if (bpp != 3 && bpp != 4)
    {
      Log::Error("Bits per pixel is not valid (24 or 32): ", filepath, " ", bpp * 8, " bpp");
      ErrorHandle::SetErrorCode(GREET_ERROR_IMAGE_BPP);
      FreeImage_Unload(dib);
      return {false,ImageFactory::GetBadBPPImage(width,height)};
    }

    std::vector<byte> bits((*width) * (*height) * 4);
    uint resultI = 0;
    uint bitsI = 0;
    for(uint y = 0; y < *height; y++)
    {
      for(uint x = 0; x < *width; x++)
      {
        // Incase the order of FreeImage is not RGB (its probably BGRA)
        bits[resultI++] = fiBits[bitsI + FI_RGBA_RED];
        bits[resultI++] = fiBits[bitsI + FI_RGBA_GREEN];
        bits[resultI++] = fiBits[bitsI + FI_RGBA_BLUE];
        if(bpp == 4)
          bits[resultI++] = fiBits[bitsI + FI_RGBA_ALPHA];
        else
          bits[resultI++] = 0xff;
        bitsI += bpp;
      }
    }
    FreeImage_Unload(dib);
    return {true,bits};
  }

  std::vector<byte> ImageUtils::CreateHeightmapImage(const std::vector<float>& heightMap, uint width, uint height)
  {
    std::vector<byte> data(width * height * 4);
    for(int i = 0;i<width*height;i++)
    {
      data[i * 4 + FI_RGBA_RED]   = heightMap[i] * 255;
      data[i * 4 + FI_RGBA_GREEN] = heightMap[i] * 255;
      data[i * 4 + FI_RGBA_BLUE]  = heightMap[i] * 255;
      data[i * 4 + FI_RGBA_ALPHA] = 0xff;
    }
    return data;
  }

  std::vector<byte> ImageUtils::CropImage(const std::vector<byte>& bits, uint width,  uint height,  uint cx,  uint cy,  uint cwidth,  uint cheight)
  {
    if (cx >= width || cy >= height || cx + cwidth > width || cy + cheight > height)
    {
      Log::Error("Invalid bounds when cropping image. ", cx, ", ", cy, ", ", cwidth, ", ", cheight);
      ErrorHandle::SetErrorCode(GREET_ERROR_IMAGE_CROP);
      return ImageFactory::GetCropErrorImage(&width,&height);
    }
    cy = height - cheight - cy;
    std::vector<byte> result(cwidth * cheight * 4);
    size_t offset = (cx + cy * width) * 4;
    size_t resultOffset = 0;
    for (uint y = 0;y < cheight;y++)
    {
      memcpy(result.data()+resultOffset, bits.data()+offset, cwidth * 4);
      offset += width * 4;
      resultOffset += cwidth * 4;
    }
    return result;
  }

  void ImageUtils::SaveImageBytes(const std::string& filepath, const std::string& output)
  {
    uint width,height,bpp;
    auto res = LoadImage(filepath, &width, &height);
    if(res.first)
    {

      std::ofstream fout;
      fout.open(output, std::ios_base::binary | std::ios_base::out);
      fout.write((char*) res.second.data(), width*height*bpp/8);

      fout.close();
    }
  }
}
