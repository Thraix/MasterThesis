#pragma once

#include <math/Vec2.h>
#include <math/Vec3.h>

namespace Greet{
  struct Mat3
  {
    union
    {
      float elements[3 * 3];
      struct
      {
        Vec3<float> columns[3];
      };
    };

    Mat3();
    Mat3(float diagonal);
    Mat3(float* elem);

    const Vec3<float>& getColumn(int index)
    {
      return columns[index];
    }

    static Mat3 Identity();
    static Mat3 Orthographic(float left, float right, float top, float bottom);
    static Mat3 OrthographicViewport();
    static Mat3 Quad(float x, float y, float width, float height);
    static Mat3 Quad(const Vec2& pos, const Vec2& size);

    static Mat3 Translate(const Vec2& translation);
    static Mat3 Scale(const Vec2& scaling);
    static Mat3 Shear(const Vec2& shearing);

    static Mat3 Translate(const float& x, const float& y);
    static Mat3 Scale(const float& x, const float& y);
    static Mat3 Rotate(float deg);
    static Mat3 RotateR(float rad);
    static Mat3 Shear(const float& x, const float& y);

    static Mat3 Inverse(const Mat3& mat);
    Mat3 Cpy();

    Mat3& Multiply(const Mat3 &other);
    Vec2 Multiply(const Vec2 &other) const;
    Vec3<float> Multiply(const Vec3<float>& other) const;

    friend Mat3 operator*(Mat3 first, const Mat3 &second);
    Mat3& operator*=(const Mat3 &other);

    friend Vec2 operator*(const Mat3& first, const Vec2& second);
    friend Vec3<float> operator*(const Mat3& first, const Vec3<float>& second);
    friend Mat3 operator~(const Mat3& first);

  };
}
