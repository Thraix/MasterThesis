#pragma once

namespace Greet {

  class LogLevel
  {
    private:
      // Level of the LogLevel
      unsigned int m_level;
    public:

      LogLevel(unsigned int level) : m_level(level) {}

      /*
         sets the level of the LogLevel
         */
      void SetLevel(unsigned int level) { m_level = level; }

      /*
         Returns the level of the LogLevel
         */
      unsigned int GetLevel() const { return m_level; }

      // Comparing LogLevels
      friend bool operator==(const LogLevel& level1, const LogLevel& level2) { return level1.m_level == level2.m_level; }
      friend bool operator!=(const LogLevel& level1, const LogLevel& level2) { return level1.m_level != level2.m_level; }
      friend bool operator>=(const LogLevel& level1, const LogLevel& level2) { return level1.m_level >= level2.m_level; }
      friend bool operator>(const LogLevel& level1, const LogLevel& level2) { return level1.m_level > level2.m_level; }
      friend bool operator<=(const LogLevel& level1, const LogLevel& level2) { return level1.m_level <= level2.m_level; }
      friend bool operator<(const LogLevel& level1, const LogLevel& level2) { return level1.m_level < level2.m_level; }

      // Low level -> high priority
      static LogLevel Information() {
        return (1 << 24);
      }
      // Low level -> high priority
      static LogLevel Warning() {
        return (1 << 16);
      }
      // Low level -> high priority
      static LogLevel Error() {
        return (1 << 8);
      }
  };
}
