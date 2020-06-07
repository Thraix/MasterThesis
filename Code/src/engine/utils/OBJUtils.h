#pragma once

#include <vector>
#include <graphics/models/Mesh.h>
#include <string>
#include <fstream>

namespace Greet {
  class OBJUtils
  {
    private:
      static inline MeshData ErrorModel();
      static inline void ProcessVertex(const std::vector<std::string>& vertexData, std::vector<uint>& indicesArray, const std::vector<Vec2>& texCoords, const std::vector<Vec3<float>>& normals, Vec2* texCoordsArray, Vec3<float>* normalsArray);
    public:
      static MeshData LoadObj(const std::string& filename);
  };
}
