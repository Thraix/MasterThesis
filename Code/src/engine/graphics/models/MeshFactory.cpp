#include "MeshFactory.h"
#include <logging/Log.h>
namespace Greet {

  // Calculate a normal of 3 points in space
  Vec3<float> MeshFactory::CalculateNormal(const Vec3<float>& p1, const Vec3<float>& p2, const Vec3<float>& p3)
  {
    Vec3<float> v1 = p2 - p1;
    Vec3<float> v2 = p3 - p1;
    Vec3<float> normal = v1.Cross(v2);
    return normal;
  }

  void MeshFactory::CalculateNormals(const Pointer<Vec3<float>>& vertices, const Pointer<uint>& indices, Pointer<Vec3<float>>& normals)
  {
    Vec3<float> faceNormal;
    Vec3<float> p1, p2, p3;
    memset(normals.Data(), 0, sizeof(float) * 3 * vertices.Size());
    for (uint i = 0; i < indices.Size(); i += 3)
    {
      if(indices[i] >= vertices.Size() ||
          indices[i+1] >= vertices.Size() ||
          indices[i+2] >= vertices.Size())
      {
        Log::Error("Index out of bound in CalculateNormals");
        continue;
      }
      p1 = vertices[indices[i]];
      p2 = vertices[indices[i + 1]];
      p3 = vertices[indices[i + 2]];
      faceNormal = CalculateNormal(p1, p2, p3);
      normals[indices[i]] += faceNormal;
      normals[indices[i + 1]] += faceNormal;
      normals[indices[i + 2]] += faceNormal;
    }
    for (uint i = 0; i < vertices.Size(); i++)
    {
      normals[i].Normalize();
    }
  }

  // Calculate normals for a mesh
  Pointer<Vec3<float>> MeshFactory::CalculateNormals(const Pointer<Vec3<float>> vertices, const Pointer<uint>& indices)
  {
    Pointer<Vec3<float>> normals(vertices.Size());
    CalculateNormals(vertices, indices, normals);
    return normals;
  }

  MeshData MeshFactory::Polygon(uint count, float Size, PolygonSizeFormat format)
  {
    if(count < 3)
    {
      Log::Warning("Invalid Polygon corners (returning square): ", count);
      count = 4;
    }
    uint indexCount = 3 * (count - 2);
    Pointer<Vec3<float>> vertices = Pointer<Vec3<float>>(count);
    Pointer<Vec3<float>> normals(count);
    Pointer<uint> indices = Pointer<uint>(indexCount);
    float radius = 0;

    // 180 * (count - 2) / count
    float cornerAngle = M_PI * (1 - 2 / (float)count);

    switch(format)
    {
      case SIDE_LENGTH:
        radius = Size / (2 * cos(cornerAngle/2));
        break;
      case CIRCUMSCRIBED_RADIUS:
        radius = Size;
        break;
      case INSCRIBED_RADIUS:
        radius = Size / sin(cornerAngle/2);
        break;
    }
    float angle = 2 * M_PI / (float)count;
    for(uint i = 0; i < count;i++)
    {
      vertices[i] = Vec3<float>(cos(angle/2 + angle*i), 0, sin(angle/2 + angle*i)) * radius;
      normals[i] = Vec3<float>(0,1,0);
    }

    for(uint i = 0;i < count - 2; i++)
    {
      indices[i*3 + 0] = 0;
      indices[i*3 + 1] = 2 + i;
      indices[i*3 + 2] = 1 + i;
    }

    MeshData data(vertices, indices);
    data.AddAttribute({MESH_NORMALS_LOCATION, BufferAttributeType::VEC3}, normals);
    return data;
  }

