#pragma once

#include <graphics/Renderable2D.h>

#define __PRETTY_FUNCTION__ __func__

namespace Greet {
  class Group : public Renderable
  {
    protected:
      std::vector<Renderable*> m_renderables;
      Mat3 m_transformationMatrix;
    public:
      bool enable;
    public:
      Group(const Vec2& position);
      Group(const Mat3& transform);
      virtual ~Group();
      virtual void Add(Renderable* renderable);
      virtual void Remove(Renderable* renderable);
      virtual bool Update(float timeElapsed) override;
      virtual void Begin(Renderer2D* renderer) const override;
      virtual void Submit(Renderer2D* renderer) const override;
      virtual void End(Renderer2D* renderer) const override;

      void SetEnable(bool enable) { Group::enable = enable;}
      void SetTransformationMatrix(Mat3 transformationMatrix) {m_transformationMatrix = transformationMatrix;}

      void SetColor(uint color) override { Log::Warning("THIS DOES NOTHING",__PRETTY_FUNCTION__); }
      inline uint GetColor() const override { Log::Warning("THIS DOES NOTHING",__PRETTY_FUNCTION__);return 0xffffffff;}
      void SetPosition(const Vec2& position) override { Log::Warning("THIS DOES NOTHING",__PRETTY_FUNCTION__); }
      inline const Vec2& GetPosition() const override
      {
        Log::Warning("THIS DOES NOTHING",__PRETTY_FUNCTION__);
        static Vec2 ret(0,0);
        return ret;
      }
      void SetSize(const Vec2& size) override { Log::Warning("THIS DOES NOTHING",__PRETTY_FUNCTION__); }
      inline const Vec2& GetSize() const override
      {
        Log::Warning("THIS DOES NOTHING",__PRETTY_FUNCTION__);
        static Vec2 ret(1,1);
        return ret;
      }
  };

}
