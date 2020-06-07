#pragma once

namespace Greet {

  template <typename Real>
  struct Vec3;

  class Quaternion
  {
    public:
      float x;
      float y;
      float z;
      float w;

    public:
      Quaternion(float x, float y, float z, float w);

      float Length();
      Quaternion& Normalize();
      Quaternion& Conjugate();

      Quaternion& Multiply(const Quaternion& other);
      Quaternion& Multiply(const Vec3<float>& other);

      friend Quaternion operator*(const Quaternion& first, const Quaternion& second);
      friend Quaternion operator*(const Quaternion& first, const Vec3<float>& second);

      Quaternion& operator*=(const Quaternion &other);
      Quaternion& operator*=(const Vec3<float>  &other);
  };

}
