#include "FontAtlas.h"

#include <math/Vec3.h>
#include <internal/GreetGL.h>

namespace Greet
{
  FontAtlas::FontAtlas(const std::string& filename, uint width, uint height, uint fontSize)
    : yPos(0), xPos(0), nextYPos(0), width(width), height(height), fontSize(fontSize),
    atlas{Texture2D::Create(width, height, TextureParams(TextureFilter::LINEAR, TextureWrap::CLAMP_TO_EDGE, TextureInternalFormat::RGBA))},
    m_pixels(width*height*4)
  {
    if(FT_Init_FreeType(&library))
    {
      Log::Error("Could not initialize FreeType.");
    }
    if(FT_New_Face(library, filename.c_str(), 0, &face))
    {
      Log::Error("Could not initialize font: ", filename);
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    memset(m_pixels.data(), 0, width*height*4);
  }

  FontAtlas::~FontAtlas()
  {
  }

  const Glyph& FontAtlas::GetGlyph(char character)
  {
    auto it = glyphs.find(character);
    if(it != glyphs.end())
    {
      return it->second;
    }
    return AddGlyph(character);
  }

  const Glyph& FontAtlas::AddGlyph(char character)
  {
    if(FT_Load_Char(face,character,FT_LOAD_RENDER))
    {
      Log::Error("Could not load character: ", character);
      // Lets just hope that there is a character in the map. Maybe add a dummy character later
      return glyphs.begin()->second;
    }

    uint pixelWidth = face->glyph->bitmap.width;
    uint pixelHeight = face->glyph->bitmap.rows;
    if(xPos + pixelWidth >= width)
    {
      xPos = 0;
      yPos = nextYPos + 1;
    }
    // Set new y value.
    if(yPos+pixelHeight > nextYPos)
      nextYPos = yPos+pixelHeight;
    for(uint y = 0;y<pixelHeight;y++)
    {
      for(uint x = 0;x<pixelWidth;x++)
      {
        unsigned char a = face->glyph->bitmap.buffer[(x+y*pixelWidth)];
        m_pixels[4*((x+xPos) + (y+yPos) * width)+0] = 0xff;
        m_pixels[4*((x+xPos) + (y+yPos) * width)+1] = 0xff;
        m_pixels[4*((x+xPos) + (y+yPos) * width)+2] = 0xff;
        m_pixels[4*((x+xPos) + (y+yPos) * width)+3] = a;
      }
    }
    Glyph g;
    g.width = face->glyph->metrics.width / 64.0;
    g.bearingX = face->glyph->metrics.horiBearingX / 64.0;
    g.advanceX = face->glyph->metrics.horiAdvance / 64.0;
    g.advanceY = face->glyph->metrics.vertAdvance / 64.0;
    g.height = face->glyph->metrics.height / 64.0;
    g.bearingY = (face->glyph->metrics.horiBearingY) / 64.0;
    g.descending = (g.height - face->glyph->metrics.horiBearingY) / 64.0;
    g.textureCoords = Vec4(yPos / (float)height, xPos / (float)width, (yPos+pixelHeight)/(float)height, (xPos+pixelWidth)/(float)width);
    xPos += pixelWidth + 1;
    std::pair<std::map<char,  Glyph>::iterator, bool> ret = glyphs.emplace(character, g);
    atlas->SetPixels(m_pixels);
    //Log::Info(g.width," ",g.kerning, " ", g.advanceX," ",g.advanceY, " ", g.height," ", g.ascending," ",g.descending);
    return ret.first->second;
  }

  uint FontAtlas::GetTextureId() const
  {
    return atlas->GetTexId();
  }

  uint FontAtlas::GetBaselineOffset() const
  {
    return (face->size->metrics.descender+ face->size->metrics.ascender) / 64.0f;
  }
}
