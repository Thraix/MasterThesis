#pragma once

#include <common/Types.h>
#include <string>
#include <graphics/fonts/FontAtlas.h>

namespace Greet{

  class FontContainer;

  class Font
  {
    private:
      FontAtlas* m_atlas;
      FontContainer* m_container;
      float m_size;

    public:
      Font(FontContainer* container, uint size);
      virtual ~Font();
      void Init();
      inline FontAtlas* GetFontAtlas() const { return m_atlas;}
      inline uint GetFontAtlasId() const { return m_atlas->GetTextureId();}
      inline float GetSize() const {return m_size;}
      inline float GetBaselineOffset() const { return m_atlas->GetBaselineOffset();}
      inline const FontContainer* GetFontContainer() const { return m_container;}
      float GetWidthOfText(const std::string& text, uint startPos, uint endPos, float scale=1) const;
      float GetWidthOfText(const std::string& text, float scale=1) const;
      std::vector<float> GetPartialWidths(const std::string& text, float scale=1) const;

      friend bool operator<(const Font& f1, const Font& f2)
      {
        return f1.GetSize() < f2.GetSize();
      }
  };

  struct FontCompare
  {
    using is_transparent = void;

    // Yes these arguments are dumb, but atleast the compiler is not complaining anymore+-
    bool operator()(Font *const& f1, Font *const& f2) const
    {
      return f1->GetSize() < f2->GetSize();
    }

    // Yes these arguments are dumb, but atleast the compiler is not complaining anymore
    bool operator()(const uint& s, Font *const& f) const
    {
      return s < f->GetSize();
    }

    // Yes these arguments are dumb, but atleast the compiler is not complaining anymore
    bool operator()(Font *const& f, const uint& s) const
    {
      return f->GetSize() < s;
    }
  };
}
