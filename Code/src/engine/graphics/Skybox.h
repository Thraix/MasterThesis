#pragma once

#include <graphics/textures/CubeMap.h>
#include <graphics/cameras/Camera.h>
#include <graphics/shaders/Shader.h>
#include <graphics/models/Mesh.h>

namespace Greet {

  class Skybox
  {
    private:
      Ref<CubeMap> m_map;
      Ref<Shader> m_shader;
      Mesh* m_mesh;
    public:
      Skybox(const Ref<CubeMap>& map);
      Skybox(const Ref<CubeMap>&, const Ref<Shader>& shader);
      virtual ~Skybox();

      const Ref<CubeMap>& GetCubeMap() const { return m_map; };
      void Render(const Mat4& projectionMatrix, const Mat4& viewMatrix) const;
      void Render(const Camera& camera) const;
  };
}
