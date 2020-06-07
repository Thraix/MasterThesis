#include "ShaderFactory.h"

#include <graphics/shaders/Shader.h>


namespace Greet {

  const char* ShaderFactory::shader2d=
#include "factory/shader2d.glsl"
    ;

  const char* ShaderFactory::shader3d =
#include "factory/shader3d.glsl"
    ;

  const char* ShaderFactory::shaderGUI =
#include "factory/shaderGUI.glsl"
    ;

  const char* ShaderFactory::shaderSkybox =
#include "factory/shaderSkybox.glsl"
    ;

  const char* ShaderFactory::shaderErrorVert =
#include "factory/shaderError.vert"
    ;
  const char* ShaderFactory::shaderErrorFrag =
#include "factory/shaderError.frag"
    ;

  Ref<Shader> ShaderFactory::Shader2D()
  {
    return Shader::FromSource(shader2d);
  }

  Ref<Shader> ShaderFactory::Shader3D()
  {
    return Shader::FromSource(shader3d);
  }

  Ref<Shader> ShaderFactory::ShaderGUI()
  {
    return Shader::FromSource(shaderGUI);
  }

  Ref<Shader> ShaderFactory::ShaderSkybox()
  {
    return Shader::FromSource(shaderSkybox);
  }

  Ref<Shader> ShaderFactory::ShaderError()
  {
    Log::Info("Loading error shader");
    return Shader::FromSource(shaderErrorVert, shaderErrorFrag);
  }
}
