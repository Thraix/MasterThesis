#include "Driver.h"

#include <logging/Log.h>

namespace Greet {
  Driver::Driver(float time, DriverListener* listener)
    : m_time(time), m_elapsed(0), m_listener(listener)
  {

  }

  void Driver::Preupdate(float timeElapsed)
  {
    m_elapsedSinceStart += timeElapsed;
    m_elapsed = timeElapsed;
    if (m_elapsedSinceStart >= m_time)
    {
      m_elapsed = m_time - m_elapsedSinceStart + timeElapsed;
      m_elapsedSinceStart = m_time;
    }
    m_listener->OnDriverUpdated();
  }

  bool Driver::Postupdate()
  {
    if (m_elapsedSinceStart >= m_time)
    {
      OnFinished();
      return true;
    }
    return false;
  }

  void Driver::OnFinished()
  {
    m_listener->OnDriverFinished();
  }
}
