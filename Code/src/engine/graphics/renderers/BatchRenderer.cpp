#include "BatchRenderer.h"

#include <graphics/RenderCommand.h>
#include <internal/GreetGL.h>
#include <logging/Log.h>

namespace Greet {

  BatchRenderer::BatchRenderer(const Ref<Shader>& shader)
    : Renderer2D{shader}
  {
    // TODO: Make this into a RenderCommand.
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextures);

    std::vector<int> samplerUnits(maxTextures);
    std::iota(samplerUnits.begin(), samplerUnits.end(), 0);
    shader->Enable();
    shader->SetUniform1iv("textures", maxTextures, samplerUnits.data());
    vao = VertexArray::Create();
    vbo = VertexBuffer::CreateDynamic(nullptr, RENDERER_BUFFER_SIZE);
    vbo->SetStructure({
        { shader->GetAttributeLocation("position"), BufferAttributeType::VEC2},
        { shader->GetAttributeLocation("texCoord"), BufferAttributeType::VEC2},
        { shader->GetAttributeLocation("texID"), BufferAttributeType::FLOAT},
        { shader->GetAttributeLocation("color"), BufferAttributeType::UBYTE4, true},
        { shader->GetAttributeLocation("maskTexCoord"), BufferAttributeType::VEC2},
        { shader->GetAttributeLocation("maskTexID"), BufferAttributeType::FLOAT}
        });
    vao->AddVertexBuffer(vbo);
    vbo->Disable();

