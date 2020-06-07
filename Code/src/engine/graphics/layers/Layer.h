#pragma once

#include <graphics/shaders/Shader.h>
#include <graphics/Renderable.h>
#include <graphics/renderers/BatchRenderer.h>
#include <graphics/renderers/Renderer2D.h>
#include <graphics/layers/Scene.h>

namespace Greet {

  class Layer : public Scene
  {
    protected:
      Renderer2D* renderer;
      std::vector<Renderable*> renderables;

      Mat3 projectionMatrix;
    public:
      Layer(Renderer2D* renderer, const Mat3& projectionMatrix)
        : renderer(renderer), projectionMatrix(projectionMatrix)
      {
        renderer->GetShader()->Enable();
        renderer->GetShader()->SetUniformMat3("projectionMatrix", projectionMatrix);
        renderer->GetShader()->Disable();
      }

      virtual ~Layer()
      {
        delete renderer;

        for (uint i = 0; i < renderables.size(); i++)
          delete renderables[i];
      }

      virtual void Add(Renderable* renderable)
      {
        renderables.push_back(renderable);
      }

      virtual void PreRender() override
      {
        renderer->Begin();
        setUniforms();
      }

      virtual void Render() const override
      {
        uint size = renderables.size();
        for (uint i = 0; i < size; i++)
        {
          renderables[i]->Begin(renderer);
          renderables[i]->Submit(renderer);
          renderables[i]->End(renderer);
        }
      }

      virtual void PostRender() override
      {
        renderer->End();
        renderer->Flush();
      }

      virtual void Update(float timeElapsed) override
      {
        uint size = renderables.size();
        for (uint i = 0; i < size; i++)
          renderables[i]->Update(timeElapsed);
      }

      void SetProjectionMatrix(Mat3 projectionMatrix)
      {
        projectionMatrix = projectionMatrix;
        renderer->GetShader()->Enable();
        renderer->GetShader()->SetUniformMat3("projectionMatrix", projectionMatrix);
        renderer->GetShader()->Disable();
      }

      virtual void ViewportResize(float x, float y, float width, float height) {}
      virtual void ViewportResize(ViewportResizeEvent& event) override {}
      Mat3 GetProjectionMatrix() const { return projectionMatrix; };
      inline const Ref<Shader>& GetShader() const { return renderer->GetShader(); }
      inline uint Size() const { return renderables.size(); }


    protected:
      virtual void setUniforms() const {}
  };
}
