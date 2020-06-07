#include "LogStream.h"

#include <iostream>
#include <logging/Log.h>

namespace Greet {

  LogStream::LogStream()
    : m_stream(std::cout), m_logLevel(LogLevel::Information()), m_name(""), m_levelCheck(*GTECheck)
  {

  }


  LogStream::LogStream(std::ostream& stream, LogLevel logLevel, const std::string& name)
    : m_stream(stream), m_logLevel(logLevel), m_name(name), m_levelCheck(*GTECheck)
  {

  }

  LogStream::LogStream(std::ostream& stream, LogLevel logLevel, const std::string& name, LevelCheck levelCheck)
    : m_stream(stream), m_logLevel(logLevel), m_name(name), m_levelCheck(levelCheck)
  {

  }

  LogStream::~LogStream()
  {

  }

  void LogStream::SetLogLevel(LogLevel level)
  {
    m_logLevel = level;
  }

  bool operator==(const LogStream& stream, const std::string& name)
  {
    return !stream.m_name.compare(name);
  }

  bool operator!=(const LogStream& stream, const std::string& name)
  {
    return stream.m_name.compare(name);
  }

  bool operator==(const LogStream& stream1, const LogStream& stream2)
  {
    return !stream1.m_name.compare(stream2.m_name);
  }

  LogStream& LogStream::operator=(const LogStream& stream)
  {
    std::cout << "no copyng cunt" << std::endl;
    ASSERT(false, "Cannot copy LogStream");
    return *this;
  }

  bool LogStream::EQCheck(const LogLevel& myLevel, const LogLevel& outputLevel)
  {
    return myLevel == outputLevel;
  }

  bool LogStream::NEQCheck(const LogLevel& myLevel, const LogLevel& outputLevel)
  {
    return myLevel != outputLevel;
  }

  bool LogStream::GTECheck(const LogLevel& myLevel, const LogLevel& outputLevel)
  {
    return myLevel >= outputLevel;
  }

  bool LogStream::GTCheck(const LogLevel& myLevel, const LogLevel& outputLevel)
  {
    return myLevel > outputLevel;
  }

  bool LogStream::LTECheck(const LogLevel& myLevel, const LogLevel& outputLevel)
  {
    return myLevel <= outputLevel;
  }

  bool LogStream::LTCheck(const LogLevel& outputLevel, const LogLevel& myLevel)
  {
    return myLevel < outputLevel;
  }

}
