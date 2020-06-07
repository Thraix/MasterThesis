#include "JSONObject.h"

#include <utils/StringUtils.h>

namespace Greet {

  std::string JSONObject::valueNull;
  JSONObject JSONObject::objectNull;
  JSONArray JSONObject::arrayNull;

  JSONObject::JSONObject()
  {

  }

  JSONObject::JSONObject(const JSONObject& object)
    :m_values(object.m_values), m_objects(object.m_objects), m_arrays(object.m_arrays)
  {

  }
  JSONObject::JSONObject(std::map<std::string,std::string> values, std::map<std::string,JSONObject> objects,std::map<std::string,JSONArray> arrays)
    : m_values(values), m_objects(objects), m_arrays(arrays)
  {

  }

  bool JSONObject::HasKey(const std::string& key) const
  {
    if (m_values.find(key) != m_values.end())
      return true;
    if (m_objects.find(key) != m_objects.end())
      return true;
    if (m_arrays.find(key) != m_arrays.end())
      return true;
    return false;
  }

  bool JSONObject::HasValueKey(const std::string& key) const
  {
    return m_values.find(key) != m_values.end();
  }

  bool JSONObject::HasArrayKey(const std::string& key) const
  {
    return m_arrays.find(key) != m_arrays.end();
  }

  bool JSONObject::HasObjectKey(const std::string& key) const
  {
    return m_objects.find(key) != m_objects.end();
  }

  void JSONObject::AddValue(std::string key, std::string value)
  {
    if (HasKey(key))
    {
      Log::Error("Key already defined in JSONObject: ", key);
      return;
    }
    m_values[key] = value;
  }
  void JSONObject::AddArray(std::string key, JSONArray value)
  {
    if (HasKey(key))
    {
      Log::Error("Key already defined in JSONObject: ", key);
      return;
    }
    m_arrays[key] = value;
  }
  void JSONObject::AddObject(std::string key, JSONObject value)
  {
    if (HasKey(key))
    {
      Log::Error("Key already defined in JSONObject: ", key);
      return;
    }
    m_objects[key] = value;
  }

  const std::string& JSONObject::GetValue(const std::string& key, const std::string& defaultValue) const
  {
    auto it = m_values.find(key);
    if (it == m_values.end())
    {
      return defaultValue;
    }
    return it->second;
  }

  int JSONObject::GetValueAsInt(const std::string& key, int defaultValue) const
  {
    auto it = m_values.find(key);
    if (it == m_values.end())
    {
      return defaultValue;
    }
    return std::stoi(it->second.c_str());
  }

  uint JSONObject::GetValueAsUint(const std::string& key, uint defaultValue) const
  {
    auto it = m_values.find(key);
    if (it == m_values.end())
    {
      return defaultValue;
    }
    return std::stoul(it->second.c_str());
  }

  float JSONObject::GetValueAsFloat(const std::string& key, float defaultValue) const
  {
    auto it = m_values.find(key);
    if (it == m_values.end())
    {
      return defaultValue;
    }
    return atof(it->second.c_str());
  }

  bool JSONObject::GetValueAsBool(const std::string& key, bool defaultValue) const
  {
    auto it = m_values.find(key);
    if (it == m_values.end())
    {
      return defaultValue;
    }
    return it->second == "true";
  }

  bool JSONObject::IsNull(const std::string& key, bool defaultValue) const
  {
    auto it = m_values.find(key);
    if (it == m_values.end())
    {
      return defaultValue;
    }
    return it->second == "null";
  }

  const JSONArray& JSONObject::GetArray(const std::string& key) const
  {
    auto it = m_arrays.find(key);
    if (it == m_arrays.end())
    {
      Log::Error("Key does not exist in JSONObject: ", key);
      return arrayNull;
    }
    return it->second;

  }

  const JSONObject& JSONObject::GetObject(const std::string& key) const
  {
    auto it = m_objects.find(key);
    if (it == m_objects.end())
    {
      Log::Error("Key does not exist in JSONObject: ", key);
      return objectNull;
    }
    return it->second;
  }

  std::ostream& operator<<(std::ostream& os, const JSONObject& obj)
  {
    os << JSON::get_indent() << "{\n";
    JSON::push_indent();
    uint vsize = obj.m_values.size();
    uint asize = obj.m_arrays.size();
    uint osize = obj.m_objects.size();

    uint i = 1;
    for (auto it = obj.m_values.begin();it != obj.m_values.end(); ++it, ++i)
    {
      os << JSON::get_indent() << "\"" << it->first << "\"" << ": ";
      if (it->second == "null" || it->second == "true" || it->second == "false" || StringUtils::IsNumber(it->second))
      {
        os << it->second;
      }
      else
      {
        os << "\"" << it->second << "\"";
      }

      if (i != vsize || asize != 0 || osize != 0)
      {
        os << ",\n";
      }
    }

    i = 1;
    for (auto it = obj.m_arrays.begin();it != obj.m_arrays.end(); ++it, ++i)
    {
      os << JSON::get_indent() << "\"" << it->first << "\"" << ":\n" << it->second;
      if (i != asize || osize != 0)
      {
        os << ",\n";
      }
    }

    i = 1;
    for (auto it = obj.m_objects.begin();it != obj.m_objects.end(); ++it, ++i)
    {
      os << JSON::get_indent() << "\"" << it->first << "\"" << ":\n" << it->second;
      if (i != osize)
      {
        os << ",\n";
      }
    }
    JSON::pop_indent();
    os << "\n" << JSON::get_indent() << "}";
    return os;
  }
}
