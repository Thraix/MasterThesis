#include "FontContainer.h"

namespace Greet{

  FontContainer::FontContainer(const std::string& filename, const std::string& name)
    : m_filename(filename),m_data(NULL),m_datasize(0), m_name(name)
  {
  }

  FontContainer::FontContainer(const byte* data, uint datasize, const std::string& name)
    : m_filename(""),m_data(data),m_datasize(datasize), m_name(name)
  {

  }

  FontContainer::~FontContainer()
  {
    auto it = m_fonts.begin();
    while(it != m_fonts.end())
    {
      delete (*it);
      it++;
    }
    m_fonts.clear();
  }

  Font* FontContainer::GetSize(uint size)
  {
    auto it = m_fonts.find(size);
    if (it == m_fonts.end())
    {
      Font* font = new Font(this,size);
      m_fonts.emplace(font);
      return font;
    }
    return *it;
  }

}
