#include "Shader.h"

#include <graphics/shaders/ShaderFactory.h>
#include <internal/GreetGL.h>
#include <utils/ErrorHandle.h>
#include <utils/FileUtils.h>
#include <utils/HotSwapping.h>

#include <fstream>
#include <sstream>

namespace Greet {

  Shader::Shader(const std::string& filename)
    : Resource(filename), m_shaderID{Load(filename)}, uniforms{GetUniforms(m_shaderID)}
  {
  }

  Shader::Shader(const std::string& vertSrc, const std::string& fragSrc, const std::string& geomSrc)
    : m_shaderID{Load(vertSrc, fragSrc, geomSrc, geomSrc.size() != 0)}, uniforms{GetUniforms(m_shaderID)}
  {

  }

  Shader::Shader(const std::string& vertSrc, const std::string& fragSrc)
    : m_shaderID{Load(vertSrc,fragSrc)}, uniforms{GetUniforms(m_shaderID)}
  {

  }

  Shader::~Shader()
  {
    GLCall(glDeleteProgram(m_shaderID));
  }

  void Shader::ReloadResource()
  {
    if(m_shaderID)
    {
      std::array<std::string,3> ss = ReadFile(filePath);
      if(!ss[0].empty())
      {
        uint vertShader = CompileShader(m_shaderID, ss[0], GL_VERTEX_SHADER, false);
        if(!vertShader)
          return;
        uint fragShader = CompileShader(m_shaderID, ss[1], GL_FRAGMENT_SHADER, false);
        if(!fragShader)
          return;
        uint geomShader = 0;
        if(!ss[2].empty())
        {
          geomShader = CompileShader(m_shaderID, ss[2], GL_GEOMETRY_SHADER, false);
          if(!geomShader)
            return;
        }
        GLCall(uint program = glCreateProgram());
        AttachShader(program, vertShader);
        AttachShader(program, fragShader);
        if(geomShader)
          AttachShader(program, geomShader);
        GLCall(glLinkProgram(program));
        GLCall(glValidateProgram(program));
        uint oldProgram = m_shaderID;
        m_shaderID = program;
        MoveUniforms(program, oldProgram);
        GLCall(glDeleteProgram(oldProgram));
      }
    }
    else
      Log::Error("Invalid pointer");
  }

