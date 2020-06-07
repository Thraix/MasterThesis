#include "FontManager.h"

#include <logging/Log.h>
#include <common/Types.h>
#include <utils/ErrorHandle.h>

namespace Greet{

  std::vector<FontContainer*> FontManager::m_fonts;

  void FontManager::Add(FontContainer* font)
  {
    for (int i = 0;i < m_fonts.size();i++)
    {
      if (font->GetName() == m_fonts[i]->GetName())
      {
        ErrorHandle::SetErrorCode(GREET_ERROR_MANAGER_ADD);
        Log::Error("Given font name already exists: ", font->GetName().c_str());
        return;
      }
    }
    m_fonts.push_back(font);
  }

  Font* FontManager::Get(const std::string& fontname, uint size)
  {
    for (uint i = 0; i < m_fonts.size(); i++)
    {
      if (fontname.compare(m_fonts[i]->GetName())==0)
      {
        return m_fonts[i]->GetSize(size);
      }
    }
    ErrorHandle::SetErrorCode(GREET_ERROR_MANAGER_GET);
    Log::Error("Could not find the given font: ", fontname.c_str());
    if (m_fonts.size() > 0)
      return m_fonts[0]->GetSize(size);
    return NULL; // Return Default that always can be read.
  }

  // TODO: REMOVE FONTS

  void FontManager::Destroy()
  {
    for (uint i = 0; i < m_fonts.size(); i++)
    {
      delete m_fonts[i];
    }
    m_fonts.clear();
  }

}
