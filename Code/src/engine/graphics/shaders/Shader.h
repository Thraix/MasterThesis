#pragma once

#include <string>
#include <map>
#include <math/Maths.h>
#include <memory>
#include <functional>
#include <utils/Utils.h>
#include <utils/FileUtils.h>
#include <utils/HotSwapping.h>
#include <common/Memory.h>

namespace Greet {

  struct UniformData
  {
    UniformData(const std::string& name, int arraySize, uint type)
      : name{name}, arraySize{arraySize}, type{type}
    {}
    std::string name;
    int arraySize;
    uint type;
  };

  struct ShaderDeleter final
  {
    void operator()(uint* id);
  };

  class Shader final : public Resource
  {
    private:
      uint m_shaderID;
      std::map<std::string, int> uniforms;

    private:
      Shader(const std::string& filename);
      Shader(const std::string& vertSrc, const std::string& fragSrc, const std::string& geomSrc);
      Shader(const std::string& vertSrc, const std::string& fragSrc);

      uint Load(const std::string& filename);
      uint Load(const std::string& vertSrc, const std::string& fragSrc, const std::string& geomSrc, bool hasGeometry);
      uint Load(const std::string& vertSrc, const std::string& fragSrc);
      uint LoadError(uint program);
      uint CompileAttachShader(const uint program, const std::string& shaderSrc, const uint shaderType, bool safeFail);
      uint CompileShader(const uint program, const std::string& shaderSrc, const uint shaderType, bool safeFail);
      void AttachShader(const uint program, uint shader);
      int GetUniformLocation(const std::string& name) const;
      std::map<std::string, int> GetUniforms(const uint program);
      std::vector<UniformData> GetListOfUniforms(uint program);
      void MoveUniforms(uint program, uint oldProgram);

    public:
      ~Shader();

      void ReloadResource() override;
      void Enable() const;
      static void Disable();
      uint GetProgram() const { return m_shaderID; }

      int GetAttributeLocation(const std::string& name) const;
      void BindAttributeOutput(uint attachmentId, const std::string& name) const;

      void SetUniformBoolean(const std::string& name, bool value) const;
      void SetUniform1f(const std::string& name, float value) const;
      void SetUniform1fv(const std::string& name, uint count, float* value) const;
      void SetUniform1i(const std::string& name, int value) const;
      void SetUniform1iv(const std::string& name, uint count, int* value) const;
      void SetUniform1ui(const std::string& name, uint value) const;
      void SetUniform1uiv(const std::string& name, uint count, uint* values) const;
      void SetUniform2f(const std::string& name, const Vec2& value) const;
      void SetUniform3f(const std::string& name, const Vec3<float>& value) const;
      void SetUniform4f(const std::string& name, const Vec4& value) const;
      void SetUniformMat3(const std::string& name, const Mat3 &value) const;
      void SetUniformMat4(const std::string& name, const Mat4 &value) const;
      bool operator<(const Shader& s);
    private:
      static std::array<std::string,3> ReadFile(const std::string& filename);
      static std::array<std::string,3> ReadStream(std::istream& stream);
    public:
      static Ref<Shader> FromFile(const std::string& shaderPath);
      static Ref<Shader> FromFile(const std::string& vertPath, const std::string& fragPath);
      static Ref<Shader> FromFile(const std::string& vertPath, const std::string& fragPath, const std::string& geomPath);
      static Ref<Shader> FromSource(const std::string& shaderSrc);
      static Ref<Shader> FromSource(const std::string& vertSrc, const std::string& fragSrc);
      static Ref<Shader> FromSource(const std::string& vertSrc, const std::string& fragSrc, const std::string& geomSrc);

  };

}