  MeshData MeshFactory::Quad(float x, float y, float z, float width, float length)
  {
    Pointer<Vec3<float>> vertices = Pointer<Vec3<float>>(4);
    float halfWidth = width / 2.0f;
    float halfLength = length / 2.0f;
    vertices[0]  = Vec3<float>(x-halfWidth, y, z-halfLength);
    vertices[1]  = Vec3<float>(x+halfWidth, y, z-halfLength);
    vertices[2]  = Vec3<float>(x+halfWidth, y, z+halfLength);
    vertices[3]  = Vec3<float>(x-halfWidth, y, z+halfLength);

    Pointer<Vec3<float>> normals = Pointer<Vec3<float>>(4);
    normals[0]  = Vec3<float>(0.0f, 1.0f, 0.0f);
    normals[1]  = Vec3<float>(0.0f, 1.0f, 0.0f);
    normals[2]  = Vec3<float>(0.0f, 1.0f, 0.0f);
    normals[3]  = Vec3<float>(0.0f, 1.0f, 0.0f);

    Pointer<uint> indices{0,2,1,0,3,2};
    MeshData meshdata(vertices,indices);
    meshdata.AddAttribute({MESH_NORMALS_LOCATION, BufferAttributeType::VEC3},normals);
    return meshdata;
  }
  // x, y, z
  MeshData MeshFactory::Cube(float x, float y, float z, float width, float height, float length)
  {
    Pointer<Vec3<float>> vertices = Pointer<Vec3<float>>(8);
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;
    float halfLength = length / 2.0f;

    vertices[0]  = Vec3<float>(x-halfWidth, y-halfHeight, z-halfLength);
    vertices[1]  = Vec3<float>(x-halfWidth, y+halfHeight, z-halfLength);
    vertices[2]  = Vec3<float>(x-halfWidth, y+halfHeight, z+halfLength);
    vertices[3]  = Vec3<float>(x-halfWidth, y-halfHeight, z+halfLength);

    vertices[4]  = Vec3<float>(x+halfWidth, y-halfHeight, z-halfLength);
    vertices[5]  = Vec3<float>(x+halfWidth, y+halfHeight, z-halfLength);
    vertices[6]  = Vec3<float>(x+halfWidth, y+halfHeight, z+halfLength);
    vertices[7]  = Vec3<float>(x+halfWidth, y-halfHeight, z+halfLength);

    Pointer<Vec3<float>> normals(8);
    normals[0] = Vec3<float>(-1.0f, -1.0f, -1.0f);
    normals[1] = Vec3<float>(-1.0f,  1.0f, -1.0f);
    normals[2] = Vec3<float>(-1.0f,  1.0f,  1.0f);
    normals[3] = Vec3<float>(-1.0f, -1.0f,  1.0f);
    normals[4] = Vec3<float>( 1.0f, -1.0f, -1.0f);
    normals[5] = Vec3<float>( 1.0f,  1.0f, -1.0f);
    normals[6] = Vec3<float>( 1.0f,  1.0f,  1.0f);
    normals[7] = Vec3<float>( 1.0f, -1.0f,  1.0f);

    Pointer<uint> indices{
      0, 2, 1, 0, 3, 2,
      3, 6, 2, 3, 7, 6,
      7, 5, 6, 7, 4, 5,
      4, 1, 5, 4, 0, 1,
      2, 5, 1, 2, 6, 5,
      3, 0, 4, 3, 4, 7 };

    MeshData meshdata(vertices,indices);
    meshdata.AddAttribute({MESH_NORMALS_LOCATION, BufferAttributeType::VEC3}, normals);
    return meshdata;
  }

