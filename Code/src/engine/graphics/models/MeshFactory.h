#pragma once

#include <graphics/models/MeshData.h>
#include <math/Maths.h>

namespace Greet {
  struct MeshFactory
  {
    enum PolygonSizeFormat
    {
      SIDE_LENGTH, INSCRIBED_RADIUS, CIRCUMSCRIBED_RADIUS
    };

    static Vec3<float> CalculateNormal(const Vec3<float>& p1, const Vec3<float>& p2, const Vec3<float>& p3);
    static void CalculateNormals(const Pointer<Vec3<float>>& vertices, const Pointer<uint>& indices, Pointer<Vec3<float>>& normals);
    static Pointer<Vec3<float>> CalculateNormals(const Pointer<Vec3<float>> vertices, const Pointer<uint>& indices);
    static MeshData Polygon(uint count, float size, PolygonSizeFormat format);
    static MeshData Quad(float x, float y, float z, float width, float length);
    static MeshData Cube(float x, float y, float z, float width, float height, float length);
    // This Cube generator works better for voxel rendering since the normals work better.
    static MeshData Cube2(float x, float y, float z, float width, float height, float length);
    static MeshData Tetrahedron(float x, float y, float z, float size);
    static MeshData Grid(float x, float y, float z, float width, float length, uint gridWidth, uint gridLength, const Pointer<float>& heightMap, float height);
    static uint IndexGrid(uint x, uint z, uint gridWidth, uint gridLength);
    static MeshData LowPolyGrid(float x, float y, float z, float width, float length, uint gridWidth, uint gridLength, const Pointer<float>& heightMap, float height);
    static MeshData Grid(float x, float y, float z, float width, float length, uint gridWidth, uint gridLength);
    static MeshData Sphere(const Vec3<float>& position, float radius, uint latitudes, uint longitudes);
  };
}
