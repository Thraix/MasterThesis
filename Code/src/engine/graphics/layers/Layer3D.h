#pragma once

#include <graphics/renderers/Renderer3D.h>
#include <graphics/layers/Scene.h>

namespace Greet {
  class Layer3D : public Scene
  {
    protected:
      std::vector<Renderer3D*> renderers;
      Camera* camera;
      Skybox* skybox;

    public:
      Layer3D(Camera* camera, Skybox* skybox);
      Layer3D(Camera* camera);
      virtual ~Layer3D();
      virtual void PreRender() override;
      virtual void Render() const override;
      virtual void PostRender() override;
      virtual void Update(float timeElapsed) override;
      virtual void OnEvent(Event& event) override;
      virtual void ViewportResize(ViewportResizeEvent& event) override;

      void AddRenderer(Renderer3D* renderer);
      void RemoveRenderer(Renderer3D* renderer);

      Vec3<float> GetWorldToScreenCoordinate(const Vec3<float>& coordinate) const;
      void GetScreenToWorldCoordinate(const Vec2& screenPos, Vec3<float>* near, Vec3<float>* direction) const;
  };
}