  // x, y, z
  MeshData MeshFactory::Cube2(float x, float y, float z, float width, float height, float length)
  {
    Pointer<Vec3<float>> vertices = Pointer<Vec3<float>>(24);
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;
    float halfLength = length / 2.0f;


    vertices[0] = Vec3<float>(x - halfWidth, y - halfHeight, z - halfLength);
    vertices[1] = Vec3<float>(x - halfWidth, y + halfHeight, z - halfLength);
    vertices[2] = Vec3<float>(x - halfWidth, y + halfHeight, z + halfLength);
    vertices[3] = Vec3<float>(x - halfWidth, y - halfHeight, z + halfLength);

    vertices[4] = Vec3<float>(x + halfWidth, y - halfHeight, z - halfLength);
    vertices[5] = Vec3<float>(x + halfWidth, y + halfHeight, z - halfLength);
    vertices[6] = Vec3<float>(x + halfWidth, y + halfHeight, z + halfLength);
    vertices[7] = Vec3<float>(x + halfWidth, y - halfHeight, z + halfLength);

    vertices[8] = Vec3<float>(x - halfWidth, y - halfHeight, z - halfLength);
    vertices[9] = Vec3<float>(x - halfWidth, y - halfHeight, z + halfLength);
    vertices[10] = Vec3<float>(x + halfWidth, y - halfHeight, z + halfLength);
    vertices[11] = Vec3<float>(x + halfWidth, y - halfHeight, z - halfLength);

    vertices[12] = Vec3<float>(x - halfWidth, y + halfHeight, z - halfLength);
    vertices[13] = Vec3<float>(x - halfWidth, y + halfHeight, z + halfLength);
    vertices[14] = Vec3<float>(x + halfWidth, y + halfHeight, z + halfLength);
    vertices[15] = Vec3<float>(x + halfWidth, y + halfHeight, z - halfLength);


    vertices[16] = Vec3<float>(x - halfWidth, y - halfHeight, z - halfLength);
    vertices[17] = Vec3<float>(x + halfWidth, y - halfHeight, z - halfLength);
    vertices[18] = Vec3<float>(x + halfWidth, y + halfHeight, z - halfLength);
    vertices[19] = Vec3<float>(x - halfWidth, y + halfHeight, z - halfLength);

    vertices[20] = Vec3<float>(x - halfWidth, y - halfHeight, z + halfLength);
    vertices[21] = Vec3<float>(x + halfWidth, y - halfHeight, z + halfLength);
    vertices[22] = Vec3<float>(x + halfWidth, y + halfHeight, z + halfLength);
    vertices[23] = Vec3<float>(x - halfWidth, y + halfHeight, z + halfLength);

    Pointer<Vec3<float>> normals = Pointer<Vec3<float>>(24);
    normals[0] = Vec3<float>(-1.0f, 0.0f, 0.0f);
    normals[1] = Vec3<float>(-1.0f, 0.0f, 0.0f);
    normals[2] = Vec3<float>(-1.0f, 0.0f, 0.0f);
    normals[3] = Vec3<float>(-1.0f, 0.0f, 0.0f);

    normals[4] = Vec3<float>(1.0f, 0.0f, 0.0f);
    normals[5] = Vec3<float>(1.0f, 0.0f, 0.0f);
    normals[6] = Vec3<float>(1.0f, 0.0f, 0.0f);
    normals[7] = Vec3<float>(1.0f, 0.0f, 0.0f);

    normals[8] = Vec3<float>(0.0f, -1.0f, 0.0f);
    normals[9] = Vec3<float>(0.0f, -1.0f, 0.0f);
    normals[10] = Vec3<float>(0.0f, -1.0f, 0.0f);
    normals[11] = Vec3<float>(0.0f, -1.0f, 0.0f);

    normals[12] = Vec3<float>(0.0f, 1.0f, 0.0f);
    normals[13] = Vec3<float>(0.0f, 1.0f, 0.0f);
    normals[14] = Vec3<float>(0.0f, 1.0f, 0.0f);
    normals[15] = Vec3<float>(0.0f, 1.0f, 0.0f);

    normals[16] = Vec3<float>(0.0f, 0.0f, -1.0f);
    normals[17] = Vec3<float>(0.0f, 0.0f, -1.0f);
    normals[18] = Vec3<float>(0.0f, 0.0f, -1.0f);
    normals[19] = Vec3<float>(0.0f, 0.0f, -1.0f);

    normals[20] = Vec3<float>(0.0f, 0.0f, 1.0f);
    normals[21] = Vec3<float>(0.0f, 0.0f, 1.0f);
    normals[22] = Vec3<float>(0.0f, 0.0f, 1.0f);
    normals[23] = Vec3<float>(0.0f, 0.0f, 1.0f);


    Pointer<uint> indices{
      0, 2, 1, 0, 3, 2,
        4, 5, 6, 4, 6, 7,
        8, 10, 9, 8, 11, 10,
        12, 13, 14, 12, 14, 15,
        16, 18, 17, 16, 19, 18,
        20, 21, 22, 20, 22, 23 };

    MeshData meshdata(vertices, indices);
    meshdata.AddAttribute({MESH_NORMALS_LOCATION, BufferAttributeType::VEC3},normals);
    return meshdata;
  }

