#include "CubeMap.h"

#include <internal/GreetGL.h>
#include <utils/ImageUtils.h>
#include <utils/ErrorHandle.h>
#include <internal/GreetGL.h>

namespace Greet {
  CubeMap::CubeMap(const std::string& top, const std::string& bottom, const std::string& left, const std::string& right, const std::string& front, const std::string& back)
    : Texture(GL_TEXTURE_CUBE_MAP)
  {
    LoadCubeMap(top, bottom, left, right, front, back);
  }

  CubeMap::CubeMap(const std::string& map)
    : Resource(map), Texture(GL_TEXTURE_CUBE_MAP)
  {
    uint width;
    uint height;
    auto res = ImageUtils::LoadImage(map.c_str(), &width, &height);
    LoadCubeMap(res.second, width, height, res.first);
  }

  CubeMap::CubeMap(const std::vector<byte>& data, uint width, uint height)
    : Texture(GL_TEXTURE_CUBE_MAP)
  {
    LoadCubeMap(data, width, height, true);
  }

  CubeMap::CubeMap(uint texId)
    : Texture(texId, (uint)GL_TEXTURE_CUBE_MAP)
  {

  }

  CubeMap::CubeMap()
    : Texture(GL_TEXTURE_CUBE_MAP)
  {
  }

  void CubeMap::ReloadResource()
  {
    uint width;
    uint height;
    auto res = ImageUtils::LoadImage(filePath.c_str(), &width, &height);
    LoadCubeMap(res.second, width, height, res.first);
  }

  void CubeMap::LoadCubeMap(const std::vector<byte>& data, uint width, uint height, bool printDimensionError)
  {
    if(width % 4 != 0 || height % 3 != 0)
    {
      LoadImage(data,width,height,GL_TEXTURE_CUBE_MAP_POSITIVE_X,false);
      LoadImage(data,width,height,GL_TEXTURE_CUBE_MAP_NEGATIVE_X,false);
      LoadImage(data,width,height,GL_TEXTURE_CUBE_MAP_POSITIVE_Y,false);
      LoadImage(data,width,height,GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,false);
      LoadImage(data,width,height,GL_TEXTURE_CUBE_MAP_POSITIVE_Z,false);
      LoadImage(data,width,height,GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,false);
    }
    else
    {
      uint w = width / 4;
      uint h = height / 3;
      LoadImage(ImageUtils::CropImage(data,width,height,0,h,w,h),w,h,GL_TEXTURE_CUBE_MAP_POSITIVE_X,true);
      LoadImage(ImageUtils::CropImage(data,width,height,w*2,h,w,h),w,h,GL_TEXTURE_CUBE_MAP_NEGATIVE_X,true);

      LoadImage(ImageUtils::CropImage(data,width,height,w,0,w,h),w,h,GL_TEXTURE_CUBE_MAP_POSITIVE_Y,true);
      LoadImage(ImageUtils::CropImage(data,width,height,w,h*2,w,h),w,h,GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,true);

      LoadImage(ImageUtils::CropImage(data,width,height,w,h,w,h),w,h,GL_TEXTURE_CUBE_MAP_POSITIVE_Z,true);
      LoadImage(ImageUtils::CropImage(data,width,height,w*3,h,w,h),w,h,GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,true);
    }
    LoadParameters();

    Disable();
  }

  void CubeMap::LoadCubeMap(const std::string& top, const std::string& bottom, const std::string& left, const std::string& right, const std::string& front, const std::string& back)
  {
    Enable(0);

    LoadImage(right, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    LoadImage(left, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
    LoadImage(top, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
    LoadImage(bottom, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
    LoadImage(back, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
    LoadImage(front, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

    LoadParameters();

    Disable();
  }

  void CubeMap::LoadParameters()
  {
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
  }

  void CubeMap::LoadImage(const std::string& image, uint mapLocation)
  {
    uint width;
    uint height;
    auto res = ImageUtils::LoadImage(image.c_str(), &width, &height);
    LoadImage(res.second,width,height,mapLocation,true);
  }

  void CubeMap::LoadImage(const std::vector<byte>& bits, uint width, uint height, uint mapLocation, bool flip)
  {
    if(flip)
    {
      GLCall(glTexImage2D(mapLocation, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImageUtils::FlipImage(bits,width,height).data()));
    }
    else
    {
      GLCall(glTexImage2D(mapLocation, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bits.data()));
    }
  }
}
