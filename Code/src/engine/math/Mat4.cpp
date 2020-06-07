#include "Mat4.h"

#include <math/MathFunc.h>
#include <cstring>

#define _0_0 0
#define _0_1 1
#define _0_2 2
#define _0_3 3
#define _1_0 4
#define _1_1 5
#define _1_2 6
#define _1_3 7
#define _2_0 8
#define _2_1 9
#define _2_2 10
#define _2_3 11
#define _3_0 12
#define _3_1 13
#define _3_2 14
#define _3_3 15
#define SIZE 4
#define FLOATS SIZE*SIZE

namespace Greet {
  Mat4::Mat4()
  {
    memset(elements, 0.0f, FLOATS * sizeof(float));
  }

  Mat4::Mat4(float diagonal)
  {
    memset(elements, 0.0f, FLOATS * sizeof(float));
    elements[_0_0] = diagonal;
    elements[_1_1] = diagonal;
    elements[_2_2] = diagonal;
    elements[_3_3] = diagonal;
  }

  Mat4::Mat4(float* elem)
  {
    memcpy(elements, elem, FLOATS * sizeof(float));
  }

  Mat4 Mat4::Identity()
  {
    return Mat4(1.0f);
  }

  Mat4 Mat4::Orthographic(float left, float right, float top, float bottom, float near, float far)
  {
    Mat4 result(1.0f);

    result.elements[_0_0] = 2.0f / (right - left);
    result.elements[_1_1] = 2.0f / (top - bottom);
    result.elements[_2_2] = -2.0f / (far- near);
    result.elements[_3_0] = -(right + left) / (right - left);
    result.elements[_3_1] = -(top + bottom) / (top - bottom);
    result.elements[_3_2] = -(far + near) / (far - near);

    return result;
  }

  Mat4 Mat4::ProjectionMatrix(float aspect, float fov, float near, float far)
  {
    Mat4 result(1.0f);
    float tan2 = 1.0f / tan(Math::ToRadians(fov * 0.5f));
    result.elements[_0_0] = tan2 / aspect;
    result.elements[_1_1] = tan2;
    result.elements[_2_2] = (far + near) / (near - far);
    result.elements[_3_2] = 2 * (far * near) / (near - far);
    result.elements[_2_3] = -1;
    result.elements[_3_3] = 0;

    return result;
  }

  Mat4 Mat4::TransformationMatrix(const Vec3<float>& position, const Vec3<float>& rotation, const Vec3<float>& scale)
  {
    return
      Mat4::Translate(position) *
      Mat4::RotateX(rotation.x) *
      Mat4::RotateY(rotation.y) *
      Mat4::RotateZ(rotation.z) *
      Mat4::Scale(scale);
  }

  Mat4 Mat4::ViewMatrix(const Vec3<float>& position, const Vec3<float>& rotation)
  {
    return
      Mat4::RotateX(rotation.x) *
      Mat4::RotateY(rotation.y) *
      Mat4::RotateZ(rotation.z) *
      Mat4::Translate(-position.x, -position.y, -position.z);
  }

  Mat4 Mat4::AlignAxis(const Vec3<float>& point, const Vec3<float>& normal, const Vec3<float>& forward)
  {
    Vec3<float> yaxis = normal;
    yaxis.Normalize();

    Vec3<float> xaxis = (forward.Cross(yaxis)).Normalize();
    Vec3<float> zaxis = xaxis.Cross(yaxis);

    Mat4 result(1.0f);

    result.elements[_0_0] = xaxis.x;
    result.elements[_0_1] = xaxis.y;
    result.elements[_0_2] = xaxis.z;
    result.elements[_1_0] = yaxis.x;
    result.elements[_1_1] = yaxis.y;
    result.elements[_1_2] = yaxis.z;
    result.elements[_2_0] = zaxis.x;
    result.elements[_2_1] = zaxis.y;
    result.elements[_2_2] = zaxis.z;

    result.elements[_3_0] = point.x;
    result.elements[_3_1] = point.y;
    result.elements[_3_2] = point.z;
    return result;
  }

  Mat4 Mat4::TPCamera(const Vec3<float>& position, float distance, float height, float rotation)
  {
    return
      RotateRX(asin(height)) *
      Mat4::RotateY(90) *
      Mat4::Translate(Vec3<float>(sqrt(1 - height*height) * distance, -height * distance, 0)) *
      Mat4::RotateY(rotation) *
      Mat4::Translate(-position.x, -position.y, -position.z);
  }


