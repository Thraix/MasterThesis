#pragma once


#include <common/Types.h>
#include <logging/Log.h>

namespace Greet {

  struct LogUtils
  {
    static uint Hex10ToDec16(const char c)
    {
      if (c >= '0' && c <= '9')
        return (uint)(c - '0');
      else if (c >= 'a' && c <= 'f')
        return (uint)(c - 'a' + 10);
      else if(c >= 'A' && c <= 'F')
        return (uint)(c - 'A' + 10);
      Log::Warning("Utils::Hex10ToDec16 couldn't recognize the hex character \'", c, "\'");
      return 0;
    }

    static char Dec16ToHex10(int dec)
    {
      switch (dec)
      {
        case 0:
          return '0';
        case 1:
          return '1';
        case 2:
          return '2';
        case 3:
          return '3';
        case 4:
          return '4';
        case 5:
          return '5';
        case 6:
          return '6';
        case 7:
          return '7';
        case 8:
          return '8';
        case 9:
          return '9';
        case 10:
          return 'a';
        case 11:
          return 'b';
        case 12:
          return 'c';
        case 13:
          return 'd';
        case 14:
          return 'e';
        case 15:
          return 'f';
        default:
          return '0';
      }
    }

    static std::string DecToHex(uint dec, uint values = 1)
    {
      std::string s = "";
      bool trailing = true;
      for (uint i = 8; i >= 1; i--)
      {
        char hex = Dec16ToHex10(((dec >> (i * 4 - 4)) & 0xf));
        if (hex != '0' || i <= values || !trailing)
        {
          s += hex;
          if (hex != '0')
            trailing = false;
        }
      }
      return s;
    }

    static uint HexToDec(const std::string& hex)
    {
      if (hex.length() > 8)
      {
        return 0;
      }
      else
      {
        uint result = 0;

        int size = hex.length();
        for (int i = 0; i < size; i++)
        {
          result = (result << 4) | Hex10ToDec16(hex[i]);
        }
        return result;
      }
    }
  };
}
