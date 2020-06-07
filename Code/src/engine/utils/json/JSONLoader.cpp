#include "JSONLoader.h"

#include <utils/FileUtils.h>
#include <utils/StringUtils.h>

namespace Greet {

  JSONObject JSONLoader::LoadJSON(const std::string& filename)
  {
    std::string file = FileUtils::read_file(filename.c_str());
    uint currentPos = file.find("{");
    while (IsIndent(file, currentPos));
    JSONObject obj;
    if (!IsObject(file, currentPos,&obj))
    {
      Log::Error("Invalid format: ",currentPos,file[currentPos] == ' ');
      return JSONObject();
    }
    while (IsIndent(file, currentPos));
    //if (currentPos != file.size())
    {
      //Log::Info("JSONObject ended but there is more in the file.");
      //return JSONObject();
    }
    return obj;
  }

  bool JSONLoader::IsValue(const std::string& file, uint& currentPos, std::string* value, JSONObject* object, JSONArray* jsonArray, uint* type)
  {
    uint lastPos = currentPos;
    std::string s = file.substr(currentPos,4);
    currentPos += 4;
    if (s == "null")
    {
      *value = "null";
      *type = 0;
      return true;
    }
    if (s == "true")
    {
      *value = "true";
      *type = 0;
      return true;
    }
    currentPos = lastPos;
    s = file.substr(currentPos,5);
    currentPos += 5;
    if (s == "false")
    {
      *type = 0;
      *value = "false";
      return true;
    }
    currentPos = lastPos;
    if (IsString(file, currentPos, value))
    {
      *type = 0;
      return true;
    }
    if (IsNumber(file, currentPos, value))
    {
      *type = 0;
      return true;
    }
    if (IsObject(file, currentPos, object))
    {
      *type = 1;
      return true;
    }
    if (IsArray(file, currentPos, jsonArray))
    {
      *type = 2;
      return true;
    }
    return false;
  }

  bool JSONLoader::IsObject(const std::string& file, uint& currentPos, JSONObject* object)
  {
    JSONObject retObject;
    std::string key;
    std::string value;
    JSONObject obj;
    JSONArray jsonArray;
    uint type;
    uint lastPos = currentPos;
    while (IsIndent(file, currentPos));
    char c = file[currentPos++];
    if (c != '{')
    {
      currentPos = lastPos;
      return false;
    }
    do
    {
      while (IsIndent(file, currentPos));
      if (!IsKeyValue(file, currentPos, &key, &value, &obj, &jsonArray, &type))
      {
        currentPos = lastPos;
        Log::Info("Argument in JSONObject doesn't have a key or value");
        return false;
      }
      if (type == 0)
      {
        retObject.AddValue(key, value);
        value = "";
      }
      else if (type == 1)
      {
        retObject.AddObject(key, obj);
        obj = JSONObject();
      }
      else if (type == 2)
      {
        retObject.AddArray(key,jsonArray);
        jsonArray = JSONArray();
      }
      while (IsIndent(file, currentPos));
      c = file[currentPos++];
    } while (c == ',');

    currentPos--;
    while (IsIndent(file, currentPos));
    c = file[currentPos++];
    if (c != '}')
    {
      currentPos = lastPos;
      return false;
    }
    *object = retObject;
    return true;
  }

  bool JSONLoader::IsArray(const std::string& file, uint& currentPos, JSONArray* jsonArray)
  {
    JSONObject object;
    uint lastPos = currentPos;
    char c = file[currentPos++];
    if (c != '[')
    {
      currentPos = lastPos;
      return false;
    }
    do {
      while (IsIndent(file, currentPos));
      if (!IsObject(file, currentPos, &object))
      {
        currentPos = lastPos;
        return false;
      }
      jsonArray->AddObject(object);
      object = JSONObject(); // Reset JSONObject

      while (IsIndent(file, currentPos));
      c = file[currentPos++];
    } while (c == ',');
    currentPos--;
    while (IsIndent(file, currentPos));
    c = file[currentPos++];
    if (c != ']')
    {
      currentPos = lastPos;
      return false;
    }
    return true;
  }

