#include "Vec2.h"

#include <logging/Log.h>
#include <math.h>
#include <cassert>

namespace Greet {

  Vec2::Vec2()
  {
    this->x = 0;
    this->y = 0;
  }

  Vec2::Vec2(float x, float y)
  {
    this->x = x;
    this->y = y;
  }

  Vec2::Vec2(const Vec3<float>& vec3)
    : x(vec3.x), y(vec3.y)
  {

  }

  Vec2::Vec2(const Vec4& vec4)
    : x(vec4.x), y(vec4.y)
  {

  }

  float Vec2::Length()
  {
    return sqrt(x*x + y*y);
  }

  float Vec2::LengthSQ()
  {
    return x*x + y*y;
  }

  float Vec2::Dot(const Vec2& vec) const
  {
    return x*vec.x + y*vec.y;
  }

  float Vec2::Cross(const Vec2& vec) const
  {
    return x * vec.y - y * vec.x;
  }

  Vec2& Vec2::Abs()
  {
    x = x < 0 ? -x : x;
    y = y < 0 ? -y : y;
    return *this;
  }

  Vec2& Vec2::Projected(const Vec2& vec)
  {
    return Multiply(Dot(vec) / LengthSQ());
  }

  Vec2& Vec2::Normalize()
  {
    float len = Length();
    if(len == 0)
      return *this;
    x /= len;
    y /= len;
    return *this;
  }

  Vec2& Vec2::Rotate(const float deg)
  {
    float rad = toRadians(deg);
    float c = cos(rad);
    float s = sin(rad);

    float tempX = x*c - y*s;
    float tempY = x*s + y*c;
    x = tempX;
    y = tempY;

    return *this;
  }

  Vec2& Vec2::RotateR(const float rad)
  {
    float c = cos(rad);
    float s = sin(rad);

    float tempX = x*c - y*s;
    float tempY = x*s + y*c;
    x = tempX;
    y = tempY;

    return *this;
  }




  Vec2& Vec2::Add(const Vec2& other)
  {
    x += other.x;
    y += other.y;
    return *this;
  }

  Vec2& Vec2::Subtract(const Vec2& other)
  {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  Vec2& Vec2::Multiply(const Vec2& other)
  {
    x *= other.x;
    y *= other.y;
    return *this;
  }

  Vec2& Vec2::Divide(const Vec2& other)
  {
    x /= other.x;
    y /= other.y;
    return *this;
  }

  Vec2& Vec2::Add(const float c)
  {
    x += c;
    y += c;
    return *this;
  }

  Vec2& Vec2::Subtract(const float c)
  {
    x -= c;
    y -= c;
    return *this;
  }

  Vec2& Vec2::Multiply(const float c)
  {
    x *= c;
    y *= c;
    return *this;
  }

  Vec2& Vec2::Divide(const float c)
  {
    x /= c;
    y /= c;
    return *this;
  }

  bool Vec2::Compare(const Vec2& other)
  {
    return x == other.x && y == other.y;
  }

  Vec2 operator+(const Vec2& first, const Vec2 &second)
  {
    return Vec2(first.x,first.y).Add(second);
  }

  Vec2 operator-(const Vec2& first, const Vec2 &second)
  {
    return Vec2(first.x, first.y).Subtract(second);
  }

  Vec2 operator*(const Vec2& first, const Vec2 &second)
  {
    return Vec2(first.x, first.y).Multiply(second);
  }

  Vec2 operator/(const Vec2& first, const Vec2 &second)
  {
    return Vec2(first.x, first.y).Divide(second);
  }

  Vec2 operator+(const Vec2& first, const float c)
  {
    return Vec2(first.x, first.y).Add(c);
  }

  Vec2 operator-(const Vec2& first, const float c)
  {
    return Vec2(first.x, first.y).Subtract(c);
  }

  Vec2 operator*(const Vec2&  first, const float c)
  {
    return Vec2(first.x, first.y).Multiply(c);
  }

  Vec2 operator/(const Vec2& first, const float c)
  {
    return Vec2(first.x, first.y).Divide(c);
  }

  Vec2 operator-(const Vec2& first)
  {
    return Vec2(-first.x, -first.y);
  }

  float Vec2::operator[](uint i)
  {
    assert(i < 2);
    return *((&x)+i);
  }

  Vec2& Vec2::operator+=(const Vec2 &other)
  {
    return Add(other);
  }

  Vec2& Vec2::operator-=(const Vec2 &other)
  {
    return Subtract(other);
  }

  Vec2& Vec2::operator*=(const Vec2 &other)
  {
    return Multiply(other);
  }

  Vec2& Vec2::operator/=(const Vec2 &other)
  {
    return Divide(other);
  }

  Vec2& Vec2::operator+=(const float other)
  {
    x += other;
    y += other;
    return *this;
  }

  Vec2& Vec2::operator-=(const float other)
  {
    x -= other;
    y -= other;
    return *this;
  }

  Vec2& Vec2::operator*=(const float other)
  {
    x *= other;
    y *= other;
    return *this;
  }

  Vec2& Vec2::operator/=(const float other)
  {
    x /= other;
    y /= other;
    return *this;
  }

  bool Vec2::operator==(const Vec2 &other)
  {
    return Compare(other);
  }

  bool Vec2::operator!=(const Vec2 &other)
  {
    return !Compare(other);
  }

  bool Vec2::operator<(const Vec2& other)
  {
    if(x == other.x)
      return y < other.y;
    return x < other.x;
  }

  bool Vec2::operator>(const Vec2& other)
  {
    if(x == other.x)
      return y > other.y;
    return x > other.x;
  }

  bool Vec2::operator<=(const Vec2& other)
  {
    if(x == other.x)
      return y <= other.y;
    return x <= other.x;
  }

  bool Vec2::operator>=(const Vec2& other)
  {
    if(x == other.x)
      return y >= other.y;
    return x >= other.x;
  }

  std::ostream& operator<<(std::ostream& stream, const Vec2& vec)
  {
    return stream << "(" << vec.x << ", " << vec.y << ")";
  }
}
