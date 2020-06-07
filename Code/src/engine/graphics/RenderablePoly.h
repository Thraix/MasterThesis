#pragma once
#if 0

#include "Renderable.h"


namespace Greet {
  class RenderablePoly : public Renderable
  {
    public:
      Vec2 m_position;
      uint m_color;
    private:
      Vec2* m_vertices;
      uint m_vertexCount;

    public:

      RenderablePoly(Vec2 position, Vec2* vertices, uint vertexCount, uint color)
        : m_position(position),m_vertices(vertices), m_vertexCount(vertexCount), m_color(color)
      {

      }

      ~RenderablePoly()
      {
        delete[] m_vertices;
      }

      void SetVertices(Vec2* vertices, uint vertexCount)
      {
        m_vertices = vertices;
        m_vertexCount = vertexCount;
      }

      virtual void Submit(Renderer2D* renderer) const override
      {
        if (render)
          renderer->Submit(this);
      }

      virtual bool Update(float timeElapsed) override
      {
        return true;
      }

      void SetPosition(const Vec2& position) override { m_position = position; }
      inline const Vec2& GetPosition() const { return m_position; }
      void SetColor(uint color) override { m_color = color; }
      inline uint GetColor() const override { return m_color; }
      void SetSize(const Vec2& size) override { }
      inline const Vec2& GetSize() const override { return Vec2(1,1);}

      inline const Vec2* GetVertices() const { return m_vertices; }
      inline const uint GetVertexCount() const { return m_vertexCount; }
  };
}
#endif
