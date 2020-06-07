#pragma once

#include <vector>
#include <graphics/fonts/FontContainer.h>

namespace Greet {
  class FontManager
  {
    private:
      static std::vector<FontContainer*> m_fonts;
    public:

      static void Add(FontContainer* font);
      static Font* Get(const std::string& fontname, uint size);
      static void Destroy();
    private:
      FontManager(){}
  };
}
