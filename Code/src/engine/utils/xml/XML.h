#pragma once

#include <utils/xml/XMLObject.h>

namespace Greet
{
  struct XML
  {
    static XMLObject FromString(const std::string& string, const std::string& filename);
    static XMLObject FromFile(const std::string& fileName);
  };
}
