#pragma once

#include <map>
#include <string>
#include <fstream>
#include <utils/StringUtils.h>
#include <logging/Log.h>

namespace Greet
{
  class MetaFileClass
  {
    friend class MetaFile;
    private:
      std::map<std::string, std::string> values;

      MetaFileClass()
      {}

    public:
      std::string GetValue(const std::string& key, const std::string& val) const
      {
        auto it = values.find(key);
        if(it != values.end())
          return it->second;
        return val;
      }

      const std::map<std::string, std::string>& GetValues() const
      {
        return values;
      }
  };

  class MetaFile
  {
    private:
      std::string filepath;
      std::map<std::string, std::vector<MetaFileClass>> classes; // map<class, map<variable, value>>

    public:
      MetaFile(const std::string& filepath)
        : filepath{filepath}
      {
        if(StringUtils::EndsWith(filepath, ".meta"))
        {
          std::ifstream stream(filepath);

          if(!stream.is_open())
          {
            Log::Error("Could not find meta file: ", filepath);
            return;
          }

          std::string currentClass = "";
          auto metaClassIt = classes.end();
          std::string line;
          while(std::getline(stream, line))
          {
            if(line.empty())
              continue;

            if(line[StringUtils::GetTrimStartPos(line)] == '[' && line[StringUtils::GetTrimEndPos(line)] == ']' )
            {
              currentClass = StringUtils::Trim(line);
              currentClass = currentClass.substr(1, currentClass.size() - 2);
              metaClassIt = classes.find(currentClass);
              if(metaClassIt == classes.end())
                metaClassIt = classes.emplace(currentClass, std::vector<MetaFileClass>{{}}).first;
              else
                metaClassIt->second.push_back({});
              continue;
            }

            size_t pos = line.find("=");
            if(pos == std::string::npos)
            {
              Log::Warning("Meta file line does not contain \'=\'");
              continue;
            }

            std::string key = line.substr(0,pos);
            std::string value = line.substr(pos+1);
            if(key.length() == 0)
            {
              Log::Warning("Meta file key is empty");
              continue;
            }
            if(metaClassIt != classes.end())
            {
              auto res = metaClassIt->second[metaClassIt->second.size()-1].values.emplace(key, value);
              if(!res.second)
              {
                Log::Warning("Meta file key is defined twice: ", key);
              }
            }
            else
            {
              Log::Error("No meta file header specified: ", filepath);
            }
          }
        }
      }

      const std::vector<MetaFileClass>& GetMetaClass(const std::string& className)
      {
        static std::vector<MetaFileClass> empty;
        auto it = classes.find(className);
        if(it != classes.end())
          return it->second;
        return empty;
      }

      friend std::ostream& operator<<(std::ostream& stream, const MetaFile& file)
      {
        for(auto metaClasses : file.classes)
        {
          for(auto metaClass : metaClasses.second)
          {
            stream << "[" << metaClasses.first << "]" << std::endl;
            for(auto value : metaClass.GetValues())
            {
              stream << value.first << "=" << value.second << std::endl;
            }
          }
        }
        return stream;
      }
  };
}
