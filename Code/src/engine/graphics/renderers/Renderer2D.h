#pragma once

#include <stack>

#include <graphics/Sprite.h>
#include <graphics/fonts/Font.h>
#include <graphics/shaders/Shader.h>
#include <logging/Log.h>
#include <math/Maths.h>

namespace Greet{

  class Renderable;
  class Renderable2D;
  class RenderablePoly;

  class Renderer2D
  {
    protected:
      std::stack<Mat3> m_transformationStack;
      const Mat3* m_transformationBack;
      Ref<Shader> shader;
    public:
      Renderer2D(const Ref<Shader>& shader)
        : shader{shader}
      {
        m_transformationStack.push(Mat3::Identity());
        m_transformationBack = &m_transformationStack.top();
      }

      virtual ~Renderer2D() {}
      virtual void Begin() {}
      virtual void Flush() = 0;
      virtual void End() {}

      virtual void Submit(const Renderable2D& renderable) = 0;

      void PushMatrix(const Mat3 &matrix, bool override = false)
      {
        if (override)
          m_transformationStack.push(matrix);
        else
          m_transformationStack.push(*m_transformationBack * matrix);
        m_transformationBack = &m_transformationStack.top();

      }
      void PopMatrix()
      {
        if (m_transformationStack.size() > 1)
          m_transformationStack.pop();
        else
          Log::Warning("Trying to pop the last matrix.");
        m_transformationBack = &m_transformationStack.top();
      }

      const Mat3& GetMatrix() const
      {
        return *m_transformationBack;
      }

      const Ref<Shader>& GetShader() const { return shader; }
  };
}
