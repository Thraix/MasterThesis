#pragma once

#include <utils/json/JSONArray.h>
#include <common/Types.h>
#include <map>

namespace Greet {

  class JSONObject
  {
    public:
      friend JSONArray;

    private:
      // Static Null values
      static std::string valueNull;
      static JSONObject objectNull;
      static JSONArray arrayNull;

      std::map<std::string, std::string> m_values;
      std::map<std::string, JSONArray> m_arrays;
      std::map<std::string, JSONObject> m_objects;

    public:
      JSONObject();
      JSONObject(const JSONObject& object);
      JSONObject(std::map<std::string,std::string> values,std::map<std::string,JSONObject> objects,std::map<std::string,JSONArray> arrays);

      bool HasKey(const std::string& key) const;
      bool HasValueKey(const std::string& key) const;
      bool HasArrayKey(const std::string& key) const;
      bool HasObjectKey(const std::string& key) const;

      void AddValue(std::string key, std::string value);
      void AddArray(std::string key, JSONArray value);
      void AddObject(std::string key, JSONObject value);

      const std::string& GetValue(const std::string& key, const std::string& defaultValue = "") const;
      uint GetValueAsUint(const std::string& key, uint defaultValue = 0) const;
      int GetValueAsInt(const std::string& key, int defaultValue = 0) const;
      float GetValueAsFloat(const std::string& key, float defaultValue = 0.0) const;
      bool GetValueAsBool(const std::string& key, bool defaultValue = false) const;
      bool IsNull(const std::string& key, bool defaultValue = true) const;

      const JSONArray& GetArray(const std::string& key) const;
      const JSONObject& GetObject(const std::string& key) const;

      friend std::ostream& operator<<(std::ostream& os, const JSONObject& obj);
  };

}
