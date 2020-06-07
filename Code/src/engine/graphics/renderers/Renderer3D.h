#pragma once

#include <math/Maths.h>
#include <graphics/models/EntityModel.h>
#include <graphics/cameras/Camera.h>
#include <graphics/Skybox.h>
#include <graphics/layers/Scene.h>


namespace Greet{

  class Renderer3D
  {

    protected:

    public:
      Renderer3D()
      {}

      virtual ~Renderer3D()
      {}

      virtual void Update(float timeElapsed) {}
      void BindMatrices(const Ref<Shader>& shader, const Camera* camera, bool shouldBindShader = false) const;

      virtual void Begin(Camera* camera) {}
      virtual void Render(Camera* camera) const {}
      virtual void End(Camera* camera) {}
  };
}
