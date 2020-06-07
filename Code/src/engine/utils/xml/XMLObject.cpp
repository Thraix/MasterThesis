#include "XMLObject.h"

#include <cstring>
#include <utils/xml/XMLException.h>
#include <utils/StringUtils.h>
#include <logging/Log.h>

namespace Greet
{
  XMLObject::XMLObject(const std::string& string)
  {
    int pos = 0;
    int line = 1;
    XMLLoadData data{pos, line, ""};
    if(!ReadHead(string, data))
      ReadBodyTail(string, data);
  }

  XMLObject::XMLObject(const std::string& string, int pos, int line, const std::string& file)
  {
    XMLLoadData data{pos, line, file};
    if (!ReadHead(string, data))
      ReadBodyTail(string, data);
  }

  XMLObject::XMLObject(const std::string& string, XMLLoadData& data)
  {
    if (!ReadHead(string, data))
      ReadBodyTail(string, data);
  }

  XMLObject::XMLObject(const std::string& name, const std::map<std::string, std::string>& attributes, const std::string& text)
    :name(name), attributes(attributes), text(text)
  {

  }

  XMLObject::XMLObject(const std::string& name, const std::map<std::string, std::string>& attributes, const std::vector<XMLObject>& objects)
    : name(name), attributes(attributes), objects(objects)
  {

  }

  bool XMLObject::HasAttribute(const std::string& property) const
  {
    return attributes.find(property) != attributes.end();
  }

  const std::string& XMLObject::GetAttribute(const std::string& property) const
  {
    auto it = attributes.find(property);
    if (it == attributes.end())
      throw XMLException((std::string("Attribute could not be found \"") + property + "\".").c_str());
    return it->second;
  }

  const std::string& XMLObject::GetAttribute(const std::string& property, const std::string& defaultValue) const
  {
    auto it = attributes.find(property);
    if (it == attributes.end())
      return defaultValue;
    return it->second;
  }

  unsigned int XMLObject::GetObjectCount() const
  {
    return objects.size();
  }

  const XMLObject& XMLObject::GetObject(unsigned int i) const
  {
    if (i >= objects.size())
      throw XMLException((std::string("XML index out of bounds \"") + std::to_string(i) + "\".").c_str());

    return objects[i];
  }

  const std::vector<XMLObject>& XMLObject::GetObjects() const
  {
    return objects;
  }

  const std::string& XMLObject::GetName() const
  {
    return name;
  }

  const std::string& XMLObject::GetText() const
  {
    return text;
  }

  void XMLObject::SetName(const std::string& name)
  {
    if(StringUtils::IsWord(name))
      this->name = name;
    else
      Log::Error("XML Head can only be made up of letters");
  }

  void XMLObject::SetText(const std::string& text)
  {
    this->text = text;
  }

  void XMLObject::AddAttribute(const std::string& property, const std::string& value)
  {
    if(StringUtils::IsWord(property))
      attributes.emplace(property, value);
    else
      Log::Error("XML property name can only be made up of letters");

  }

  XMLObject XMLObject::GetStrippedXMLObject() const
  {
    if(text == "")
      return XMLObject(name, attributes, objects);
    else
      return XMLObject(name, attributes, text);
  }

  ////////////////////////////////////////////////////////////
  //                                                        //
  // Everything below here handles the reading of xml files //
  //                                                        //
  ////////////////////////////////////////////////////////////


  bool XMLObject::ReadHead(const std::string& string, XMLLoadData& data)
  {
    // Check if the first character is the start of and xml tag.
    ReadWhiteSpace(string, data);
    if (string[data.pos] != '<')
      throw XMLException("Not an XML Object.", data);

    // Check if there is a closing tag
    size_t closing = string.find('>');
    if (closing == std::string::npos)
      throw XMLException("No enclosing > for opening tag.", data);

    // Read the name of the tag
    ReadName(string, data);

    //  Read all attributes of the xml tag
    ReadAttributes(string, data);

    // Read opening tag
    if (string[data.pos] == '/')
    {
      data.pos++;
      ReadWhiteSpace(string, data);
      if (string[data.pos] != '>')
        throw XMLException((std::string("Invalid character proceeding / in opening XML Tag \"") + string[data.pos] + "\".").c_str(), data);
      data.pos++;
      // nothing more to read.
      return true;
    }

    ReadWhiteSpace(string, data);
    if (string[data.pos] != '>')
      throw XMLException((std::string("Invalid character proceeding attributes in opening XML Tag \"") + string[data.pos] + "\".").c_str(), data);
    (data.pos)++;
    return false;
  }

  void XMLObject::ReadName(const std::string& string, XMLLoadData& data)
  {
    data.pos++;
    ReadWhiteSpace(string, data);
    if (!StringUtils::IsLetter(string[data.pos]))
      throw XMLException("Invalid XML name. Can only contain letters.", data);
    name = StringUtils::GetWord(string, data.pos);
    data.pos += name.length();
    ReadWhiteSpace(string, data);
    if (string[data.pos] != '/' && string[data.pos] != '>' && StringUtils::IsWhiteSpace(string[data.pos]))
    {
      throw XMLException((std::string("Invalid character proceeding name in XML Tag \"") + string[data.pos] + "\".").c_str(), data);
    }
  }