  void Shader::MoveUniforms(uint program, uint oldProgram)
  {
    std::map<std::string, int> oldUniforms = uniforms;
    uniforms = GetUniforms(program);

    std::vector<UniformData> newUniforms = GetListOfUniforms(program);
    GLint numActiveUniforms = 0;
    GLCall(glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numActiveUniforms));
    std::vector<GLchar> nameData(256);
    Enable();
    for(auto it = newUniforms.begin(); it != newUniforms.end();++it)
    {
      auto found = oldUniforms.find(it->name);
      if(found != oldUniforms.end())
      {
        GLint arraySize = 0;
        GLenum type = 0;
        GLsizei actualLength = 0;
        GLCall(glGetActiveUniform(oldProgram, found->second, nameData.size(), &actualLength, &arraySize, &type, &nameData[0]));

        // Type has changed
        if(it->type != type || it->arraySize != arraySize)
          continue;
        if(it->type == GL_FLOAT)
        {
          if(it->arraySize == 1)
          {
            float f;
            GLCall(glGetUniformfv(oldProgram, found->second, &f));
            SetUniform1f(it->name, f);
          }
          else
          {
            float values[it->arraySize];
            // For some reason GetnUniforms doesn't work
            // So you have to query each element individually...
            for(int i = 0;i<it->arraySize;i++)
            {
              GLCall(int location = glGetUniformLocation(program,
                    std::string(found->first + "["+std::to_string(i)+"]").c_str()));
              GLCall(glGetUniformfv(oldProgram, location, &values[i]));
            }
            SetUniform1fv(it->name, it->arraySize, values);
          }
        }
        else if(it->type == GL_FLOAT_VEC2)
        {
          Vec2 f;
          GLCall(glGetnUniformfv(oldProgram, found->second, sizeof(Vec2), (float*)&f));
          SetUniform2f(it->name, f);
        }
        else if(it->type == GL_FLOAT_VEC3)
        {
          Vec3<float> f;
          GLCall(glGetnUniformfv(oldProgram, found->second, sizeof(Vec3<float>), (float*)&f));
          SetUniform3f(it->name, f);
        }
        else if(it->type == GL_FLOAT_VEC4)
        {
          Vec4 f;
          GLCall(glGetnUniformfv(oldProgram, found->second, sizeof(Vec4), (float*)&f));
          SetUniform4f(it->name, f);
        }
        else if(it->type == GL_FLOAT)
        {
          Vec4 f;
          GLCall(glGetnUniformfv(oldProgram, found->second, sizeof(Vec4), (float*)&f));
          SetUniform4f(it->name, f);
        }
        else if(it->type == GL_INT ||
            it->type == GL_SAMPLER_1D ||
            it->type == GL_SAMPLER_2D ||
            it->type == GL_SAMPLER_3D ||
            it->type == GL_SAMPLER_CUBE)
        {
          if(it->arraySize == 1)
          {
            int i;
            GLCall(glGetUniformiv(oldProgram, found->second, &i));
            SetUniform1i(it->name, i);
          }
          else
          {
            int values[it->arraySize];
            // For some reason GetnUniforms doesn't work
            // So you have to query each element individually...
            for(int i = 0;i<it->arraySize;i++)
            {
              GLCall(int location = glGetUniformLocation(program,
                    std::string(found->first + "["+std::to_string(i)+"]").c_str()));
              GLCall(glGetUniformiv(oldProgram, location, &values[i]));
            }
            SetUniform1iv(it->name, it->arraySize, values);
          }
        }
        else if(it->type == GL_UNSIGNED_INT)
        {
          if(it->arraySize == 1)
          {
            uint i;
            GLCall(glGetUniformuiv(oldProgram, found->second, &i));
            SetUniform1ui(it->name, i);
          }
          else
          {
            uint values[it->arraySize];
            // For some reason GetnUniforms doesn't work
            // So you have to query each element individually...
            for(int i = 0;i<it->arraySize;i++)
            {
              GLCall(int location = glGetUniformLocation(program,
                    std::string(found->first + "["+std::to_string(i)+"]").c_str()));
              GLCall(glGetUniformuiv(oldProgram, location, &values[i]));
            }
            SetUniform1uiv(it->name, it->arraySize, values);
          }
        }
        else if(it->type == GL_FLOAT_MAT3)
        {
          Mat3 mat;
          GLCall(glGetnUniformfv(oldProgram, found->second, sizeof(Mat3), mat.elements));
          SetUniformMat3(it->name, mat);
        }
        else if(it->type == GL_FLOAT_MAT4)
        {
          Mat4 mat;
          GLCall(glGetnUniformfv(oldProgram, found->second, sizeof(Mat4), mat.elements));
          SetUniformMat4(it->name, mat);
        }
      }
    }

  }

  std::array<std::string,3> Shader::ReadFile(const std::string& filename)
  {
    std::ifstream file(filename);
    if (!file.good())
    {
      Log::Error("Shader::FromFile Couldn't find shader in path \'", filename, "\'");
      return {
        ShaderFactory::shaderErrorVert,
        ShaderFactory::shaderErrorFrag,
        ""};
    }
    return ReadStream(file);
  }

  std::array<std::string,3> Shader::ReadStream(std::istream& stream)
  {
    std::array<std::stringstream, 3> ss;
    const static uint VERTEX = 0;
    const static uint FRAGMENT = 1;
    const static uint GEOMETRY = 2;

    std::string line;
    uint shader = VERTEX;
    while (std::getline(stream, line))
    {
      if (line == "//vertex")
        shader = VERTEX;
      else if (line == "//fragment")
        shader = FRAGMENT;
      else if (line == "//geometry")
        shader = GEOMETRY;
      else
      {
        ss[shader] << line << std::endl;
      }
    }
    return {ss[0].str(),ss[1].str(),ss[2].str()};
  }

  uint Shader::Load(const std::string& filename)
  {
    std::array<std::string,3> ss =  ReadFile(filename);
    return Load(ss[0], ss[1], ss[2], !ss[2].empty());
  }

  uint Shader::Load(const std::string& vertSrc, const std::string& fragSrc)
  {
    return Load(vertSrc, fragSrc, "", false);
  }

  uint Shader::Load(const std::string& vertSrc, const std::string& fragSrc, const std::string& geomSrc, bool hasGeometry)
  {
    GLCall(uint program = glCreateProgram());

    uint shader = CompileAttachShader(program, vertSrc, GL_VERTEX_SHADER, true);
    if (!shader)
      return LoadError(program);
    shader = CompileAttachShader(program, fragSrc, GL_FRAGMENT_SHADER, true);
    if (!shader)
      return LoadError(program);
    if (hasGeometry)
    {
      shader = CompileAttachShader(program, geomSrc, GL_GEOMETRY_SHADER, true);
      if(!shader)
        return LoadError(program);
    }
    GLCall(glLinkProgram(program));
    int resultFlag;
    GLCall(glGetProgramiv(program, GL_LINK_STATUS, &resultFlag));
    if (resultFlag == GL_FALSE)
    {
      GLint length;
      GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
      std::vector<char> error(length);
      GLCall(glGetProgramInfoLog(program, length, &length, &error[0]));
      Log::Error("Failed to link shader!\n", &error[0]);
      return LoadError(program);
    }
    GLCall(glValidateProgram(program));
    return program;
  }

  uint Shader::LoadError(uint program)
  {
    glDeleteProgram(program);
    GLCall(program = glCreateProgram());

    CompileAttachShader(program, ShaderFactory::shaderErrorVert, GL_VERTEX_SHADER, true);
    CompileAttachShader(program, ShaderFactory::shaderErrorFrag, GL_FRAGMENT_SHADER, true);
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));
    return program;
  }

  std::map<std::string, int> Shader::GetUniforms(const uint program)
  {
    std::map<std::string, int> uniforms;
    GLint numActiveUniforms = 0;
    GLCall(glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numActiveUniforms));
    std::vector<GLchar> nameData(256);
    for(int i= 0; i< numActiveUniforms; ++i)
    {
      GLint arraySize = 0;
      GLenum type = 0;
      GLsizei actualLength = 0;
      GLCall(glGetActiveUniform(program, i, nameData.size(), &actualLength, &arraySize, &type, &nameData[0]));

      // For some reason arrays return 'arrayName[0]'
      for(int i = 0;i<actualLength;i++)
      {
        if(nameData[i] == '[')
        {
          nameData[i] = '\0';
          break;
        }
      }
      std::string name(nameData.data());
      uniforms.emplace(name, i);
    }
    return uniforms;
  }

  std::vector<UniformData> Shader::GetListOfUniforms(uint program)
  {
    std::vector<UniformData> uniforms;
    GLint numActiveUniforms = 0;
    GLCall(glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numActiveUniforms));
    std::vector<GLchar> nameData(256);
    for(int i= 0; i< numActiveUniforms; ++i)
    {
      GLint arraySize = 0;
      GLenum type = 0;
      GLsizei actualLength = 0;
      GLCall(glGetActiveUniform(program, i, nameData.size(), &actualLength, &arraySize, &type, &nameData[0]));

      // For some reason arrays return 'arrayName[0]'
      for(int i = 0;i<actualLength;i++)
      {
        if(nameData[i] == '[')
        {
          nameData[i] = '\0';
          break;
        }
      }
      uniforms.push_back({nameData.data(), arraySize, type});
    }
    return uniforms;
  }

  uint Shader::CompileAttachShader(const uint program, const std::string& shaderSrc, const uint shaderType, bool safeFail)
  {
    uint shader = CompileShader(program, shaderSrc, shaderType, safeFail);
    if(shader == 0)
      return 0;
    AttachShader(program, shader);
    return 1;
  }

  uint Shader::CompileShader(const uint program, const std::string& shaderSrc, const uint shaderType, bool safeFail)
  {
    GLCall(uint shader = glCreateShader(shaderType));
    const char* src = shaderSrc.c_str();
    GLCall(glShaderSource(shader, 1, &src, NULL));
    GLCall(glCompileShader(shader));

    GLint resultFrag;
    GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &resultFrag));
    if (resultFrag == GL_FALSE)
    {
      GLint length;
      GLCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));
      std::vector<char> error(length);
      GLCall(glGetShaderInfoLog(shader, length, &length, &error[0]));
      if (shaderType == GL_FRAGMENT_SHADER)
      {
        Log::Error("Failed to compile fragment Shader!\n", &error[0]);
        ErrorHandle::SetErrorCode(GREET_ERROR_SHADER_FRAGMENT);
        if(safeFail)
        {
          // Should never fail
          GLCall(glShaderSource(shader, 1, &ShaderFactory::shaderErrorFrag, NULL));
        }
        else
        {
          GLCall(glDeleteShader(shader));
          return 0;
        }
      }
      else if (shaderType == GL_VERTEX_SHADER)
      {
        Log::Error("Failed to compile vertex Shader!\n", &error[0]);
        ErrorHandle::SetErrorCode(GREET_ERROR_SHADER_VERTEX);
        if(safeFail)
        {
          // Should never fail
          GLCall(glShaderSource(shader, 1, &ShaderFactory::shaderErrorVert, NULL));
        }
        else
        {
          GLCall(glDeleteShader(shader));
          return 0;
        }
      }
      else if (shaderType == GL_GEOMETRY_SHADER)
      {
        Log::Error("Failed to compile geometry Shader!\n", &error[0]);
        ErrorHandle::SetErrorCode(GREET_ERROR_SHADER_GEOMETRY);
        {
          GLCall(glDeleteShader(shader));
          return 0;
        }
      }
      else
      {
        Log::Error("Failed to compile unknown shader!\n", &error[0]);
        {
          GLCall(glDeleteShader(shader));
          return 0;
        }
      }
      GLCall(glCompileShader(shader));
    }
    return shader;
  }



  void Shader::AttachShader(const uint program, uint shader)
  {
    GLCall(glAttachShader(program, shader));
    GLCall(glDeleteShader(shader));
  }

  void Shader::BindAttributeOutput(uint attachmentId, const std::string& name) const
  {
    GLCall(glBindFragDataLocation(m_shaderID,attachmentId,name.c_str()));
  }

  int Shader::GetUniformLocation(const std::string& name) const
  {
    auto it = uniforms.find(name);
    if(it != uniforms.end())
      return it->second;
    return -1;
  }

  void Shader::SetUniformBoolean(const std::string& name, bool value) const
  {
    GLCall(glUniform1f(GetUniformLocation(name), value ? 1.0f : 0.0f));
  }

  void Shader::SetUniform1f(const std::string& name, float value) const
  {
    GLCall(glUniform1f(GetUniformLocation(name), value));
  }

  void Shader::SetUniform1fv(const std::string& name, uint count, float* value) const
  {
    GLCall(glUniform1fv(GetUniformLocation(name), count, value));
  }

  void Shader::SetUniform1i(const std::string& name, int value) const
  {
    GLCall(glUniform1i(GetUniformLocation(name), value));
  }

  void Shader::SetUniform1iv(const std::string& name, uint count, int* value) const
  {
    GLCall(glUniform1iv(GetUniformLocation(name), count, value));
  }

  void Shader::SetUniform1ui(const std::string& name, uint value) const
  {
    GLCall(glUniform1ui(GetUniformLocation(name), value));
  }

  void Shader::SetUniform1uiv(const std::string& name, uint count, uint* value) const
  {
    GLCall(glUniform1uiv(GetUniformLocation(name), count, value));
  }

  void Shader::SetUniform2f(const std::string& name, const Vec2 &value) const
  {
    GLCall(glUniform2f(GetUniformLocation(name), value.x, value.y));
  }

  void Shader::SetUniform3f(const std::string& name, const Vec3<float>& value) const
  {
    GLCall(glUniform3f(GetUniformLocation(name), value.x, value.y, value.z));
  }

  void Shader::SetUniform4f(const std::string& name, const Vec4 &value) const
  {
    GLCall(glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w));
  }

  void Shader::SetUniformMat3(const std::string& name, const Mat3 &value) const
  {
    GLCall(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, value.elements));
  }

  void Shader::SetUniformMat4(const std::string& name, const Mat4 &value) const
  {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, value.elements));
  }

  bool Shader::operator<(const Shader& s)
  {
    return m_shaderID < s.m_shaderID;
  }

  void Shader::Enable() const
  {
    GLCall(glUseProgram(m_shaderID));
  }

  void Shader::Disable()
  {
    GLCall(glUseProgram(0));
  }

  int Shader::GetAttributeLocation(const std::string& name) const
  {
    GLCall(int location = glGetAttribLocation(m_shaderID, name.c_str()));
    return location;
  }

  Ref<Shader> Shader::FromFile(const std::string& shaderPath)
  {
    return Ref<Shader>{new Shader{shaderPath}};
  }

  Ref<Shader> Shader::FromFile(const std::string& vertPath, const std::string& fragPath)
  {
    std::string vertSourceString = FileUtils::read_file(vertPath.c_str());
    std::string fragSourceString = FileUtils::read_file(fragPath.c_str());
    return Ref<Shader>(new Shader{vertSourceString, fragSourceString});
  }


  Ref<Shader> Shader::FromFile(const std::string& vertPath, const std::string& fragPath, const std::string& geomPath)
  {
    std::string vertSourceString = FileUtils::read_file(vertPath.c_str());
    std::string fragSourceString = FileUtils::read_file(fragPath.c_str());
    std::string geomSourceString = FileUtils::read_file(geomPath.c_str());
    return Ref<Shader>(new Shader{vertSourceString,fragSourceString,geomSourceString});
  }

  Ref<Shader> Shader::FromSource(const std::string& shaderSrc)
  {
    std::stringstream shaderStream{shaderSrc};
    std::array<std::string, 3> ss = ReadStream(shaderStream);
    return Ref<Shader>(new Shader{ss[0], ss[1], ss[2]});
  }

  Ref<Shader> Shader::FromSource(const std::string& vertSrc, const std::string& fragSrc)
  {
    return Ref<Shader>(new Shader{vertSrc, fragSrc});
  }

  Ref<Shader> Shader::FromSource(const std::string& vertSrc, const std::string& fragSrc, const std::string& geomSrc)
  {
    return Ref<Shader>(new Shader{vertSrc, fragSrc, geomSrc});
  }
}
