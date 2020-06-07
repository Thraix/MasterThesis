#include "Mat3.h"

#include <math/MathFunc.h>
#include <graphics/RenderCommand.h>
#include <cstring>

#define _0_0 0
#define _0_1 1
#define _0_2 2
#define _1_0 3
#define _1_1 4
#define _1_2 5
#define _2_0 6
#define _2_1 7
#define _2_2 8
#define SIZE 3
#define FLOATS SIZE*SIZE

namespace Greet{
  Mat3::Mat3()
  {
    for (int i = 0; i < 9; i++)
      elements[i] = 0.0f;
  }

  Mat3::Mat3(float diagonal)
  {
    for (int i = 0; i < 9; i++)
      elements[i] = 0.0f;
    elements[0] = diagonal;
    elements[4] = diagonal;
    elements[8] = diagonal;
  }

  Mat3::Mat3(float* elem)
  {
    for (int i = 0;i < 9;i++)
      elements[i] = elem[i];
  }

  Mat3 Mat3::Identity()
  {
    return Mat3(1.0f);
  }

  Mat3 Mat3::Orthographic(float left, float right, float top, float bottom)
  {
    Mat3 result(1.0f);

    result.elements[0] = 2.0f / (right - left);
    result.elements[4] = 2.0f / (top - bottom);
    result.elements[6] = -(right + left) / (right - left);
    result.elements[7] = -(top + bottom) / (top - bottom);

    return result;
  }

  Mat3 Mat3::OrthographicViewport()
  {
    Mat3 result(1.0f);

    result.elements[0] = 2.0f / (RenderCommand::GetViewportWidth());
    result.elements[4] = 2.0f / (-RenderCommand::GetViewportHeight());
    result.elements[6] = -1;
    result.elements[7] = 1;

    return result;

  }

  Mat3 Mat3::Quad(float x, float y, float width, float height)
  {
    Mat3 result(1.0f);

    result.elements[6] = x;
    result.elements[7] = y;
    result.elements[0] = width;
    result.elements[4] = height;

    return result;
  }

  Mat3 Mat3::Quad(const Vec2& pos, const Vec2& size)
  {
    Mat3 result(1.0f);

    result.elements[6] = pos.x;
    result.elements[7] = pos.y;
    result.elements[0] = size.x;
    result.elements[4] = size.y;

    return result;
  }

  Mat3 Mat3::Translate(const Vec2& translation)
  {
    Mat3 result(1.0f);

    result.elements[6] = translation.x;
    result.elements[7] = translation.y;

    return result;
  }

  Mat3 Mat3::Translate(const float& x, const float& y)
  {
    Mat3 result(1.0f);

    result.elements[6] = x;
    result.elements[7] = y;

    return result;
  }

  Mat3 Mat3::Scale(const Vec2& scaling)
  {
    Mat3 result(1.0f);

    result.elements[0] = scaling.x;
    result.elements[4] = scaling.y;

    return result;
  }

  Mat3 Mat3::Scale(const float& x, const float& y)
  {
    Mat3 result(1.0f);

    result.elements[0] = x;
    result.elements[4] = y;

    return result;
  }

  Mat3 Mat3::Rotate(float deg)
  {
    Mat3 result(1.0f);
    float r = Math::ToRadians(deg);
    float s = sin(r);
    float c = cos(r);

    result.elements[0] = c;
    result.elements[1] = s;
    result.elements[3] = -s;
    result.elements[4] = c;

    return result;
  }

  Mat3 Mat3::RotateR(float rad)
  {
    Mat3 result(1.0f);
    float s = sin(rad);
    float c = cos(rad);

    result.elements[0] = c;
    result.elements[1] = s;
    result.elements[3] = -s;
    result.elements[4] = c;

    return result;
  }

  Mat3 Mat3::Shear(const Vec2& shearing)
  {
    Mat3 result(1.0f);

    result.elements[3] = shearing.x;
    result.elements[1] = shearing.y;

    return result;
  }

  Mat3 Mat3::Shear(const float& x, const float& y)
  {
    Mat3 result(1.0f);

    result.elements[3] = x;
    result.elements[1] = y;

    return result;
  }

  Mat3 Mat3::Inverse(const Mat3& mat)
  {
    float temp[FLOATS], det;

    temp[0] = mat.elements[4] * mat.elements[8] - mat.elements[7] * mat.elements[5];
    temp[1] = mat.elements[7] * mat.elements[2] - mat.elements[1] * mat.elements[8];
    temp[2] = mat.elements[1] * mat.elements[5] - mat.elements[4] * mat.elements[2];
    temp[3] = mat.elements[6] * mat.elements[5] - mat.elements[3] * mat.elements[8];
    temp[4] = mat.elements[0] * mat.elements[8] - mat.elements[6] * mat.elements[2];
    temp[5] = mat.elements[3] * mat.elements[2] - mat.elements[0] * mat.elements[5];
    temp[6] = mat.elements[3] * mat.elements[7] - mat.elements[6] * mat.elements[4];
    temp[7] = mat.elements[6] * mat.elements[1] - mat.elements[0] * mat.elements[7];
    temp[8] = mat.elements[0] * mat.elements[4] - mat.elements[3] * mat.elements[1];

    det = mat.elements[0] * temp[0] + mat.elements[3] * temp[1] + mat.elements[6] * temp[2];

    if (det == 0)
      return mat;

    det = 1.0f / det;

    Mat3 res = mat;

    for (int i = 0; i < 9; i++)
      res.elements[i] = temp[i] * det;

    return res;
  }

  Mat3 Mat3::Cpy()
  {
    return Mat3(elements);
  }

  Mat3& Mat3::Multiply(const Mat3 &other)
  {
    float data[9];
    for (int row = 0; row < 3; row++)
    {
      for (int col = 0; col < 3; col++)
      {
        float sum = 0.0f;
        for (int e = 0; e < 3; e++)
        {
          sum += elements[col + e * 3] * other.elements[e + row * 3];
        }
        data[col + row * 3] = sum;
      }
    }
    memcpy(elements, data, 9*sizeof(float));

    return *this;
  }

  Vec2 Mat3::Multiply(const Vec2 &other) const
  {
    float x = columns[0].x * other.x + columns[1].x * other.y + columns[2].x;
    float y = columns[0].y * other.x + columns[1].y * other.y + columns[2].y;
    float z = columns[0].z * other.x + columns[1].z * other.y + columns[2].z;
    return Vec2(x/z, y/z);
  }

  Vec3<float> Mat3::Multiply(const Vec3<float> &other) const
  {
    float x = columns[0].x * other.x + columns[1].x * other.y + columns[2].x * other.z;
    float y = columns[0].y * other.x + columns[1].y * other.y + columns[2].y * other.z;
    float z = columns[0].z * other.x + columns[1].z * other.y + columns[2].z * other.z;
    return Vec3<float>(x,y,z);
  }

  Mat3 operator*(Mat3 first, const Mat3 &second)
  {
    return first.Multiply(second);
  }

  Mat3& Mat3::operator*=(const Mat3 &other){
    return Multiply(other);
  }

  Vec2 operator*(const Mat3 &first, const Vec2 &second)
  {
    return first.Multiply(second);
  }

  Vec3<float> operator*(const Mat3 &first, const Vec3<float> &second)
  {
    return first.Multiply(second);
  }

  Mat3 operator~(const Mat3& first)
  {
    return Mat3::Inverse(first);
  }
}
