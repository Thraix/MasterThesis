#pragma once

#include <math/Mat3.h>

namespace Greet {
  class Transform
  {
    public:
      Mat3 m_matrix;
    private:

    public:
      Transform()
        : m_matrix(Mat3(1))
      {

      }

      Transform(Mat3 matrix)

      {
        m_matrix = matrix;
      }

      Transform& Init(float diagonal = 1)
      {
        m_matrix = Mat3(diagonal);
        return *this;
      }

      Transform& Scale(float x, float y)
      {
        m_matrix *= Mat3::Scale(x, y);
        return *this;
      }

      Transform& Scale(Vec2 scale)
      {
        m_matrix *= Mat3::Scale(scale);
        return *this;
      }

      Transform& Translate(float x, float y)
      {
        m_matrix *= Mat3::Translate(x,y);
        return *this;
      }

      Transform& Translate(Vec2 translate)
      {
        m_matrix *= Mat3::Translate(translate);
        return *this;
      }

      Transform& Rotate(float deg)
      {
        m_matrix *= Mat3::Rotate(deg);
        return *this;
      }

      Transform& RotateR(float rad)
      {
        m_matrix *= Mat3::RotateR(rad);
        return *this;
      }

      Transform& Shear(float x, float y)
      {
        m_matrix *= Mat3::Shear(x, y);
        return *this;
      }

      Transform& Shear(Vec2 shear)
      {
        m_matrix *= Mat3::Shear(shear);
        return *this;
      }

      inline const Mat3& GetMatrix() const { return m_matrix; }
  };
}
