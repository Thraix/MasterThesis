#pragma once

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <iostream>
#include <math/Vec4.h>
#include <math/MathFunc.h>
#include <math/Quaternion.h>
#include <cassert>

namespace Greet{

  template<typename Real>
  struct Vec3
  {
    union
    {
      Real vals[3];
      struct {
        Real x, y, z;
      };
      struct {
        Real r, g, b;
      };
      struct {
        Real h, s, v;
      };
    };
    Vec3()
      : x{}, y{}, z{}
    {}

    Vec3(const Real& x, const Real& y, const Real& z)
      : x{x}, y{y}, z{z}
    {}

    Vec3(const Real& v)
      : x{v}, y{v}, z{v}
    {}

    explicit Vec3(const Vec4& vec4)
      : x{vec4.x}, y{vec4.y}, z{vec4.z}
    {}

    Real Length() const
    {
      return sqrt(x * x + y * y + z * z);
    }

    Real LengthSQ() const
    {
      return x * x + y * y + z  * z;
    }

    Real Dot(const Vec3& vec) const
    {
      return x * vec.x + y * vec.y + z  * vec.z;
    }

    Vec3<Real> Cross(const Vec3<Real>& vec) const
    {
      return Vec3(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
    }

    // All These modifies the current vec3
    Vec3& Normalize()
    {
      float len = Length();
      if(len == 0)
        return *this;
      return Divide(len);
    }

    template <typename std::enable_if<std::is_floating_point<Real>::value>* = nullptr>
    Vec3<Real>& Rotate(const Real& angle, const Vec3& axis)
    {
      Real sh = (Real)sin(Math::ToRadians(angle / 2.0));
      Real ch = (Real)cos(Math::ToRadians(angle / 2.0));

      Real rX = axis.x * sh;
      Real rY = axis.y * sh;
      Real rZ = axis.z * sh;
      Real rW = ch;

      Quaternion rotation(rX,rY,rZ,rW);
      Quaternion conjugate = rotation.Conjugate();

      Quaternion w = rotation * (*this) * conjugate;

      x = w.x;
      y = w.y;
      z = w.z;

      return *this;
    }

    Vec3& Add(const Vec3& other)
    {
      x += other.x;
      y += other.y;
      z += other.z;
      return *this;
    }

    Vec3& Subtract(const Vec3& other)
    {
      x -= other.x;
      y -= other.y;
      z -= other.z;
      return *this;
    }

    Vec3& Multiply(const Vec3& other)
    {
      x *= other.x;
      y *= other.y;
      z *= other.z;
      return *this;
    }

    Vec3& Divide(const Vec3& other)
    {
      x /= other.x;
      y /= other.y;
      z /= other.z;
      return *this;
    }

    Vec3& Add(const Real& other)
    {
      x += other;
      y += other;
      z += other;
      return *this;
    }

    Vec3& Subtract(const Real& other)
    {
      x -= other;
      y -= other;
      z -= other;
      return *this;
    }

    Vec3& Multiply(const Real& other)
    {
      x *= other;
      y *= other;
      z *= other;
      return *this;
    }

    Vec3& Divide(const Real& other)
    {
      x /= other;
      y /= other;
      z /= other;
      return *this;
    }

    bool Compare(const Vec3& other) const
    {
      return x == other.x && y == other.y && z == other.z;
    }

    friend Vec3 operator+(const Vec3& first, const Vec3 &second)
    {
      return Vec3(first).Add(second);
    }

    friend Vec3 operator-(const Vec3& first, const Vec3 &second)
    {
      return Vec3(first).Subtract(second);
    }

    friend Vec3 operator*(const Vec3& first, const Vec3 &second)
    {
      return Vec3(first).Multiply(second);
    }

    friend Vec3 operator/(const Vec3& first, const Vec3 &second)
    {
      return Vec3(first).Divide(second);
    }

    friend Vec3 operator+(const Vec3& vec, const Real& c)
    {
      return Vec3(vec).Add(c);
    }

    friend Vec3 operator-(const Vec3& vec, const Real& c)
    {
      return Vec3(vec).Subtract(c);
    }

    friend Vec3 operator*(const Vec3& vec, const Real& c)
    {
      return Vec3(vec).Multiply(c);
    }

    friend Vec3 operator/(const Vec3& vec, const Real& c)
    {
      return Vec3(vec).Divide(c);
    }

    friend Vec3 operator+(const Real& c, const Vec3& vec)
    {
      return Vec3(vec).Add(c);
    }

    friend Vec3 operator-(const Real& c, const Vec3& vec)
    {
      return Vec3(c - vec.x,c - vec.y,c - vec.z);
    }

    friend Vec3 operator*(const Real& c, const Vec3& vec)
    {
      return Vec3(vec).Multiply(c);
    }

    friend Vec3 operator/(const Real& c, const Vec3& vec)
    {
      return Vec3(c / vec.x, c / vec.y, c / vec.z);
    }

    Real operator[](uint i) const
    {
      assert(i < 3);
      return *((&x)+i);
    }

    Vec3& operator+=(const Vec3 &other)
    {
      return Add(other);
    }

    Vec3& operator-=(const Vec3 &other)
    {
      return Subtract(other);
    }

    Vec3& operator*=(const Vec3 &other)
    {
      return Multiply(other);
    }

    Vec3& operator/=(const Vec3 &other)
    {
      return Divide(other);
    }

    Vec3& operator+=(const Real& c)
    {
      return Add(c);
    }

    Vec3& operator-=(const Real& c)
    {
      return Subtract(c);
    }

    Vec3& operator*=(const Real& c)
    {
      return Multiply(c);
    }

    Vec3& operator/=(const Real& c)
    {
      return Divide(c);
    }

    bool operator!=(const Vec3 &second) const
    {
      return !Compare(second);
    }

    Vec3 operator-() const
    {
      return Vec3(-x, -y, -z);
    }

    bool operator==(const Vec3 &second) const
    {
      return Compare(second);
    }

    bool operator<(const Vec3& rhs) const
    {
      if(x < rhs.x)
        return true;
      else if(x == rhs.x)
      {
        if(y < rhs.y)
          return true;
        else if(y == rhs.y)
          return z < rhs.z;
      }
      return false;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Vec3& vec)
    {
      return stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    }
  };
  namespace Vec
  {
    template <typename Real>
    static Real Dot(const Vec3<Real>& lhs, const Vec3<Real>& rhs)
    {
      return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z  * rhs.z;
    }

    template <typename Real>
    static Vec3<Real> Cross(const Vec3<Real>& lhs, const Vec3<Real>& rhs)
    {
      return Vec3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
    }

    template <typename Real>
    static Vec3<Real> Max(const Vec3<Real>& lhs, const Vec3<Real>& rhs)
    {
      return Vec3<Real>{std::max(lhs.x,rhs.x),std::max(lhs.y,rhs.y),std::max(lhs.z,rhs.z)};
    }

    template <typename Real>
    static Vec3<Real> Min(const Vec3<Real>& lhs, const Vec3<Real>& rhs)
    {
      return Vec3<Real>{std::min(lhs.x,rhs.x),std::min(lhs.y,rhs.y),std::min(lhs.z,rhs.z)};
    }
  }

}
