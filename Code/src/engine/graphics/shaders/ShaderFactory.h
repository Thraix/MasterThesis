#pragma once

#include <graphics/shaders/Shader.h>

namespace Greet {

  struct ShaderFactory
  {

    static const char* shader2d;
    static const char* shader3d;
    static const char* shaderGUI;
    static const char* shaderSkybox;
    static const char* shaderErrorVert;
    static const char* shaderErrorFrag;

    static Ref<Shader> Shader2D();
    static Ref<Shader> Shader3D();
    static Ref<Shader> ShaderGUI();
    static Ref<Shader> ShaderSkybox();
    static Ref<Shader> ShaderError();

  };
}