  MeshData MeshFactory::Tetrahedron(float x, float y, float z, float Size)
  {
    float tan30 = 0.5773502f;  //   tan(30/180*PI)
    float cos30 = 1.1547005f; //   1 / cos(30/180*PI)
    float halfSize = Size * 0.5f;
    float l1 = halfSize*cos30;
    float l2 = halfSize*tan30; //
    float height = Size*0.8164965f;// Height of tetrahedron
    float corner = Size*0.4082483f; // Size/sqrt(6) -> Center to corner
    float face = height - corner; // Center to face

    Vec3<float> v1 = Vec3<float>(x, y-face, z + l1);
    Vec3<float> v2 = Vec3<float>(x + halfSize, y - face, z - l2);
    Vec3<float> v3 = Vec3<float>(x - halfSize, y - face, z - l2);
    Vec3<float> v4 = Vec3<float>(x, y + corner, z);


    Pointer<Vec3<float>> vertices = Pointer<Vec3<float>>(4);
    vertices[0] = v1;
    vertices[1] = v2;
    vertices[2] = v3;
    vertices[3] = v4;


    Pointer<Vec3<float>> normals = Pointer<Vec3<float>>(4);

    normals[0] = (v1 - Vec3<float>(x, y, z)).Normalize();
    normals[1] = (v2 - Vec3<float>(x, y, z)).Normalize();
    normals[2] = (v3 - Vec3<float>(x, y, z)).Normalize();
    normals[3] = (v4 - Vec3<float>(x, y, z)).Normalize();

    Pointer<uint> indices{ 0, 2, 1, 1, 3, 0, 2, 0, 3, 1, 2, 3};
    MeshData meshdata(vertices, indices);
    meshdata.AddAttribute({MESH_NORMALS_LOCATION, BufferAttributeType::VEC3},normals);
    return meshdata;
  }

  MeshData MeshFactory::Grid(float x, float y, float z, float width, float length, uint gridWidth, uint gridLength)
  {
    return Grid(x,y,z,width,length,gridWidth,gridLength,{},0);
  }

  MeshData MeshFactory::Grid(float x, float y, float z, float width, float length, uint gridWidth, uint gridLength, const Pointer<float>& heightMap, float height)
  {
    if (gridWidth < 1 || gridLength < 1)
      return Quad(x, y, z, width, length);

    float tileWidth = width / (float)gridWidth;
    float tileLength = length / (float)gridLength;

    uint vertexCount = (gridWidth + 1) * (gridLength+ 1);

    Pointer<Vec3<float>> vertices(vertexCount);

    // decenter x and y
    x -= width / 2.0f;
    z -= length / 2.0f;


    for (uint iz = 0; iz <= gridLength; iz++)
    {
      for (uint ix = 0; ix <= gridWidth; ix++)
      {
        float heightM = heightMap.Size() == 0 ? 0 : heightMap[ix + iz*(gridWidth + 1)];
        vertices[ix + iz*(gridWidth + 1)] = Vec3<float>(x + ix*tileWidth, y + heightM*height, z + iz*tileLength);
      }
    }

    uint indexCount = 6 * gridWidth * gridLength;
    Pointer<uint> indices(indexCount);

    // Loop through every square
    uint i = 0;
    for (uint iz = 0; iz < gridLength; iz++)
    {
      for (uint ix = 0; ix < gridWidth; ix++)
      {
        if (ix % 2 == iz % 2)
        {
          indices[i++] = ix + iz * (gridWidth + 1);
          indices[i++] = ix + (iz + 1) * (gridWidth + 1);
          indices[i++] = ix + 1 + (iz + 1) * (gridWidth + 1);
          indices[i++] = ix + 1 + iz * (gridWidth + 1);
          indices[i++] = ix + iz * (gridWidth + 1);
          indices[i++] = ix + 1 + (iz + 1) * (gridWidth + 1);
        }
        else
        {
          indices[i++] = ix + iz * (gridWidth + 1);
          indices[i++] = ix + (iz + 1) * (gridWidth + 1);
          indices[i++] = ix + 1 + iz * (gridWidth + 1);
          indices[i++] = ix + 1 + iz * (gridWidth + 1);
          indices[i++] = ix + (iz + 1) * (gridWidth + 1);
          indices[i++] = ix + 1 + (iz + 1) * (gridWidth + 1);
        }
      }
    }

    Pointer<Vec3<float>> normals = CalculateNormals(vertices, indices);
    MeshData data(vertices, indices);
    data.AddAttribute({MESH_NORMALS_LOCATION, BufferAttributeType::VEC3},normals);

    return data;
  }

