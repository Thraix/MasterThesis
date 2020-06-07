#include "Log.h"

namespace Greet {

  Log Log::m_log;
  Log::Log()
  {
    m_logger.AddLogStream(new LogStream(std::cout, LogLevel::Information(), "console"));
  }

  Log::~Log()
  {
  }

  void Log::AddLogStream(LogStream* stream)
  {
    m_log.m_logger.AddLogStream(stream);
  }

  LogStream* Log::RemoveLogStream(const std::string& name)
  {
    return m_log.m_logger.RemoveLogStream(name);
  }

  LogStream* Log::RemoveLogStream(LogStream* stream)
  {
    return m_log.m_logger.RemoveLogStream(stream);
  }

}
