#pragma once

#include <math/Maths.h>
#include <graphics/shaders/Shader.h>
#include <string.h>

namespace Greet {
  class Light
  {
    public:
      Vec3<float> position;
      uint color;

    public:
      Light(Vec3<float> position, uint color) : position(position) , color(color) {}

      void SetToUniform(const Ref<Shader>& shader, const std::string& prefix)
      {
        shader->SetUniform3f((prefix + "_position").c_str(), position);
        Vec3 color1 = Vec3(((color & 0xff0000) >> 16) / 255.0f, ((color & 0xff00) >> 8) / 255.0f, (color & 0xff) / 255.0f);
        shader->SetUniform3f((prefix + "_color").c_str(), color1);
      }

  };
}
