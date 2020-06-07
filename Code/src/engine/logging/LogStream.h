#pragma once

#include <logging/LogLevel.h>
#include <ostream>
#include <string>
#include <iostream>
#include <iterator>

namespace Greet {

  typedef bool(*LevelCheck)(const LogLevel& myLevel, const LogLevel& outputLevel);


  class LogStream
  {
    public:
      friend class Logger;
    private:
      /* Outputstream for the LogStream */
      std::ostream& m_stream;
      /* Loglevel that is compared with the given log level whilst outputing */
      LogLevel m_logLevel;
      /* Name of the LogStream, used to differantiate streams */
      std::string m_name;
      /* The comparison that checks if the stream should output this with a given LogLevel*/
      LevelCheck m_levelCheck;
    private:
      LogStream();
    public:

      LogStream(std::ostream& stream, LogLevel logLevel, const std::string& name);
      LogStream(std::ostream& stream, LogLevel logLevel, const std::string& name, LevelCheck levelCheck);
      virtual ~LogStream();

      /*
       * Output the given arguments to the stream with a given LogLevel
       */
      template<typename... Args>
      void Output(const LogLevel& level, const Args&... args)
      {
        // Check if the given level is allowed to be streamed.
        if (m_levelCheck(m_logLevel, level))
        {
          // Output all the arguments with an endline
          Put("[Greet]", args..., '\n');
        }
      }


      /*
       * Return the name of the stream.
       */
      const std::string& GetName() const { return m_name; }

      /*
       * Set the loglevel for the stream
       */
      void SetLogLevel(LogLevel level);

      /*
       * Checks if the stream name is equal to the given name parameter
       */
      friend bool operator==(const LogStream& stream, const std::string& name);

      /*
       * Checks if the stream name is equal to the given name parameter
       */
      friend bool operator!=(const LogStream& stream, const std::string& name);

      /*
       * Checks if the stream name is equal to the given streams name
       */
      friend bool operator==(const LogStream& stream1, const LogStream& stream2);

      /*
       * For some reason this is needed.
       */
      LogStream& operator=(const LogStream& stream);

      // Default level checks, returns true if the given stream should stream with the given output level.
      static bool EQCheck(const LogLevel& myLevel, const LogLevel& outputLevel);
      static bool NEQCheck(const LogLevel& myLevel, const LogLevel& outputLevel);
      static bool GTECheck(const LogLevel& myLevel, const LogLevel& outputLevel);
      static bool GTCheck(const LogLevel& myLevel, const LogLevel& outputLevel);
      static bool LTECheck(const LogLevel& myLevel, const LogLevel& outputLevel);
      static bool LTCheck(const LogLevel& myLevel, const LogLevel& outputLevel);

    private:


      /*
       * PutHelper function for everything
       */
      template <typename T>
      auto PutHelper(const T& t, int)
        -> decltype(m_stream << t, std::declval<void>())
      {
        m_stream << t;
      }

      /*
       * PutHelper function for Container
       */
      template <typename T>
      auto PutHelper(const T& t, long long)
        -> decltype(t.begin(), t.end(), std::declval<void>())
      {
        auto size = t.size()-1;
        for(auto it = t.begin();it!=t.end();++it,--size)
        {
          Put(*it);
          if(size != 0)
            Put(" ");
        }
      }
      /*
       * Put all the arguments to the stream.
       */
      template<typename T, typename... Args>
      void Put(const T& t, const Args&... args)
      {
        // Put one argument to the stream.
        Put(t);
        // Put the rest of the arguments to the stream (if only one, put(const T&) is called)
        Put(args...);
      }

      /*
       * Put the arguments to the stream.
       */
      template <typename T>
      void Put(const T& t)
      {
        PutHelper(t, 0);
      }

  };

}
