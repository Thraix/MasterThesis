#include "LinearDriver.h"

#include <logging/Logger.h>

namespace Greet {

  LinearDriver::LinearDriver(float& value, float change, float time, bool relativeChange, DriverListener* listener)
    : Driver1(value, time, listener)
  {
    if (relativeChange)
      m_speed = change / m_time;
    else
      m_speed = (change - m_start) / m_time;
  }

  void LinearDriver::Update()
  {
    m_value += m_elapsed * m_speed;
  }
}