  uint MeshFactory::IndexGrid(uint x, uint z, uint gridWidth, uint gridLength)
  {
    if (z >= gridLength - 1)
    {
      return x + (z - (gridLength - 1)) * (gridWidth+1) + (gridLength - 1) * gridWidth * 2;
    }
    else if (x == 0)
    {
      return z * 2 * gridWidth;
    }
    else if (x == gridWidth)
    {
      return x * 2 - 1 + z * 2 * gridWidth;
    }
    else
    {
      return x * 2 - 1 + z * 2 * gridWidth;
    }
    return 0;
  }

  MeshData MeshFactory::LowPolyGrid(float x, float y, float z, float width, float length, uint gridWidth, uint gridLength, const Pointer<float>& heightMap, float height)
  {
    if (gridWidth < 1 || gridLength < 1)
      return Quad(x, y, z, width, length);

    float tileWidth = width / (float)gridWidth;
    float tileLength = length / (float)gridLength;

    uint vertexCount = (gridWidth + 1) * (gridLength + 1) + (gridWidth - 1)*(gridLength - 1);

    Pointer<Vec3<float>> vertices = Pointer<Vec3<float>>(vertexCount);

    // decenter x and y
    x -= width / 2.0f;
    z -= length / 2.0f;

    for (uint iz = 0; iz <= gridLength; iz++)
    {
      for (uint ix = 0; ix <= gridWidth; ix++)
      {
        float heightM = heightMap.Size() == 0 ? 0 : heightMap[ix + iz*(gridWidth + 1)];
        Vec3<float> vec = Vec3<float>(x + ix*tileWidth, y + heightM*height, z + iz*tileLength);
        uint index = IndexGrid(ix,iz,gridWidth, gridLength);
        vertices[index] = vec;
        if(iz < gridLength - 1 && ix != 0 && ix != gridWidth)
        {
          vertices[index + 1] = vec;
        }
      }
    }

    uint indexCount = 6 * gridWidth * gridLength;
    Pointer<uint> indices = Pointer<uint>(indexCount);

    // Loop through every square
    uint i = 0;
    for (uint iz = 0; iz < gridLength; iz++)
    {
      for (uint ix = 0; ix < gridWidth; ix++)
      {
        if (iz < gridLength - 1)
        {
          uint add = (ix != 0) ? 1 : 0;
          if (ix % 2 == iz % 2)
          {
            indices[i++] = IndexGrid(ix, iz,gridWidth,gridLength) + add;
            indices[i++] = IndexGrid(ix, iz + 1, gridWidth, gridLength);
            indices[i++] = IndexGrid(ix + 1, iz + 1, gridWidth, gridLength);
            indices[i++] = IndexGrid(ix + 1, iz, gridWidth, gridLength);
            indices[i++] = IndexGrid(ix, iz, gridWidth, gridLength);
            indices[i++] = IndexGrid(ix + 1, iz + 1, gridWidth, gridLength);
          }
          else
          {
            indices[i++] = IndexGrid(ix, iz, gridWidth, gridLength) + add;
            indices[i++] = IndexGrid(ix, iz + 1, gridWidth, gridLength);
            indices[i++] = IndexGrid(ix + 1, iz, gridWidth, gridLength);
            indices[i++] = IndexGrid(ix + 1, iz, gridWidth, gridLength);
            indices[i++] = IndexGrid(ix, iz + 1, gridWidth, gridLength);
            indices[i++] = IndexGrid(ix + 1, iz + 1, gridWidth, gridLength);
          }
        }
        else
        {
          if (ix % 2 == iz % 2)
          {
            indices[i++] = IndexGrid(ix, iz, gridWidth, gridLength);
            indices[i++] = IndexGrid(ix, iz + 1, gridWidth, gridLength);
            indices[i++] = IndexGrid(ix + 1, iz + 1, gridWidth, gridLength);
            indices[i++] = IndexGrid(ix + 1, iz + 1, gridWidth, gridLength);
            indices[i++] = IndexGrid(ix + 1, iz, gridWidth, gridLength);
            indices[i++] = IndexGrid(ix, iz, gridWidth, gridLength);
          }
          else
          {
            indices[i++] = IndexGrid(ix, iz, gridWidth, gridLength);
            indices[i++] = IndexGrid(ix, iz + 1, gridWidth, gridLength);
            indices[i++] = IndexGrid(ix + 1, iz, gridWidth, gridLength);
            indices[i++] = IndexGrid(ix + 1, iz + 1, gridWidth, gridLength);
            indices[i++] = IndexGrid(ix + 1, iz, gridWidth, gridLength);
            indices[i++] = IndexGrid(ix, iz + 1, gridWidth, gridLength);
          }
        }

      }
    }

    Pointer<Vec3<float>> normals = CalculateNormals(vertices,indices);
    MeshData data(vertices, indices);
    data.AddAttribute({MESH_NORMALS_LOCATION, BufferAttributeType::VEC3},normals);

    return data;
  }

