#pragma once

struct Mat4
{
  float elements[16];

  __device__ float operator[](int i)
  {
    return elements[i];
  }
};

__device__ float3 operator+(float3 v1, float3 v2)
{
  return make_float3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

__device__ float3 operator+(float3 v, float f)
{
  return make_float3(v.x + f, v.y + f, v.z + f);
}

__device__ float3 operator-(float3 v1, float3 v2)
{
  return make_float3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

__device__ float3 operator*(float3 v1, float3 v2)
{
  return make_float3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

__device__ float3 operator*(float3 v, float f)
{
  return make_float3(v.x * f, v.y * f, v.z * f);
}
__device__ float3 operator-(float3 v)
{
  return make_float3(-v.x, -v.y, -v.z);
}

__device__ float3 operator/(float3 v1, float3 v2)
{
  return make_float3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

__device__ float3 operator/(float3 v, float f)
{
  return make_float3(v.x / f, v.y / f, v.z / f);
}

__device__ float3 operator/(float f, float3 v)
{
  return make_float3(f / v.x, f / v.y, f / v.z);
}
__device__ bool operator!=(float3 v1, float3 v2)
{
  return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z;
}

__device__ bool operator!=(int3 v1, int3 v2)
{
  return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z;
}

__device__ bool operator==(float3 v1, float3 v2)
{
  return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}

__device__ bool operator==(int3 v1, int3 v2)
{
  return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}

__device__ void operator-=(float3& v, float f)
{
  v.x -= f;
  v.y -= f;
  v.z -= f;
}

__device__ float3 operator*(Mat4& m, float3 v)
{
  float x = m[0] * v.x + m[4] * v.y + m[8]  * v.z + m[12];
  float y = m[1] * v.x + m[5] * v.y + m[9]  * v.z + m[13];
  float z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14];
  float w = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15];
  return make_float3(x, y, z);
}

__device__ float get(float3 v, int index)
{
  float* ptr = (float*)&v;
  return *(ptr + index);
}

__device__ void set(float3& v, int index, float value)
{
  float* ptr = (float*)&v;
  *(ptr + index) = value;
}

__device__ float sign(float f)
{
  return f < 0.0f ? -1.0f : (f > 0.0f ? 1.0f : 0.0f);
}

__device__ float3 sign(float3 v)
{
  return make_float3(sign(v.x), sign(v.y), sign(v.z));
}

__device__ float min(float3 v)
{
  return min(v.x, min(v.y, v.z));
}

__device__ float max(float3 v)
{
  return max(v.x, max(v.y, v.z));
}

__device__ float3 abs(float3 v)
{
  return make_float3(abs(v.x), abs(v.y), abs(v.z));
}

__device__ int3 toInt3(float3 v)
{
  return make_int3(floor(v.x), floor(v.y), floor(v.z));
}

__device__ float3 toFloat3(int3 v)
{
  return make_float3(float(v.x), float(v.y), float(v.z));
}

__device__ float3 cross(float3 v1, float3 v2)
{
  return make_float3(
      v1.y * v2.z - v1.z * v2.y,
      v1.z * v2.x - v1.x * v2.z,
      v1.x * v2.y - v1.y * v2.x);
}

__device__ float dot(float3 v1, float3 v2)
{
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

__device__ float length(float3 v)
{
  return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

__device__ float3 normalize(float3 v)
{
  return v / length(v);
}

__device__ void swap(float3& v1, float3& v2)
{
  float3 tmp = v1;
  v1 = v2;
  v2 = tmp;
}

__device__
int2 operator*(int2 v1, int2 v2)
{
  return make_int2(v1.x * v2.x, v1.y * v2.y);
}

__device__
int3 operator+(int3 v1, int3 v2)
{
  return make_int3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

__device__
int3 operator-(int3 v1, int3 v2)
{
  return make_int3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

__device__
int3 operator*(int3 v1, int3 v2)
{
  return make_int3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

__device__
int3 operator*(int3 v, int i)
{
  return make_int3(v.x * i, v.y * i, v.z * i);
}

__device__
void operator-=(int3& v, int i)
{
  v.x -= i;
  v.y -= i;
  v.z -= i;
}

__device__ int& get(int3& v, int index)
{
  return *((int*)&v + index);
}

__device__ void set(int3& v, int index, int value)
{
  int* ptr = ((int*)&v + index);
  *ptr = value;
}


__device__ 
int2 toInt2(int3 v, int axis1, int axis2)
{
  return make_int2(get(v, axis1), get(v, axis2));
}

__device__
int min(int3 v)
{
  return min(v.x, min(v.y, v.z));
}

__device__
int sign(int i)
{
  return i < 0 ? -1 : (i > 0 ? 1 : 0);
}

__device__
int3 sign(int3 v)
{
  return make_int3(sign(v.x), sign(v.y), sign(v.z));
}

__device__
int3 abs(int3 v)
{
  return make_int3(abs(v.x), abs(v.y), abs(v.z));
}

__device__
int dot(int2 v1, int2 v2)
{
  return v1.x * v2.x + v1.y * v2.y;
}

__device__
int3 cross(int3 v1, int3 v2)
{
  return make_int3(
      v1.y * v2.z - v1.z * v2.y,
      v1.z * v2.x - v1.x * v2.z,
      v1.x * v2.y - v1.y * v2.x);
}

__device__ int max(int3 v)
{
  return max(v.x, max(v.y, v.z));
}

__device__
int sum(int2 v)
{
  return v.x + v.y;
}

__device__ 
void swap(int3& v1, int3& v2)
{
  int3 tmp = v1;
  v1 = v2;
  v2 = tmp;
}

__device__ float2 operator+(float2 v1, float2 v2)
{
  return make_float2(v1.x + v2.x, v1.y + v2.y);
}

__device__ float2 operator-(float2 v1, float2 v2)
{
  return make_float2(v1.x - v2.x, v1.y - v2.y);
}

__device__ float2 operator*(float2 v, float f)
{
  return make_float2(v.x * f, v.y * f);
}

__device__ float2 operator/(float2 v, float f)
{
  return make_float2(v.x / f, v.y / f);
}

__device__ float& get(float2& v, int index)
{
  float* ptr = (float*)&v;
  return *(ptr + index);
}

__device__ float length(float2 v)
{
  return sqrt(v.x * v.x + v.y * v.y);
}

__device__ float2 toFloat2(float3 v, int axis1, int axis2)
{
  return make_float2(get(v, axis1), get(v, axis2));
}

__device__ float2 normalize(float2 v)
{
  return v / length(v);
}

__device__ float dot(float2 v1, float2 v2)
{
  return v1.x * v2.x + v1.y * v2.y;
}
