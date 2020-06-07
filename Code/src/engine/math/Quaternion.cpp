#include "Quaternion.h"

#include "MathFunc.h"
#include "Vec3.h"


namespace Greet {

  Quaternion::Quaternion(float x, float y, float z, float w) :x(x), y(y), z(z), w(w) {}

  float Quaternion::Length()
  {
    return (float)sqrt(x*x + y*y + z*z + w*w);
  }

  Quaternion& Quaternion::Normalize()
  {
    float len = Length();
    x /= len;
    y /= len;
    z /= len;
    w /= len;
    return *this;
  }

  Quaternion& Quaternion::Conjugate()
  {
    x = -x;
    y = -y;
    z = -z;
    return *this;
  }

  Quaternion& Quaternion::Multiply(const Quaternion& other)
  {
    float w_ = w  * other.w - x  * other.x - y  * other.y - z  * other.z;
    float x_ = x  * other.w + w  * other.x + y  * other.z - z  * other.y;
    float y_ = y  * other.w + w  * other.y + z  * other.x - x  * other.z;
    float z_ = z  * other.w + w  * other.z + x  * other.y - y  * other.x;
    x = x_;
    y = y_;
    z = z_;
    w = w_;
    return *this;
  }

  Quaternion& Quaternion::Multiply(const Vec3<float>& other)
  {
    float w_ = -x * other.x - y * other.y - z * other.z;
    float x_ =  w * other.x + y * other.z - z * other.y;
    float y_ =  w * other.y + z * other.x - x * other.z;
    float z_ =  w * other.z + x * other.y - y * other.x;
    x = x_;
    y = y_;
    z = z_;
    w = w_;
    return *this;
  }

  Quaternion operator*(const Quaternion& first, const Quaternion &second)
  {
    return Quaternion(first.x,first.y,first.z,first.w).Multiply(second);
  }

  Quaternion operator*(const Quaternion& first, const Vec3<float>& second)
  {
    return Quaternion(first.x, first.y, first.z, first.w).Multiply(second);
  }

  Quaternion& Quaternion::operator*=(const Quaternion &other)
  {
    return Multiply(other);
  }

  Quaternion& Quaternion::operator*=(const Vec3<float>& other)
  {
    return Multiply(other);
  }

}
