#pragma once

#include <chrono>

namespace Greet {
  class Timer
  {
    private:

      std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
    public:
      Timer()
      {
        m_start = std::chrono::high_resolution_clock::now();
      }

      void Reset()
      {
        m_start = std::chrono::high_resolution_clock::now();
      }

      float Elapsed()
      {
        return std::chrono::duration_cast<std::chrono::duration<float,std::milli>>(std::chrono::high_resolution_clock::now() - m_start).count() / 1000.0f;
      }
  };
}

#define TIMETEST(x) Timer timer; x; Log::Info("Time Elapsed: ", timer.Elapsed());
