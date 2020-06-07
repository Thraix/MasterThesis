#pragma once

#include <graphics/textures/Texture2D.h>
#include <map>
#include <math/Vec4.h>
#include <logging/Logger.h>
#include <common/Memory.h>

typedef struct FT_LibraryRec_  *FT_Library;
typedef struct FT_FaceRec_*  FT_Face;

namespace Greet
{
  struct Glyph
  {
    float width;
    float bearingX;
    float advanceX;
    float advanceY;
    float height;
    float bearingY;
    float descending;
    Vec4 textureCoords;
  };

  class FontAtlas
  {
    private:
      FT_Library library;
      FT_Face face;
      Ref<Texture2D> atlas;
      uint width;
      uint height;
      uint fontSize;
      std::map<char, Glyph> glyphs;
      std::vector<byte> m_pixels;

      uint yPos;
      uint xPos;
      uint nextYPos;

    public:
      FontAtlas(const std::string& filename, uint width, uint height, uint fontSize);
      virtual ~FontAtlas();

      const Glyph& GetGlyph(char character);
      uint GetTextureId() const;
      uint GetBaselineOffset() const;
    private:
      const Glyph& AddGlyph(char character);
  };
}
