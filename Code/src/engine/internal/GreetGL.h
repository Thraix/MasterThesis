#ifndef _GREETGL_H_
#define _GREETGL_H_

#include <utils/LogUtils.h>
#include <logging/Log.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <FreeImage.h>

static void GLClearError()
{
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR);
}

static void GLLogError(const char* glCall, const char* file, int line, bool assert)
{
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR)
  {
    Greet::Log::Error("OpenGL error(0x", Greet::LogUtils::DecToHex(err), ")", " at ", file, ":", line, " using ", glCall);
  }
}

#if defined _DEBUG
#define GLCall(x) GLClearError(); x; GLLogError(#x,__FILE__,__LINE__, true)
#else
#define GLCall(x) x
#endif

#endif // _GREETGL_H_
