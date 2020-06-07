#pragma once

#include <math/Maths.h>
#include <graphics/Renderable.h>
#include <common/Types.h>
#include <graphics/Sprite.h>

namespace Greet{

  struct VertexData
  {
    Vec2 vertex;
    Vec2 texCoord;
    float texID;
    uint color;
    Vec2 maskTexCoord;
    float maskTexID;
  };

  class Renderable2D : public Renderable
  {
    public:
      uint m_color;
      Vec2 m_position;
      Vec2 m_size;
    protected:
      std::optional<Sprite> m_sprite;
      std::optional<Sprite> m_mask;
    protected:

    public:
      Renderable2D(const Vec2& position,const Vec2& size, uint color, Sprite sprite)
        : m_position(position),m_size(size), m_color(color), m_sprite(sprite)
      {

      }

      Renderable2D(const Vec2& position,const Vec2& size, uint color, Sprite sprite, Sprite mask)
        : m_position(position),m_size(size), m_color(color), m_sprite(sprite), m_mask(mask)
      {

      }

      Renderable2D()
        : m_position(Vec2(0, 0)), m_size(Vec2(1, 1)), m_color(0xffffffff)
      {

      }

      void SetColor(uint color) override { m_color = color; }
      inline uint GetColor() const override { return m_color;}
      void SetPosition(const Vec2& position) override { m_position = position; }
      inline const Vec2& GetPosition() const override { return m_position;}
      void SetSize(const Vec2& size) override { m_size = size; }
      inline const Vec2& GetSize() const override { return m_size;}

      virtual ~Renderable2D()
      {
      }

      virtual void Submit(Renderer2D* renderer) const override
      {
        if (render)
          renderer->Submit(*this);
      }

      virtual bool Update(float timeElapsed) override
      {
        if(m_sprite)
          return m_sprite->Update(timeElapsed);
        return false;
      }

      inline uint GetTexID() const { return m_sprite ? m_sprite->GetTextureID() : 0; }
      inline uint GetMaskTexID() const { return m_mask ? m_mask->GetTextureID() : 0; }
      inline Vec2 GetTexPos() const { return m_sprite ? m_sprite->GetTexPos() : Vec2(0, 0); }
      inline Vec2 GetTexSize() const { return m_sprite ? m_sprite->GetTexSize() : Vec2(1, 1); }
      inline Vec2 GetMaskTexPos() const { return m_mask ? m_mask->GetTexPos() : Vec2(0, 0); }
      inline Vec2 GetMaskTexSize() const { return m_mask ? m_mask->GetTexSize() : Vec2(1, 1); }
      inline const std::optional<Sprite>& GetSprite() const { return m_sprite; }
      inline const std::optional<Sprite>& GetMask() const { return m_mask; }
  };
}
