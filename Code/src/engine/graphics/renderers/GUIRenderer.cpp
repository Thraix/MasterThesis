#include "GUIRenderer.h"

#include <algorithm>
#include <cmath>

#include <graphics/RenderCommand.h>
#include <graphics/Renderable2D.h>
#include <graphics/shaders/ShaderFactory.h>
#include <internal/GreetGL.h>
#include <utils/ColorUtils.h>

namespace Greet
{
  GUIRenderer::GUIRenderer()
    : GUIRenderer{ShaderFactory::ShaderGUI()}
  {}

  GUIRenderer::GUIRenderer(const Ref<Shader>& shader)
    : Renderer2D{shader}
  {
    // TODO: Make this into a RenderCommand.
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextures);

    std::vector<int> samplerUnits(maxTextures);
    std::iota(samplerUnits.begin(), samplerUnits.end(), 0);
    shader->Enable();
    shader->SetUniform1iv("textures", maxTextures, samplerUnits.data());
    translationStack.push({0, 0});

    m_textures = new uint[maxTextures];
    m_vertices = 10000;
    m_bufferSize = m_vertices * sizeof(GUIVertex) * 4;
    m_iboSize = m_vertices * 6;

    vao = VertexArray::Create();
    vbo = VertexBuffer::CreateDynamic(nullptr, m_bufferSize);
    vbo->SetStructure({
        {shader->GetAttributeLocation("position"), BufferAttributeType::VEC2},
        {shader->GetAttributeLocation("texCoord"), BufferAttributeType::VEC2},
        {shader->GetAttributeLocation("texID"), BufferAttributeType::FLOAT},
        {shader->GetAttributeLocation("color"), BufferAttributeType::VEC4},
        {shader->GetAttributeLocation("viewport"), BufferAttributeType::VEC4},
        {shader->GetAttributeLocation("isHsv"), BufferAttributeType::FLOAT},
        });
    vao->AddVertexBuffer(vbo);

