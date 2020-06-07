#pragma once

#include <graphics/fonts/Font.h>
#include <common/Types.h>
#include <string>
#include <set>

namespace Greet{

  class FontContainer
  {
    public:
      friend class Font;

    private:
      const byte* m_data;
      uint m_datasize;
      std::string m_name;
      std::string m_filename;
      std::set<Font*, FontCompare> m_fonts;

    public:
      FontContainer(const std::string& filename, const std::string& name);
      FontContainer(const byte* data, uint datasize, const std::string& name);
      virtual ~FontContainer();

      Font* GetSize(uint size);

      inline const std::string& GetName() const { return m_name; }
      inline const std::string& GetFileName() const { return m_filename; }
      inline const byte* GetData() const { return m_data;}
      inline uint GetDataSize() const { return m_datasize;}

  };
}