  void XMLObject::ReadAttributes(const std::string& string, XMLLoadData& data)
  {
    ReadWhiteSpace(string, data);

    while (string[data.pos] != '>' && string[data.pos] != '/')
    {
      ReadAttribute(string, data);
    }
  }

  void XMLObject::ReadAttribute(const std::string& string, XMLLoadData& data)
  {
    // Read property name
    std::string property = ReadXMLName(string, data);
    if (property.length() == 0)
      throw XMLException((std::string("Invalid character proceeding name \"") + string[data.pos] + "\".").c_str(), data);
    if (attributes.count(property) > 0)
      throw XMLException((std::string("Duplicate property in XML tag \"") + property + "\".").c_str(), data);
    data.pos += property.length();
    ReadWhiteSpace(string, data);

    // Read =
    if (string[data.pos] != '=')
      throw XMLException((std::string("Invalid character proceeding property name in XML Tag \"") + string[data.pos] + "\".").c_str(), data);
    (data.pos)++;
    ReadWhiteSpace(string, data);

    // Read value
    if (string[data.pos] != '\"')
      throw XMLException("XML property value is not inside enclosing quotes.", data);
    (data.pos)++;
    int valueStart = data.pos;
    while (string[data.pos] != '\"') (data.pos)++;
    std::string value = string.substr(valueStart, (data.pos) - valueStart);
    ReplacePredefinedEntities(value, data);
    (data.pos)++;
    attributes.emplace(property, value);
    ReadWhiteSpace(string, data);
  }

  void XMLObject::ReadBodyTail(const std::string& string, XMLLoadData& data)
  {
    ReadWhiteSpace(string, data);
    if (string[data.pos] != '<')
    {
      ReadText(string, data);
      ReadWhiteSpace(string, data);
      std::string closeTag = GetClosingTag(string, data);
      if (closeTag.length() == 0)
        throw XMLException("Tag after XML Test was not a closing tag. XMLObject doesn't support text and other XMLObjects at the same time.", data);
      return;
    }
    // Check if we can read the closing tag.
    std::string closeTag = GetClosingTag(string, data);
    while (closeTag.length() == 0)
    {
      XMLObject object = XMLObject(string, data);
      objects.push_back(object);
      ReadWhiteSpace(string, data);
      closeTag = GetClosingTag(string, data);
    }
  }

  void XMLObject::ReadText(const std::string& string, XMLLoadData& data)
  {
    int startPos = data.pos;
    while (string[data.pos] != '<') (data.pos)++;
    text = string.substr(startPos, (data.pos) - startPos);
    ReplacePredefinedEntities(text, data);
  }

  void XMLObject::ReadWhiteSpace(const std::string& string, XMLLoadData& data)
  {
    while (StringUtils::IsWhiteSpace(string[data.pos])) {
      if (string[data.pos] == '\n')
        (data.line)++;
      (data.pos)++;
    }
  }


  std::string XMLObject::GetClosingTag(const std::string& string, XMLLoadData& data)
  {
    int startPos = data.pos;
    int startLine = data.line;
    if (string[(data.pos)++] != '<')
    {
      data.pos = startPos;
      data.line = startLine;
      return "";
    }
    ReadWhiteSpace(string, data);
    if (string[(data.pos)++] != '/')
    {
      data.pos = startPos;
      data.line = startLine;
      return "";
    }
    ReadWhiteSpace(string, data);
    std::string tag = StringUtils::GetWord(string, data.pos);
    if (tag != name)
      throw XMLException((std::string("Closing tag doesn't match opening tag. (\"") + name + "\" != \"" + tag+ "\")").c_str(), data);
    data.pos += tag.length();
    ReadWhiteSpace(string, data);
    if (string[data.pos] != '>')
      throw XMLException((std::string("Invalid character in closing tag \"") + string[data.pos] + "\".").c_str(), data);
    (data.pos)++;
    return string.substr(startPos, (data.pos) - startPos);
  }

  void XMLObject::ReplacePredefinedEntities(std::string& string, XMLLoadData& data)
  {
    std::vector<std::pair<std::string, std::string>> entities
    {
      {"&quot;","\""},
      {"&apos;", "\'"},
      {"&lt;", "<"},
      {"&gt;",">"},
      {"&amp;", "&"}
    };
    size_t pos = string.find('&');
    while(pos != std::string::npos)
    {
      bool found = false;
      for(auto entity : entities)
      {
        if(strncmp(&string[pos], entity.first.c_str(), entity.first.length()) == 0)
        {
          string.replace(pos, entity.first.length(), entity.second);
          found = true;
        }
      }
      if(!found)
        Log::Warning("(" + data.file + ":" + std::to_string(data.line) + "): ""Ampersand found in xml but isn't a predefined entity.");
      pos = string.find('&', pos+1);
    }
  }

  std::string XMLObject::ReadXMLName(const std::string& string, XMLLoadData& data)
  {
    if(!(StringUtils::IsLetter(string[data.pos]) ||
          string[data.pos] == '_' ||
          string[data.pos] == ':'))
      throw XMLException(std::string("Name doesn't start with a letter."), data);

    int endPos = data.pos + 1;
    while (endPos < string.length() && (
          StringUtils::IsLetter(string[endPos]) ||
          string[endPos] == '_' ||
          string[endPos] == '-' ||
          string[endPos] == ':' ||
          string[endPos] == '.'))
      endPos++;
    return string.substr(data.pos, endPos - data.pos);
  }
}
