#pragma once

#include <math/Vec2.h>
#include <math/Vec3.h>
#include <math/Vec4.h>

namespace Greet {
  struct Mat4
  {
    union
    {
      float elements[4 * 4];
      struct
      {
        Vec4 columns[4];
      };
    };

    Mat4();
    Mat4(float diagonal);
    Mat4(float* elem);

    const Vec4& GetColumn(int index)
    {
      return columns[index];
    }

    static Mat4 Identity();
    static Mat4 Orthographic(float left, float right, float top, float bottom, float near, float far);
    static Mat4 ProjectionMatrix(float aspect, float fov, float near, float far);

    static Mat4 TransformationMatrix(const Vec3<float>& position, const Vec3<float>& rotation, const Vec3<float>& scale);
    static Mat4 ViewMatrix(const Vec3<float>& position, const Vec3<float>& rotation);
    static Mat4 AlignAxis(const Vec3<float>& point, const Vec3<float>& normal, const Vec3<float>& up);

    /*
       View Matrix of a third person camera around an object.
       position	- Position of the object
       distance	- distance from the object
       height		- height above the object, between -1 and 1
       rotation	- angle around the object
       */
    static Mat4 TPCamera(const Vec3<float>& position, float distance, float height, float rotation);

    static Mat4 Translate(const Vec3<float>& translation);
    static Mat4 Scale(const Vec3<float>& scaling);

    static Mat4 Translate(const float& x, const float& y, const float& z);
    static Mat4 Scale(const float& x, const float& y, const float& z);

    static Mat4 RotateX(float deg);
    static Mat4 RotateY(float deg);
    static Mat4 RotateZ(float deg);

    static Mat4 RotateRX(float rad);
    static Mat4 RotateRY(float rad);
    static Mat4 RotateRZ(float rad);
    static Mat4 Rotate(float deg, const Vec3<float>& axis);
    static Mat4 RotateR(float rad, const Vec3<float>& axis);

    static Mat4 Inverse(const Mat4& inv);

    Mat4 Cpy();

    Mat4& Multiply(const Mat4& other);
    Vec3<float> Multiply(const Vec3<float>& other) const;
    Vec4 Multiply(const Vec4& other) const;

    friend Mat4 operator*(Mat4 first, const Mat4 &second);
    Mat4& operator*=(const Mat4 &other);

    friend Vec3<float> operator*(const Mat4& first, const Vec3<float>& second);
    friend Vec4 operator*(const Mat4& first, const Vec4& second);
    friend Mat4 operator~(const Mat4& first);

  };
}
