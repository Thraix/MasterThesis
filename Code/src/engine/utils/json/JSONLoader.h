#pragma once

#include <utils/json/JSONObject.h>
#include <utils/json/JSONArray.h>


namespace Greet {

  class JSONLoader
  {
    public:
      static JSONObject LoadJSON(const std::string& filename);
    private:

      static bool IsKeyValue(const std::string& file, uint& currentPos, std::string* key, std::string* value, JSONObject* object, JSONArray* jsonArray, uint* type);
      static bool IsValue(const std::string& file, uint& currentPos, std::string* value, JSONObject* object, JSONArray* jsonArray, uint* type);
      static bool IsString(const std::string& file, uint& currentPos, std::string* value);
      static bool IsNumber(const std::string& file, uint& currentPos, std::string* value);
      static bool IsObject(const std::string& file, uint& currentPos, JSONObject* object);
      static bool IsArray(const std::string& file, uint& currentPos, JSONArray* jsonArray);
      static void SetString(const std::string& file, uint start, uint end, std::string* set);
      static bool IsIndent(const std::string& file, uint& currentPos);

  };

}
