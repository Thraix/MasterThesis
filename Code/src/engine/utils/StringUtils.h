#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <regex>

namespace Greet {
  struct StringUtils
  {
    enum class CharType
    {
      LETTER, NUMBER, SPACE, SYMBOL
    };

    template <typename T>
    static std::string to_string(T t)
    {
      std::stringstream ss;
      ss << t;
      return ss.str().c_str();
    }

    static std::string Passwordify(const std::string& string)
    {
      return std::string(string.length(),'*');
    }

    static bool IsNumber(const std::string& str)
    {
      return std::regex_match(str, std::regex("[-+]?[0-9]*\\.?[0-9]*"));
    }

    static size_t GetTrimStartPos(const std::string& str)
    {
      size_t pos = 0;
      while((str[pos] == ' ' || str[pos] == '\t') && pos < str.size())
        pos++;

      if(pos == str.size())
        return std::string::npos;
      return pos;
    }

    static size_t GetTrimEndPos(const std::string& str)
    {
      size_t pos = str.size() - 1;
      while((str[pos] == ' ' || str[pos] == '\t') && pos >= str.size())
        pos--;

      if(pos == -1u)
        return std::string::npos;
      return pos;
    }

    static std::string_view Trim(const std::string& str)
    {
      size_t start = GetTrimStartPos(str);
      size_t end = GetTrimEndPos(str);
      if(start == std::string::npos)
        return str;
      return std::string_view(str.c_str() + start, end - start + 1);
    }

    static std::vector<std::string> SplitString(const std::string &s, const std::string& delimiter)
    {
      size_t start = 0;
      size_t end = s.find_first_of(delimiter);

      std::vector<std::string> output;

      while (end <= std::string::npos)
      {
        output.emplace_back(s.substr(start, end - start));

        if (end == std::string::npos)
          break;

        start = end + 1;
        end = s.find_first_of(delimiter, start);
      }

      return output;
    }

    static void ReplaceAll(std::string& str, const std::string& from, const std::string& to)
    {
      if (from.empty())
        return;
      size_t start_pos = 0;
      while ((start_pos = str.find(from, start_pos)) != std::string::npos)
      {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
      }
    }

    static bool StartsWith(const std::string& s, const std::string& prefix)
    {
      return s.compare(0, prefix.size(), prefix) == 0;
    }

    static bool EndsWith(const std::string& s, const std::string& suffix)
    {
      return s.compare(s.length()-suffix.length(), suffix.size(), suffix) == 0;
    }

    static bool IsWhiteSpace(char c)
    {
      return c == '\n' || c == '\t' || c == '\r' || c == ' ' || c == '\t';
    }

    static bool IsLetter(char c)
    {
      return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    static bool IsWord(const std::string& string)
    {
      for(auto it{string.begin()}; it != string.end();++it)
      {
        if(!IsLetter(*it))
          return false;
      }
      return true;
    }

    static std::string GetWord(const std::string& string, int startPos = 0)
    {
      if (startPos >= string.length())
        throw std::runtime_error("start position out of bounds.");

      int endPos = startPos;
      while (endPos < string.length() && IsLetter(string[endPos])) endPos++;
      return string.substr(startPos, endPos - startPos);
    }

    static CharType GetCharType(char c)
    {
      if(IsLetter(c))
        return CharType::LETTER;
      else if(c >= '0' && c <= '9')
        return CharType::NUMBER;
      else if(c == ' ' || c == '\t')
        return CharType::SPACE;
      else
        return CharType::SYMBOL;
    }


  };
}