  Mat4 Mat4::Translate(const Vec3<float>& translation)
  {
    Mat4 result(1.0f);

    result.elements[_3_0] = translation.x;
    result.elements[_3_1] = translation.y;
    result.elements[_3_2] = translation.z;

    return result;
  }

  Mat4 Mat4::Translate(const float& x, const float& y, const float& z)
  {
    Mat4 result(1.0f);

    result.elements[_3_0] = x;
    result.elements[_3_1] = y;
    result.elements[_3_2] = z;
    return result;
  }

  Mat4 Mat4::Scale(const Vec3<float>& scaling)
  {
    Mat4 result(1.0f);

    result.elements[_0_0] = scaling.x;
    result.elements[_1_1] = scaling.y;
    result.elements[_2_2] = scaling.z;

    return result;
  }

  Mat4 Mat4::Scale(const float& x, const float& y, const float& z)
  {
    Mat4 result(1.0f);

    result.elements[_0_0] = x;
    result.elements[_1_1] = y;
    result.elements[_2_2] = z;

    return result;
  }

  Mat4 Mat4::RotateX(float deg)
  {
    return RotateRX(Math::ToRadians(deg));
  }

  Mat4 Mat4::RotateY(float deg)
  {
    return RotateRY(Math::ToRadians(deg));
  }

  Mat4 Mat4::RotateZ(float deg)
  {
    return RotateRZ(Math::ToRadians(deg));
  }
  Mat4 Mat4::RotateRX(float rad)
  {
    Mat4 result(1.0f);
    float c = cos(rad);
    float s = sin(rad);
    result.elements[_1_1] = c;
    result.elements[_2_1] = -s;
    result.elements[_1_2] = s;
    result.elements[_2_2] = c;
    return result;
  }

  Mat4 Mat4::RotateRY(float rad)
  {
    Mat4 result(1.0f);
    float c = cos(rad);
    float s = sin(rad);
    result.elements[_0_0] = c;
    result.elements[_0_2] = -s;
    result.elements[_2_0] = s;
    result.elements[_2_2] = c;
    return result;
  }
  Mat4 Mat4::RotateRZ(float rad)
  {
    Mat4 result(1.0f);
    float c = cos(rad);
    float s = sin(rad);
    result.elements[_0_0] = c;
    result.elements[_1_0] = -s;
    result.elements[_0_1] = s;
    result.elements[_1_1] = c;
    return result;
  }

  Mat4 Mat4::Rotate(float deg, const Vec3<float>& axis)
  {
    return RotateR(Math::ToRadians(deg), axis);
  }

  Mat4 Mat4::RotateR(float rad, const Vec3<float>& axis)
  {
    Mat4 result(1.0f);

    float c = cos(rad);
    float s = sin(rad);
    float omc = 1.0f - c;

    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    result.elements[_0_0] = x * omc + c;
    result.elements[_0_1] = y * x * omc + z * s;
    result.elements[_0_2] = x * z * omc - y * s;

    result.elements[_1_0] = x * y * omc - z * s;
    result.elements[_1_1] = y * omc + c;
    result.elements[_1_2] = y * z * omc + x * s;

    result.elements[_2_0] = x * z * omc + y * s;
    result.elements[_2_1] = y * z * omc - x * s;
    result.elements[_2_2] = z * omc + c;

    return result;
  }

