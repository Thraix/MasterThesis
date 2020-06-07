#include "Logger.h"

#include <logging/Log.h>

namespace Greet {

  Logger::Logger()
  {

  }

  Logger::~Logger()
  {
    for (std::vector<LogStream*>::iterator it = m_streams.begin(); it != m_streams.end(); it++)
    {
      delete *it;
    }
    m_streams.clear();
  }

  void Logger::AddLogStream(LogStream* stream)
  {
    // Check if there already is a stream with the given stream name.
    if (GetLogStream(stream->GetName()))
    {
      Log::Error("Stream with name ", stream->GetName(), " already exists.");
    }
    else
      m_streams.push_back(stream);
  }

  LogStream* Logger::GetLogStream(const std::string& name)
  {
    for (std::vector<LogStream*>::iterator it = m_streams.begin(); it != m_streams.end(); it++)
    {
      // it -> pointer to pointer to stream
      if (**it == name)
      {
        return *it;
      }
    }
    return NULL;
  }

  LogStream* Logger::RemoveLogStream(const std::string& name)
  {
    for (std::vector<LogStream*>::iterator it = m_streams.begin(); it != m_streams.end(); it++)
    {
      if (**it == name)
      {
        m_streams.erase(it);
        return *it;
      }
    }
    Log::Warning("No stream removed with name: ", name, ". Could not find.");
    return nullptr;
  }

  LogStream* Logger::RemoveLogStream(LogStream* stream)
  {
    return RemoveLogStream(stream->GetName());
  }

}
