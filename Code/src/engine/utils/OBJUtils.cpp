#include "OBJUtils.h"

#include <utils/FileUtils.h>
#include <utils/StringUtils.h>
#include <graphics/models/MeshFactory.h>

namespace Greet {

  MeshData OBJUtils::ErrorModel()
  {
    return MeshFactory::Cube(0,0,0,1,1,1);
  }

  MeshData OBJUtils::LoadObj(const std::string& filename)
  {
    std::vector<std::string> dataLine;

    std::vector<Vec3<float>> positions;
    std::vector<Vec2> texCoords;
    std::vector<Vec3<float>> normals;
    std::vector<uint> indices;

    std::ifstream input(filename);
    std::map<std::string, uint> verticesMap;

    // These are the same length.
    std::vector<Vec3<float>> vertexPos;
    std::vector<Vec2> vertexTexCoords;
    std::vector<Vec3<float>> vertexNormals;

    for (std::string line; getline(input, line); )
    {
      std::istringstream iss{line};
      std::string s;
      iss >> s;
      if (s == "v")
      {
        Vec3<float> vector{};
        iss >> vector.x;
        iss >> vector.y;
        iss >> vector.z;
        positions.push_back(vector);
      }
      else if (s == "vt")
      {
        Vec2 vector{};
        iss >> vector.x;
        iss >> vector.y;
        texCoords.push_back(vector);
      }
      else if (s == "vn")
      {
        Vec3<float> vector{};
        iss >> vector.x;
        iss >> vector.y;
        iss >> vector.z;
        normals.push_back(vector);
      }
      else if (s == "f")
      {
        dataLine = StringUtils::SplitString(line, " ");
        for(int i = 1; i<dataLine.size();i++)
        {
          if(dataLine[i] == "" || dataLine[i] == "\r")
            continue;
          auto it = verticesMap.find(dataLine[i]);
          if(it != verticesMap.end())
          {
            indices.push_back(it->second);
          }
          else
          {
            indices.push_back(vertexPos.size());
            verticesMap.emplace(dataLine[i], vertexPos.size());
            std::vector<std::string> vertex = StringUtils::SplitString(dataLine[i], "/");

            // Only position defined
            if(vertex.size() == 1)
            {
              vertexPos.push_back(positions[atoi(vertex[0].c_str())-1]);
            }
            else if(vertex.size() == 2) // Position and texcoord defined
            {
              vertexPos.push_back(positions[atoi(vertex[0].c_str())-1]);
              if(vertex[1] != "")
                vertexTexCoords.push_back(texCoords[atoi(vertex[1].c_str())-1]);
            }
            else if(vertex.size() == 3)
            {
              vertexPos.push_back(positions[atoi(vertex[0].c_str())-1]);
              if(vertex[1] != "")
                vertexTexCoords.push_back(texCoords[atoi(vertex[1].c_str())-1]);
              if(vertex[2] != "")
                vertexNormals.push_back(normals[atoi(vertex[2].c_str())-1]);
            }
          }
        }
      }
    }

    input.close();

    MeshData mesh{vertexPos, indices};
    if(vertexTexCoords.size() > 0)
      mesh.AddAttribute({MESH_TEXCOORDS_LOCATION, BufferAttributeType::VEC2}, Pointer<Vec2>{vertexTexCoords});

    if(vertexNormals.size() > 0)
      mesh.AddAttribute({MESH_NORMALS_LOCATION, BufferAttributeType::VEC3}, Pointer<Vec3<float>>{vertexNormals});

    return mesh;
  }
}