  Mat4 Mat4::Inverse(const Mat4& inv)
  {
    float temp[FLOATS], det;
    int i;

    temp[0] = inv.elements[5] * inv.elements[10] * inv.elements[15] -
      inv.elements[5] * inv.elements[11] * inv.elements[14] -
      inv.elements[9] * inv.elements[6] * inv.elements[15] +
      inv.elements[9] * inv.elements[7] * inv.elements[14] +
      inv.elements[13] * inv.elements[6] * inv.elements[11] -
      inv.elements[13] * inv.elements[7] * inv.elements[10];

    temp[4] = -inv.elements[4] * inv.elements[10] * inv.elements[15] +
      inv.elements[4] * inv.elements[11] * inv.elements[14] +
      inv.elements[8] * inv.elements[6] * inv.elements[15] -
      inv.elements[8] * inv.elements[7] * inv.elements[14] -
      inv.elements[12] * inv.elements[6] * inv.elements[11] +
      inv.elements[12] * inv.elements[7] * inv.elements[10];

    temp[8] = inv.elements[4] * inv.elements[9] * inv.elements[15] -
      inv.elements[4] * inv.elements[11] * inv.elements[13] -
      inv.elements[8] * inv.elements[5] * inv.elements[15] +
      inv.elements[8] * inv.elements[7] * inv.elements[13] +
      inv.elements[12] * inv.elements[5] * inv.elements[11] -
      inv.elements[12] * inv.elements[7] * inv.elements[9];

    temp[12] = -inv.elements[4] * inv.elements[9] * inv.elements[14] +
      inv.elements[4] * inv.elements[10] * inv.elements[13] +
      inv.elements[8] * inv.elements[5] * inv.elements[14] -
      inv.elements[8] * inv.elements[6] * inv.elements[13] -
      inv.elements[12] * inv.elements[5] * inv.elements[10] +
      inv.elements[12] * inv.elements[6] * inv.elements[9];

    temp[1] = -inv.elements[1] * inv.elements[10] * inv.elements[15] +
      inv.elements[1] * inv.elements[11] * inv.elements[14] +
      inv.elements[9] * inv.elements[2] * inv.elements[15] -
      inv.elements[9] * inv.elements[3] * inv.elements[14] -
      inv.elements[13] * inv.elements[2] * inv.elements[11] +
      inv.elements[13] * inv.elements[3] * inv.elements[10];

    temp[5] = inv.elements[0] * inv.elements[10] * inv.elements[15] -
      inv.elements[0] * inv.elements[11] * inv.elements[14] -
      inv.elements[8] * inv.elements[2] * inv.elements[15] +
      inv.elements[8] * inv.elements[3] * inv.elements[14] +
      inv.elements[12] * inv.elements[2] * inv.elements[11] -
      inv.elements[12] * inv.elements[3] * inv.elements[10];

    temp[9] = -inv.elements[0] * inv.elements[9] * inv.elements[15] +
      inv.elements[0] * inv.elements[11] * inv.elements[13] +
      inv.elements[8] * inv.elements[1] * inv.elements[15] -
      inv.elements[8] * inv.elements[3] * inv.elements[13] -
      inv.elements[12] * inv.elements[1] * inv.elements[11] +
      inv.elements[12] * inv.elements[3] * inv.elements[9];

    temp[13] = inv.elements[0] * inv.elements[9] * inv.elements[14] -
      inv.elements[0] * inv.elements[10] * inv.elements[13] -
      inv.elements[8] * inv.elements[1] * inv.elements[14] +
      inv.elements[8] * inv.elements[2] * inv.elements[13] +
      inv.elements[12] * inv.elements[1] * inv.elements[10] -
      inv.elements[12] * inv.elements[2] * inv.elements[9];

    temp[2] = inv.elements[1] * inv.elements[6] * inv.elements[15] -
      inv.elements[1] * inv.elements[7] * inv.elements[14] -
      inv.elements[5] * inv.elements[2] * inv.elements[15] +
      inv.elements[5] * inv.elements[3] * inv.elements[14] +
      inv.elements[13] * inv.elements[2] * inv.elements[7] -
      inv.elements[13] * inv.elements[3] * inv.elements[6];

    temp[6] = -inv.elements[0] * inv.elements[6] * inv.elements[15] +
      inv.elements[0] * inv.elements[7] * inv.elements[14] +
      inv.elements[4] * inv.elements[2] * inv.elements[15] -
      inv.elements[4] * inv.elements[3] * inv.elements[14] -
      inv.elements[12] * inv.elements[2] * inv.elements[7] +
      inv.elements[12] * inv.elements[3] * inv.elements[6];

    temp[10] = inv.elements[0] * inv.elements[5] * inv.elements[15] -
      inv.elements[0] * inv.elements[7] * inv.elements[13] -
      inv.elements[4] * inv.elements[1] * inv.elements[15] +
      inv.elements[4] * inv.elements[3] * inv.elements[13] +
      inv.elements[12] * inv.elements[1] * inv.elements[7] -
      inv.elements[12] * inv.elements[3] * inv.elements[5];

    temp[14] = -inv.elements[0] * inv.elements[5] * inv.elements[14] +
      inv.elements[0] * inv.elements[6] * inv.elements[13] +
      inv.elements[4] * inv.elements[1] * inv.elements[14] -
      inv.elements[4] * inv.elements[2] * inv.elements[13] -
      inv.elements[12] * inv.elements[1] * inv.elements[6] +
      inv.elements[12] * inv.elements[2] * inv.elements[5];

    temp[3] = -inv.elements[1] * inv.elements[6] * inv.elements[11] +
      inv.elements[1] * inv.elements[7] * inv.elements[10] +
      inv.elements[5] * inv.elements[2] * inv.elements[11] -
      inv.elements[5] * inv.elements[3] * inv.elements[10] -
      inv.elements[9] * inv.elements[2] * inv.elements[7] +
      inv.elements[9] * inv.elements[3] * inv.elements[6];

    temp[7] = inv.elements[0] * inv.elements[6] * inv.elements[11] -
      inv.elements[0] * inv.elements[7] * inv.elements[10] -
      inv.elements[4] * inv.elements[2] * inv.elements[11] +
      inv.elements[4] * inv.elements[3] * inv.elements[10] +
      inv.elements[8] * inv.elements[2] * inv.elements[7] -
      inv.elements[8] * inv.elements[3] * inv.elements[6];

    temp[11] = -inv.elements[0] * inv.elements[5] * inv.elements[11] +
      inv.elements[0] * inv.elements[7] * inv.elements[9] +
      inv.elements[4] * inv.elements[1] * inv.elements[11] -
      inv.elements[4] * inv.elements[3] * inv.elements[9] -
      inv.elements[8] * inv.elements[1] * inv.elements[7] +
      inv.elements[8] * inv.elements[3] * inv.elements[5];

    temp[15] = inv.elements[0] * inv.elements[5] * inv.elements[10] -
      inv.elements[0] * inv.elements[6] * inv.elements[9] -
      inv.elements[4] * inv.elements[1] * inv.elements[10] +
      inv.elements[4] * inv.elements[2] * inv.elements[9] +
      inv.elements[8] * inv.elements[1] * inv.elements[6] -
      inv.elements[8] * inv.elements[2] * inv.elements[5];

    det = inv.elements[0] * temp[0] + inv.elements[1] * temp[4] + inv.elements[2] * temp[8] + inv.elements[3] * temp[12];

    if (det == 0)
      return inv;

    det = 1.0 / det;

    Mat4 result = inv;

    for (i = 0; i < 16; i++)
      result.elements[i] = temp[i] * det;

    return result;
  }