    vbo->Disable();
    //Generate all the indices at runtime
    m_indices = new uint[m_iboSize];
    ibo = Buffer::Create(m_iboSize, BufferType::INDEX, BufferDrawType::DYNAMIC);
    ibo->Disable();
    vao->SetIndexBuffer(ibo);
    vao->Disable();
    shader->Disable();
  }

  void GUIRenderer::Begin()
  {
    shader->Enable();
    vbo->Enable();
    m_buffer = (GUIVertex*)vbo->MapBuffer();

    m_bufferBegin = m_buffer;
    m_lastIndex = 0;
    m_iboCount = 0;
    m_textureCount = 0;
  }

  void GUIRenderer::Draw()
  {
    if (m_iboCount > 0)
    {
      RenderCommand::EnableCulling(false);
      // TODO: Remove this and actually draw the different things correctly
      // instead
      RenderCommand::EnableDepthTest(false);

      for (byte i = 0; i < m_textureCount; i++)
      {
        GLCall(glActiveTexture(GL_TEXTURE0 + i));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_textures[i]));
      }

      vao->Enable();
      ibo->UpdateData(m_indices, sizeof(uint) * m_iboCount);

      vao->Render(DrawType::TRIANGLES, m_iboCount);

      vao->Disable();

      GLCall(glActiveTexture(GL_TEXTURE0));
      RenderCommand::ResetCulling();
      RenderCommand::ResetDepthTest();
    }
    shader->Disable();
  }

  void GUIRenderer::End()
  {
    vbo->UnmapBuffer();
    vbo->Disable();
  }

  void GUIRenderer::Flush()
  {
    End();
    Draw();
    Begin();
  }

  void GUIRenderer::PushViewport(const Vec2& pos, const Vec2& size, bool overwrite)
  {
    Vec2 p1 = GetMatrix() * (translationStack.top() + pos);
    Vec2 p2 = GetMatrix() * (translationStack.top() + pos + size);
    if (!m_viewports.empty() && !overwrite)
    {
      Vec4 top = m_viewports.top();
      p1.x = p1.x < top.x ? top.x : p1.x;
      p1.y = p1.y < top.y ? top.y : p1.y;
      p2.x = p2.x > top.z ? top.z : p2.x;
      p2.y = p2.y > top.w ? top.w : p2.y;
    }
    m_viewports.push(Vec4(p1.x, p1.y, p2.x, p2.y));
  }

  void GUIRenderer::PopViewport()
  {
    if (m_viewports.empty())
    {
      Log::Warning("Trying to pop the last viewport.");
      return;
    }
    m_viewports.pop();
  }

  void GUIRenderer::PushTranslation(const Vec2& translation, bool override)
  {
    if (override)
      translationStack.push({round(translation.x), round(translation.y)});
    else
      translationStack.push(translationStack.top() + Vec2{round(translation.x), round(translation.y)});
  }

  void GUIRenderer::PopTranslation()
  {
    if (translationStack.size() > 1)
      translationStack.pop();
    else
      Log::Warning("Trying to pop the last translation.");
  }

  void GUIRenderer::Submit(const Renderable2D& renderable)
  {
    SubmitRect(renderable.GetPosition(), renderable.GetSize(), ColorUtils::ColorHexToVec4(renderable.GetColor()), false);
  }

  void GUIRenderer::SubmitLine(const Vec2& pos, float length, float width, bool vertical, const Vec4& color, bool isHsv)
  {

    SubmitRect(pos, Vec2(vertical ? width : length, vertical ? length : width), color,isHsv);
  }

  void GUIRenderer::SubmitTriangle(const Vec2& pos1, const Vec2& pos2, const Vec2& pos3, const Vec4& color, bool isHsv)
  {
    AppendTriangle(pos1,pos2,pos3,color,isHsv);
  }

  void GUIRenderer::SubmitRect(const Vec2& pos, const Vec2& size, const Vec4& color, bool isHsv)
  {
    AppendQuad(pos, size, Vec2(0, 0), Vec2(1, 1), 0, color, isHsv);
  }
  void GUIRenderer::SubmitRoundedRect(const Vec2& pos, const Vec2& size, const Vec4& color, float radius, uint precision, bool isHsv)
  {
    AppendRoundedQuad(pos,size,color,isHsv,radius,precision);
  }

  void  GUIRenderer::SubmitRect(const Vec2& pos, const Vec2& size, const Vec4& color1, const Vec4& color2, const Vec4& color3, const Vec4& color4, bool isHsv)
  {
    AppendQuad(pos, size, Vec2(0, 0), Vec2(1, 1), 0, color1, color2, color3, color4, isHsv);
  }


  void GUIRenderer::SubmitString(const std::string& text, const Vec2& position, Font* font, const Vec4& color, bool isHsv)
  {
    float ts = GetTextureSlot(font->GetFontAtlasId());
    if (ts == 0 && font->GetFontAtlasId() != 0)
    {
      Flush();
      ts = GetTextureSlot(font->GetFontAtlasId());
    }

    FontAtlas* atlas = font->GetFontAtlas();
    const Vec2& scale = Vec2(1,1);//Vec2(64.0, 64.0) / font->GetSize();//font->getScale();
    Vec2 pos;
    Vec2 size;
    Vec2 uv0;
    Vec2 uv1;
    Vec2 roundPos = Vec2(round(position.x), round(position.y));
    float x = roundPos.x;
    for (uint i = 0;i < text.length();i++)
    {
      const Glyph& glyph = atlas->GetGlyph(text[i]);
      pos.x = x + glyph.bearingX * scale.x;
      pos.y = (roundPos.y - glyph.bearingY * scale.y);
      size.x = glyph.width * scale.x;
      size.y = glyph.height * scale.y;

      uv0.x = glyph.textureCoords.left;
      uv0.y = 1.0-glyph.textureCoords.top;
      uv1.x = glyph.textureCoords.right;
      uv1.y = 1.0-glyph.textureCoords.bottom;

      AppendQuad(pos, size, uv0, uv1, ts, color, isHsv);

      x += glyph.advanceX* scale.x;
    }
  }

  float GUIRenderer::GetTextureSlot(uint id)
  {
    if (id == 0)
      return 0;
    for (byte i = 0; i < m_textureCount; i++)
    {
      if (m_textures[i] == id)
        return i + 1;
    }

    if (m_textureCount == maxTextures)
      return 0.0f;
    m_textures[m_textureCount++] = id;
    return m_textureCount;
  }

  bool GUIRenderer::NeedFlush(uint indices, uint vertices)
  {
    if (m_vertices - (m_buffer - m_bufferBegin) < vertices)
      return true;
    if (m_iboSize - m_iboCount < indices)
      return true;
    return false;
  }

  Vec4 GUIRenderer::GetViewport(const Vec2& pos1, const Vec2& pos2) const
  {
    if (!m_viewports.empty())
      return m_viewports.top();
    else
    {
      //Log::Warning("No viewport");
      Vec4 viewport;;
      Vec2 temp = GetMatrix() * (translationStack.top() + Vec2(pos1.x, pos1.y));
      viewport.x = temp.x;
      viewport.y = temp.y;
      temp = GetMatrix() * (translationStack.top() + Vec2(pos2.x, pos2.y));
      viewport.z = temp.x;
      viewport.w = temp.y;
      return viewport;
    }
  }

  void GUIRenderer::AppendTriangle(const Vec2& pos1, const Vec2& pos2, const Vec2& pos3, const Vec4& color, bool isHsv)
  {
    if (NeedFlush(3, 3))
      Flush();

    Vec4 viewport = GetViewport(Vec2(std::min(std::min(pos1.x,pos2.x),pos3.x), std::min(std::min(pos1.y, pos2.y), pos3.y)), Vec2(std::max(std::max(pos1.x, pos2.x), pos3.x), std::max(std::max(pos1.y, pos2.y), pos3.y)));

    AppendVertexBuffer(pos1, Vec2(0, 0), 0.0f, color, viewport, isHsv);
    AppendVertexBuffer(pos2, Vec2(0, 0), 0.0f, color, viewport, isHsv);
    AppendVertexBuffer(pos3, Vec2(0, 0), 0.0f, color, viewport, isHsv);

    m_indices[m_iboCount++] = m_lastIndex;
    m_indices[m_iboCount++] = m_lastIndex + 1;
    m_indices[m_iboCount++] = m_lastIndex + 2;

    m_lastIndex += 3;
  }

  void GUIRenderer::AppendQuad(const Vec2& position, const Vec2& size, const Vec2& texCoord1, const Vec2& texCoord2, float texId, const Vec4& color1, const Vec4& color2, const Vec4& color3, const Vec4& color4, bool isHsv)
  {
    if (NeedFlush(6, 4))
      Flush();

    Vec4 viewport = GetViewport(position, position + size);

    AppendVertexBuffer(position, texCoord1, texId, color1, viewport, isHsv);
    AppendVertexBuffer(Vec2(position.x, position.y + size.y), Vec2(texCoord1.x, texCoord2.y), texId, color3, viewport, isHsv);
    AppendVertexBuffer(position + size, texCoord2, texId, color4, viewport, isHsv);
    AppendVertexBuffer(Vec2(position.x + size.x, position.y), Vec2(texCoord2.x, texCoord1.y), texId, color2, viewport, isHsv);

    m_indices[m_iboCount++] = m_lastIndex;
    m_indices[m_iboCount++] = m_lastIndex + 1;
    m_indices[m_iboCount++] = m_lastIndex + 2;
    m_indices[m_iboCount++] = m_lastIndex;
    m_indices[m_iboCount++] = m_lastIndex + 2;
    m_indices[m_iboCount++] = m_lastIndex + 3;

    m_lastIndex += 4;
  }

  // TODO: Improve this code, remove the center vertex and
  // remove unnecessary verticies when width is too small
  void GUIRenderer::AppendRoundedQuad(const Vec2& position, const Vec2& size, const Vec4& color, bool isHsv, float radius, uint precision)
  {
    // If precision is 0 or radius is too little, just draw a normal quad
    if(precision == 0 || radius <= 0)
    {
      AppendQuad(position, size, Vec2(),Vec2(),0, color, isHsv);
      return;
    }

    if(NeedFlush((precision+1) * 12, (precision+1) * 4 + 1))
      Flush();

    float startAngle = 0;
    float endAngle = M_PI*0.5; // 90 deg

    if(radius*2 > size.w)
      endAngle = acos(1 - size.w * 0.5 / radius);
    if(radius*2 > size.h)
      startAngle = asin(1 - size.h * 0.5 / radius);

    float angle = M_PI*0.5/precision;
    float aStart = Math::RoundUp(startAngle, angle);
    float aEnd = Math::RoundDown(endAngle, angle);

    if(aEnd < aStart)
      return;

    Vec4 viewport{GetViewport(position, position+size)};

    // Center vertex (to make things easier)
    // TODO: Remove this and make another vertex the main one
    // Since the whole thing is non negative curvature any vertex should do
    // however needs to be checked and I was lazy now
    AppendVertexBuffer(position+size*0.5, Vec2(0,0), 0, color, viewport, isHsv);

    for(int i = 0;i<=precision;i++)
    {
      Vec2 circlePos{cos(angle*i)*radius,sin(angle*i)*radius};

      float a = angle*i;
      if(angle*i > endAngle)
      {
        float x = circlePos.x;
        circlePos.x = radius - size.w*0.5;

        a = aEnd;
        Vec2 nextCirclePos{cos(a)*radius,sin(a)*radius};
        float t = (circlePos.x - x) / (nextCirclePos.x - x);
        circlePos.y = (nextCirclePos.y - circlePos.y) * t + circlePos.y;
      }
      if(a < startAngle)
      {
        float y = circlePos.y;
        circlePos.y = radius - size.h*0.5;

        a = aStart;
        Vec2 nextCirclePos{cos(a)*radius,sin(a)*radius};
        float t = (circlePos.y - y) / (nextCirclePos.y - y);
        circlePos.x = (nextCirclePos.x - circlePos.x) * t + circlePos.x;
      }

      // Top left
      AppendVertexBuffer(position+radius - circlePos, Vec2(0,0), 0, color, viewport, isHsv);
      // Top right
      AppendVertexBuffer(position+Vec2(size.x-radius+circlePos.x,radius-circlePos.y), Vec2(0,0), 0, color, viewport, isHsv);
      // Bottom right
      AppendVertexBuffer(position+size-radius + circlePos, Vec2(0,0), 0, color, viewport, isHsv);
      // bottom left
      AppendVertexBuffer(position+Vec2(radius-circlePos.x,size.y-radius+circlePos.y), Vec2(0,0), 0, color, viewport, isHsv);

    }

    // All corner triangles
    for(int j = 1;j<=4;j++)
    {
      // Precision parts of a corner
      for(int i = 0;i<precision;i++)
      {
        m_indices[m_iboCount++] = m_lastIndex;
        m_indices[m_iboCount++] = m_lastIndex+i*4+j;
        m_indices[m_iboCount++] = m_lastIndex+(i+1)*4+j;
      }
    }


    // Top triangle
    m_indices[m_iboCount++] = m_lastIndex;
    m_indices[m_iboCount++] = m_lastIndex+precision*4+1;
    m_indices[m_iboCount++] = m_lastIndex+precision*4+2;

    // Right triangle
    m_indices[m_iboCount++] = m_lastIndex;
    m_indices[m_iboCount++] = m_lastIndex+2;
    m_indices[m_iboCount++] = m_lastIndex+3;

    // Bottom triangle
    m_indices[m_iboCount++] = m_lastIndex;
    m_indices[m_iboCount++] = m_lastIndex+precision*4 + 3;
    m_indices[m_iboCount++] = m_lastIndex+precision*4 + 4;

    // Left triangle
    m_indices[m_iboCount++] = m_lastIndex;
    m_indices[m_iboCount++] = m_lastIndex+4;
    m_indices[m_iboCount++] = m_lastIndex+1;

    m_lastIndex += (precision+1)*4 + 1;
  }

  void GUIRenderer::AppendQuaterCircle(const Vec2& center, const Vec4& color, bool isHsv, float radius, uint precision, bool left, bool top)
  {
    Vec4 viewport = GetViewport(center-radius, center + radius);
    float xRad = radius;
    float yRad = radius;
    if(left)
      xRad = -radius;
    if(top)
      yRad = -radius;

    if(NeedFlush(3*precision, 2+precision))
      Flush();

    AppendVertexBuffer(center, Vec2(0, 0), 0, color, viewport,false);
    AppendVertexBuffer(Vec2(center.x+xRad, center.y), Vec2(0,0), 0, color, viewport,false);

    float angle = M_PI*0.5 / precision;
    for(int i = 0;i < precision; i++)
    {
      float s = yRad*sin(angle*(i+1));
      float c = xRad*cos(angle*(i+1));
      if(i == precision-1)
      {
        s = yRad;
        c = 0;
      }
      AppendVertexBuffer(Vec2(center.x+c, center.y+s), Vec2(0,0), 0, color, viewport,false);
      m_indices[m_iboCount++] = m_lastIndex;
      m_indices[m_iboCount++] = m_lastIndex + i+1;
      m_indices[m_iboCount++] = m_lastIndex + i+2;
    }
    m_lastIndex += precision+2;

  }

  void GUIRenderer::AppendQuad(const Vec2& position, const Vec2& size, const Vec2& texCoord1, const Vec2& texCoord2, float texId, const Vec4& color, bool isHsv)
  {
    AppendQuad(position, size, texCoord1, texCoord2, texId, color, color, color, color, isHsv);
  }

  void GUIRenderer::AppendVertexBuffer(const Vec2& position, const Vec2& texCoord, float texId, const Vec4& color, const Vec4& viewport, bool isHsv)
  {
    m_buffer->pos = GetMatrix() * (translationStack.top() + position);
    m_buffer->texCoord = texCoord;
    m_buffer->texId = texId;
    m_buffer->color = color;
    m_buffer->viewport = viewport;
    m_buffer->isHSV = isHsv ? 1.0 : 0.0;
    m_buffer++;
  }

}
