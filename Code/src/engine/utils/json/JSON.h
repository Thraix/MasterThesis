#pragma once

#include <common/Types.h>
#include <logging/Log.h>
#include <cstring>

namespace Greet {

  class JSON
  {
    private:
      static std::string m_indent;

    public:
      static const std::string& get_indent()
      {
        return m_indent;
      }

      static void push_indent()
      {
        m_indent.push_back('\t');
      }

      static void pop_indent()
      {
        ASSERT(m_indent.size() != 0, "JSON INDENT ASSERT");
        m_indent.pop_back();
      }
  };
}