  Mat4 Mat4::Cpy()
  {
    return Mat4(elements);
  }

  Mat4& Mat4::Multiply(const Mat4 &other)
  {
    float data[FLOATS];
    for (int row = 0; row < SIZE; row++)
    {
      for (int col = 0; col < SIZE; col++)
      {
        float sum = 0.0f;
        for (int e = 0; e < SIZE; e++)
        {
          sum += elements[col + e * SIZE] * other.elements[e + row * SIZE];
        }
        data[col + row * SIZE] = sum;
      }
    }
    memcpy(elements, data, FLOATS * sizeof(float));

    return *this;
  }

  Vec3<float> Mat4::Multiply(const Vec3<float> &other) const
  {
    float x = columns[0].x * other.x + columns[1].x * other.y + columns[2].x * other.z + columns[3].x;
    float y = columns[0].y * other.x + columns[1].y * other.y + columns[2].y * other.z + columns[3].y;
    float z = columns[0].z * other.x + columns[1].z * other.y + columns[2].z * other.z + columns[3].z;
    float w = columns[0].w * other.x + columns[1].w * other.y + columns[2].w * other.z + columns[3].w;
    return Vec3(x/w, y/w, z/w);
  }

  Vec4 Mat4::Multiply(const Vec4 &other) const
  {
    float x = columns[0].x * other.x + columns[1].x * other.y + columns[2].x * other.z + columns[3].x * other.w;
    float y = columns[0].y * other.x + columns[1].y * other.y + columns[2].y * other.z + columns[3].y * other.w;
    float z = columns[0].z * other.x + columns[1].z * other.y + columns[2].z * other.z + columns[3].z * other.w;
    float w = columns[0].w * other.x + columns[1].w * other.y + columns[2].w * other.z + columns[3].w * other.w;
    return Vec4(x, y, z, w);
  }

  Mat4 operator*(Mat4 first, const Mat4 &second)
  {
    return first.Multiply(second);
  }

  Mat4& Mat4::operator*=(const Mat4 &other) {
    return Multiply(other);
  }

  Vec3<float> operator*(const Mat4 &first, const Vec3<float> &second)
  {
    return first.Multiply(second);
  }

  Vec4 operator*(const Mat4 &first, const Vec4 &second)
  {
    return first.Multiply(second);
  }

  Mat4 operator~(const Mat4& first)
  {
    return Mat4::Inverse(first);
  }
}
