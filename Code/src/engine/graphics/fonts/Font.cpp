#include "Font.h"
#include <graphics/fonts/FontContainer.h>
#include <logging/Log.h>

namespace Greet{

  Font::Font(FontContainer* container, uint size)
    : m_container(container), m_size(size)
  {
    Init();
  }
  Font::~Font()
  {
    delete m_atlas;
  }

  void Font::Init()
  {
    m_atlas = new FontAtlas(m_container->GetFileName(),512,512, m_size);
  }

  float Font::GetWidthOfText(const std::string& text, uint startPos, uint endPos, float scale) const
  {
    float width = 0;
    if (startPos > text.size() || endPos < startPos || endPos > text.size())
    {
      Log::Error("Invalid start and endpos (start=", startPos, ", end=", endPos, ", strlen=", text.size(), ")");
      return 0;
    }

    for (uint i = startPos;i < endPos;i++)
    {
      const Glyph& glyph = m_atlas->GetGlyph(text[i]);
      // If it is the last char do not include the advancement
      if(i == endPos - 1 && text[i] != ' ')
        width += glyph.width;
      else
        width += glyph.advanceX;
    }
    return width*scale;
  }

  float Font::GetWidthOfText(const std::string& text, float scale) const
  {
    return GetWidthOfText(text,0,text.size(),scale);
  }

  std::vector<float> Font::GetPartialWidths(const std::string& text, float scale) const
  {
    float width = 0;

    std::vector<float> widths;

    for (uint i = 0;i < text.size();i++)
    {
      const Glyph& glyph = m_atlas->GetGlyph(text[i]);
      widths.push_back(width*scale);
      // If it is the last char do not include the advancment
      if(i == text.size()- 1 && text[i] != ' ')
        width += glyph.width;
      else
        width += glyph.advanceX;
    }
    widths.push_back(text.size());
    return widths;
  }
}
