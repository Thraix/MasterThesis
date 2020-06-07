#pragma once

#include <iostream>

#include <logging/LogLevel.h>
#include <logging/LogStream.h>

#include <vector>

namespace Greet {
  class Logger
  {
    private:
      std::vector<LogStream*> m_streams;
    public:
      Logger();
      virtual ~Logger();

      /*
       * Logs the given type to all the LogStream's if they allow the given LogLevel.
       */
      template <typename... Args>
      void Log(const LogLevel& level, const Args&... args)
      {
        for (std::vector<LogStream*>::iterator it = m_streams.begin(); it != m_streams.end(); it++)
        {
          (*it)->Output(level,args...);
        }
      }

      void AddLogStream(LogStream* stream);
      LogStream* GetLogStream(const std::string& name);
      LogStream* RemoveLogStream(const std::string& name);
      LogStream* RemoveLogStream(LogStream* stream);
  };
}