  MeshData MeshFactory::Sphere(const Vec3<float>& position, float radius, uint latitudes, uint longitudes)
  {
    if(latitudes < 1 || longitudes < 4)
    {
      Log::Error("Creating sphere failed. Latitude or longitude is too small.");
      Log::Error("Has to be atleast 1 and 4 respectivly");
      return Cube(position.x,position.y,position.z, radius, radius, radius);
    }

    uint vertexCount = 2 + longitudes * latitudes;
    Pointer<Vec3<float>> vertices = Pointer<Vec3<float>>(vertexCount);
    vertices[0] = Vec3<float>(0,-radius, 0);
    vertices[vertexCount - 1] = Vec3<float>(0,radius, 0);

    float angleY = M_PI / (latitudes + 1);
    float angleLong = 2 * M_PI / longitudes;
    float ay = angleY;
    uint i = 1;
    for(uint lat = 0; lat < latitudes; ++lat)
    {
      float y = radius * sin(ay - M_PI * 0.5f);
      for(uint lon = 0; lon< longitudes; ++lon)
      {
        float r = sqrt(radius*radius - y * y);
        float x = r * cos(angleLong * lon);
        float z = r * sin(angleLong * lon);
        vertices[i] = {x,y,z};
        i++;
      }
      ay += angleY;
    }

    // TRIANGLES = 2 * LONGITUDES
    // QUADS = (LATITUDES - 1) * LONGITUDES


    uint indexCount = 3 * (2 + 2 * (latitudes - 1)) * longitudes;
    Pointer<uint> indices = Pointer<uint>(indexCount);
    for(uint i = 0;i<longitudes;i++)
    {
      indices[i*3] = 0;
      indices[i*3 + 1] = i+1;
      // Allow the last index to wrap
      indices[i*3 + 2] = ((i+1) % longitudes) + 1;

      indices[indexCount - i*3 - 1] = vertexCount - (((i+1) % longitudes) + 1) - 1;
      indices[indexCount - i*3 - 2] = vertexCount - i - 2;
      indices[indexCount - i*3 - 3] = vertexCount - 1;
    }

    uint index = longitudes * 3;
    for(uint i = 0; i < latitudes - 1;i++)
    {
      uint vertexPos = 1 + i * longitudes;

      for(uint j = 0; j< longitudes; j++)
      {
        uint j1 = (j+1) % longitudes;
        indices[index++] = vertexPos + j;
        indices[index++] = vertexPos + j + longitudes;
        indices[index++] = vertexPos + j1;

        indices[index++] = vertexPos + j1;
        indices[index++] = vertexPos + j + longitudes;
        indices[index++] = vertexPos + j1 + longitudes;
      }
    }

    MeshData data{vertices, indices};
    Pointer<Vec3<float>> normals = CalculateNormals(vertices, indices);
    data.AddAttribute({MESH_NORMALS_LOCATION, BufferAttributeType::VEC3},normals);

    return data;
  }

}