  bool JSONLoader::IsKeyValue(const std::string& file, uint& currentPos, std::string* key, std::string* value, JSONObject* object, JSONArray* jsonArray, uint* type)
  {
    uint lastPos = currentPos;
    std::string keyValue;
    if (!IsString(file, currentPos,&keyValue))
    {
      currentPos = lastPos;
      return false;
    }
    while (IsIndent(file, currentPos));
    char c = file[currentPos++];
    if (c != ':')
    {
      currentPos = lastPos;
      return false;
    }
    while (IsIndent(file, currentPos));
    if (!IsValue(file,currentPos,value,object,jsonArray,type))
    {
      currentPos = lastPos;
      return false;
    }
    *key = keyValue;
    return true;
  }

  bool JSONLoader::IsString(const std::string& file, uint& currentPos, std::string* value)
  {
    uint lastPos = currentPos;
    char c = file[currentPos++];
    if (c != '\"')
    {
      currentPos = lastPos;
      return false;
    }
    c = file[currentPos++];
    while (c != '\"' && c != std::string::npos)
    {
      if (c == '\\')
      {
        c = file[currentPos++];
        if (c != '\"' && c != '\\' && c != '/' && c != 'b' && c != 'f' && c != 'n' && c != 'r' && c != 't' && c != 'u') {
          currentPos = lastPos;
          return false;
        }
        if (c == 'u')
        {
          for (int i = 0;i < 4;i++)
          {
            c = file[currentPos++];
            if ((c < '0' || c > '9') && (c < 'a' || c > 'f'))
            {
              currentPos = lastPos;
              return false;
            }
          }
        }
      }
      c = file[currentPos++];
    }
    std::string retValue;
    SetString(file,lastPos,currentPos,&retValue);
    retValue = retValue.substr(1,retValue.size()-2); // remove both " around string
    StringUtils::ReplaceAll(retValue, "\\\"", "\"");
    StringUtils::ReplaceAll(retValue, "\\\\", "\\");
    StringUtils::ReplaceAll(retValue, "\\b", "\b");
    StringUtils::ReplaceAll(retValue, "\\f", "\f");
    StringUtils::ReplaceAll(retValue, "\\n", "\n");
    StringUtils::ReplaceAll(retValue, "\\r", "\r");
    StringUtils::ReplaceAll(retValue, "\\t", "\t");
    //replace_all(retValue, "\\u", "\u");
    *value = retValue;
    return true;
  }

  bool JSONLoader::IsNumber(const std::string& file, uint& currentPos, std::string* value)
  {
    uint lastPos = currentPos;
    char c = file[currentPos++];
    if (c != '-' && (c < '0' || c > '9'))
    {
      currentPos = lastPos;
      return false;
    }
    if (c == '-')
    {
      c = file[currentPos++];
    }
    if (c >= '1' && c <= '9')
    {
      c = file[currentPos++];
      while (c >= '0' && c <= '9')
        c = file[currentPos++];
      uint pos = currentPos;
      if (c == '.')
      {
        c = file[currentPos++];
        if (c < '0' || c > '9')
        {
          currentPos = pos;
          SetString(file,lastPos,currentPos,value);
          return true;
        }
        while (c >= '0' && c <= '9')
          c = file[currentPos++];
      }
      currentPos--;
      SetString(file,lastPos,currentPos,value);
      return true;
    }
    else // if c == '0'
    {
      c = file[currentPos++];
      if (c != '.')
      {
        currentPos--;
        SetString(file,lastPos,currentPos,value);
        return true;
      }
      c = file[currentPos++];
      if (c < '0' || c > '9')
      {
        currentPos = lastPos;
        return false;
      }
      c = file[currentPos++];
      while (c >= '0' && c <= '9')
        c = file[currentPos++];
      currentPos--;
      SetString(file,lastPos,currentPos,value);
      return true;
    }
    SetString(file,lastPos,currentPos,value);
    return true;
  }


  void JSONLoader::SetString(const std::string& file, uint start, uint end, std::string* set)
  {
    *set = file.substr(start,end-start);
  }

  bool JSONLoader::IsIndent(const std::string& file, uint& currentPos)
  {
    char c = file[currentPos++];
    if (c != '\t' && c != '\n' && c != ' ')
    {
      currentPos--;
      return false;
    }
    return true;
  }
}