    indices = new uint[RENDERER_INDICES_SIZE];
    ibo = Buffer::Create(sizeof(indices), BufferType::INDEX, BufferDrawType::DYNAMIC);
    ibo->Disable();
    vao->SetIndexBuffer(ibo);
    vao->Disable();
    shader->Disable();
  }

  BatchRenderer::~BatchRenderer()
  {
    delete indices;
  }

  void BatchRenderer::Begin()
  {
    shader->Enable();
    vbo->Enable();
    m_buffer = (VertexData*)vbo->MapBuffer();
    m_iboSize = 0;
    m_lastIndex = 0;
  }

  void BatchRenderer::Submit(const Renderable2D& renderable)
  {

    const uint color = renderable.GetColor();

    const Vec2 texPos = renderable.GetTexPos();
    const Vec2 texSize = renderable.GetTexSize();

    const Vec2 maskTexPos = renderable.GetMaskTexPos();
    const Vec2 maskTexSize = renderable.GetMaskTexSize();

    const uint texID = renderable.GetTexID();
    const uint maskTexID = renderable.GetMaskTexID();
    uint ts = GetTextureSlot(texID);
    uint mts = GetTextureSlot(maskTexID);
    Draw(renderable.GetPosition(),renderable.GetSize(), texPos, texSize, ts, color, mts,maskTexPos,maskTexSize);
  }

  void BatchRenderer::SubmitString(const std::string& text, const Vec2& position, Font* font, const uint& color)
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
      pos.y = roundPos.y - glyph.bearingY * scale.y;
      size.x = glyph.width * scale.x;
      size.y = glyph.height * scale.y;

      uv0.x = glyph.textureCoords.left;
      uv0.y = 1.0-glyph.textureCoords.top;
      uv1.x = glyph.textureCoords.right;
      uv1.y = 1.0-glyph.textureCoords.bottom;

      AppendVertexBuffer(Vec2(pos.x       ,pos.y       ), Vec2(uv0.x,uv0.y),ts,color, 0, Vec2(0, 0));
      AppendVertexBuffer(Vec2(pos.x       ,pos.y+size.y), Vec2(uv0.x,uv1.y),ts,color, 0, Vec2(0, 0));
      AppendVertexBuffer(Vec2(pos.x+size.x,pos.y+size.y), Vec2(uv1.x,uv1.y),ts,color, 0, Vec2(0, 0));
      AppendVertexBuffer(Vec2(pos.x+size.x,pos.y       ), Vec2(uv1.x,uv0.y),ts,color, 0, Vec2(0, 0));

      AddIndicesPoly(4);
      m_iboSize += 6;



      x += glyph.advanceX* scale.x;
    }
  }

  void BatchRenderer::Submit(const Transform& transform, uint texID, Vec2 texPos, Vec2 texSize, uint color, uint maskTexId, const Vec2& maskTexPos, const Vec2& maskTexSize)
  {
    Draw(transform, texPos, texSize, GetTextureSlot(texID), color, GetTextureSlot(maskTexId),maskTexPos,maskTexSize);
  }

  void BatchRenderer::Submit(const Vec2& position,const Vec2& size, uint texID, Vec2 texPos, Vec2 texSize, uint color,uint maskTexId, const Vec2& maskTexPos, const Vec2& maskTexSize)
  {
    uint ts = GetTextureSlot(texID);
    uint mts = GetTextureSlot(maskTexId);
    AppendVertexBuffer(Vec2(position.x, position.y), Vec2(texPos.x, texPos.y), ts, color, mts, Vec2(maskTexPos.x, maskTexPos.y));
    AppendVertexBuffer(Vec2(position.x, position.y + size.y), Vec2(texPos.x, texPos.y + texSize.y), ts, color, mts, Vec2(maskTexPos.x, maskTexPos.y + maskTexSize.y));
    AppendVertexBuffer(Vec2(position.x + size.x, position.y + size.y), Vec2(texPos.x + texSize.x, texPos.y + texSize.y), ts, color, mts, Vec2(maskTexPos.x + maskTexSize.x, maskTexPos.y + maskTexSize.y));
    AppendVertexBuffer(Vec2(position.x + size.x, position.y), Vec2(texPos.x + texSize.x, texPos.y), ts, color, mts, Vec2(maskTexPos.x + maskTexSize.x, maskTexPos.y));
    AddIndicesPoly(4);
    m_iboSize += 6;
  }

  void BatchRenderer::Submit(const Vec2& position,const Vec2& size, uint texID, Vec2 texPos, Vec2 texSize, uint color)
  {
    uint ts = GetTextureSlot(texID);
    AppendVertexBuffer(Vec2(position.x, position.y), Vec2(texPos.x, texPos.y), ts, color, 0, Vec2(0,0));
    AppendVertexBuffer(Vec2(position.x, position.y + size.y), Vec2(texPos.x, texPos.y + texSize.y), ts, color, 0,Vec2(0,0));
    AppendVertexBuffer(Vec2(position.x + size.x, position.y + size.y), Vec2(texPos.x + texSize.x, texPos.y + texSize.y), ts, color, 0,Vec2(0,0));
    AppendVertexBuffer(Vec2(position.x + size.x, position.y), Vec2(texPos.x + texSize.x, texPos.y), ts, color, 0,Vec2(0,0));
    AddIndicesPoly(4);
    m_iboSize += 6;
  }

  void BatchRenderer::Draw(const Vec2& position,const Vec2& size, const Vec2& texPos, const Vec2& texSize, const uint textureSlot, const uint color, const uint& maskTexSlot, const Vec2& maskTexPos, const Vec2& maskTexSize)
  {
    AppendVertexBuffer(Vec2(position.x, position.y),Vec2(texPos.x, texPos.y),textureSlot,color, maskTexSlot, Vec2(maskTexPos.x, maskTexPos.y));
    AppendVertexBuffer(Vec2(position.x, position.y+size.y),Vec2(texPos.x, texPos.y + texSize.y),textureSlot,color, maskTexSlot, Vec2(maskTexPos.x, maskTexPos.y+maskTexSize.y));
    AppendVertexBuffer(Vec2(position.x+size.x, position.y+size.y),Vec2(texPos.x + texSize.x, texPos.y + texSize.y),textureSlot,color, maskTexSlot, Vec2(maskTexPos.x + maskTexSize.x, maskTexPos.y + maskTexSize.y));
    AppendVertexBuffer(Vec2(position.x+size.x, position.y),Vec2(texPos.x + texSize.x, texPos.y),textureSlot,color, maskTexSlot, Vec2(maskTexPos.x + maskTexSize.x, maskTexPos.y));
    AddIndicesPoly(4);
    m_iboSize += 6;
  }

  void BatchRenderer::Draw(const Transform& transform, const Vec2& texPos, const Vec2& texSize, const uint textureSlot, const uint& color, const uint& maskTexSlot, const Vec2& maskTexPos, const Vec2& maskTexSize)
  {
    PushMatrix(transform.GetMatrix());

    AppendVertexBuffer(Vec2(0, 0),Vec2(texPos.x, texPos.y),textureSlot,color, maskTexSlot,Vec2(maskTexPos.x, maskTexPos.y));
    AppendVertexBuffer(Vec2(0, 1),Vec2(texPos.x, texPos.y + texSize.y),textureSlot,color, maskTexSlot, Vec2(maskTexPos.x, maskTexPos.y+maskTexSize.y));
    AppendVertexBuffer(Vec2(1, 1),Vec2(texPos.x + texSize.x, texPos.y + texSize.y),textureSlot,color, maskTexSlot, Vec2(maskTexPos.x + maskTexSize.x, maskTexPos.y + maskTexSize.y));
    AppendVertexBuffer(Vec2(1, 0),Vec2(texPos.x + texSize.x, texPos.y),textureSlot,color, maskTexSlot, Vec2(maskTexPos.x + maskTexSize.x, maskTexPos.y));

    PopMatrix();
    AddIndicesPoly(4);
    m_iboSize += 6;
  }

  void BatchRenderer::Draw(const Vec2& position, const Vec2* vertices, const uint amount, const uint color)
  {
    for (uint i = 0; i < amount; i++)
    {
      AppendVertexBuffer(position+vertices[i],Vec2(0,0),0,color, 0, Vec2(0, 0));
    }
    AddIndicesPoly(amount);
    m_iboSize += (amount-2)*3;
  }

  void BatchRenderer::DrawRect(const Vec2& position, const Vec2& size, const uint& color)
  {
    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    AppendVertexBuffer(position, Vec2(0, 0), 0, color, 0, Vec2(0, 0));
    AppendVertexBuffer(Vec2(position.x,position.y+size.y), Vec2(0, 1), 0, color,0,Vec2(0,1));
    AppendVertexBuffer(Vec2(position.x+size.x, position.y + size.y), Vec2(1, 1), 0, color, 0, Vec2(1, 1));
    AppendVertexBuffer(Vec2(position.x + size.x, position.y), Vec2(1, 0), 0, color, 0, Vec2(1, 0));

    AddIndicesPoly(4);
    m_iboSize += 6;

    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
  }

  void BatchRenderer::DrawLine(const Vec2& pos1, const Vec2& pos2, float width, uint color)
  {
    Vec2 delta = pos2-pos1;
    float alpha = atan(delta.y / delta.x);
    Vec2 p = Vec2{sin(alpha),cos(alpha)} * width * 0.5f;

    // This if is needed to avoid culling problems
    if(delta.x >= 0)
    {
      AppendVertexBuffer(pos1+Vec2{p.x, -p.y}, Vec2(0, 0), 0, color, 0, Vec2(0, 0));
      AppendVertexBuffer(pos1+Vec2{-p.x, p.y}, Vec2(0, 1), 0, color, 0, Vec2(0, 1));
      AppendVertexBuffer(pos2+Vec2{-p.x, p.y}, Vec2(1, 1), 0, color, 0, Vec2(1, 1));
      AppendVertexBuffer(pos2+Vec2{p.x, -p.y}, Vec2(1, 0), 0, color, 0, Vec2(1, 0));
    }
    else
    {
      AppendVertexBuffer(pos2+Vec2{p.x, -p.y}, Vec2(0, 0), 0, color, 0, Vec2(0, 0));
      AppendVertexBuffer(pos2+Vec2{-p.x, p.y}, Vec2(0, 1), 0, color, 0, Vec2(0, 1));
      AppendVertexBuffer(pos1+Vec2{-p.x, p.y}, Vec2(1, 1), 0, color, 0, Vec2(1, 1));
      AppendVertexBuffer(pos1+Vec2{p.x, -p.y}, Vec2(1, 0), 0, color, 0, Vec2(1, 0));
    }

    AddIndicesPoly(4);
    m_iboSize += 6;
  }

  void BatchRenderer::FillRect(const Vec2& position, const Vec2& size, const uint& color)
  {
    AppendVertexBuffer(position, Vec2(0, 0), 0, color, 0, Vec2(0, 0));
    AppendVertexBuffer(Vec2(position.x, position.y + size.y), Vec2(0, 1), 0, color, 0, Vec2(0, 1));
    AppendVertexBuffer(Vec2(position.x + size.x, position.y + size.y), Vec2(1, 1), 0, color, 0, Vec2(1, 1));
    AppendVertexBuffer(Vec2(position.x + size.x, position.y), Vec2(1, 0), 0, color, 0, Vec2(1, 0));

    AddIndicesPoly(4);
    m_iboSize += 6;
  }

  void BatchRenderer::FillRect(const Vec2& position, const Vec2& size, const uint& color, const Sprite* mask)
  {
    const Vec2& m_maskTexPos = mask->GetTexPos();
    const Vec2& m_maskTexSize = mask->GetTexSize();

    const uint& mtid = GetTextureSlot(mask->GetTextureID());

    AppendVertexBuffer(position, Vec2(0, 0), 0, color, mtid, m_maskTexPos);
    AppendVertexBuffer(Vec2(position.x, position.y + size.y), Vec2(0, 1), 0, color, mtid, Vec2(m_maskTexPos.x, m_maskTexPos.y+m_maskTexSize.y));
    AppendVertexBuffer(Vec2(position.x + size.x, position.y + size.y), Vec2(1, 1), 0, color, mtid, m_maskTexPos+m_maskTexSize);
    AppendVertexBuffer(Vec2(position.x + size.x, position.y), Vec2(1, 0), 0, color, mtid, Vec2(m_maskTexPos.x + m_maskTexSize.x, m_maskTexPos.y));

    AddIndicesPoly(4);
    m_iboSize += 6;
  }

  void BatchRenderer::AppendVertexBuffer(const Vec2& position, const Vec2& texCoord, const uint& texID, const uint& color, const uint& maskTexId, const Vec2& maskTexCoord)
  {
    m_buffer->vertex = *m_transformationBack*position;
    m_buffer->texCoord = texCoord;
    m_buffer->texID = texID;
    m_buffer->color = color;
    m_buffer->maskTexCoord = maskTexCoord;
    m_buffer->maskTexID = maskTexId;
    m_buffer++;
  }

  uint BatchRenderer::GetTextureSlot(const uint texID)
  {
    if (texID == 0)
      return 0.0f;
    uint ts = 0.0f;
    bool found = false;
    const int size = m_texSlots.size();
    for (int i = 0; i < size; i++)
    {
      if (m_texSlots[i] == texID)
      {
        ts = (uint)(i + 1);
        found = true;
        break;
      }
    }
    if (!found)
    {
      if (size >= maxTextures)
      {
        End();
        Flush();
        Begin();
      }
      m_texSlots.push_back(texID);
      ts = (uint)(size + 1);
    }
    return ts;
  }

  void BatchRenderer::End()
  {
    vbo->UnmapBuffer();
    vbo->Disable();
  }

  void BatchRenderer::Flush()
  {
    RenderCommand::EnableDepthTest(false);
    for (uint i = 0; i < m_texSlots.size(); i++)
    {
      GLCall(glActiveTexture(GL_TEXTURE0 + i));
      GLCall(glBindTexture(GL_TEXTURE_2D, m_texSlots[i]));
    }

    EnableBuffers();
    vao->Render(DrawType::TRIANGLES, m_iboSize);
    DisableBuffers();

    m_iboSize = 0;
    m_texSlots.clear();
    GLCall(glActiveTexture(GL_TEXTURE0));
    RenderCommand::ResetDepthTest();
    shader->Disable();
  }

  void BatchRenderer::EnableBuffers()
  {
    vao->Enable();
    ibo->UpdateData(indices, m_iboSize*sizeof(uint));
  }

  void BatchRenderer::DisableBuffers()
  {
    vao->Disable();
  }

  void BatchRenderer::AddIndicesPoly(uint vertices)
  {
    vertices -= 2;
    for (uint i = 0; i < vertices; i++)
    {
      indices[m_iboSize + 3 * i] = m_lastIndex;
      indices[m_iboSize + 3 * i + 1] = m_lastIndex + i+1;
      indices[m_iboSize + 3 * i + 2] = m_lastIndex + i+2;

    }
    m_lastIndex = indices[m_iboSize + 3 * (vertices - 1) + 2] + 1;
  }
}
