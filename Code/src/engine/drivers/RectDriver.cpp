#include "RectDriver.h"

#include <logging/Logger.h>

namespace Greet {

  RectDriver::RectDriver(float& x, float& y, float& width, float& height, float dx, float dy, float dwidth, float dheight, float time, bool relativeChange, DriverListener* listener)
    : Driver4(x,y,width,height, time, listener)
  {
    if (relativeChange)
    {
      m_speed1 = dx / m_time;
      m_speed2 = dy / m_time;
      m_speed3 = dwidth / m_time;
      m_speed4 = dheight / m_time;
    }
    else
    {
      m_speed1 = (dx - m_start1) / m_time;
      m_speed2 = (dy - m_start1) / m_time;
      m_speed3 = (dwidth - m_start1) / m_time;
      m_speed4 = (dheight - m_start1) / m_time;
    }
  }

  void RectDriver::Update()
  {
    m_value1 += m_elapsed * m_speed1;
    m_value2 += m_elapsed * m_speed2;
    m_value3 += m_elapsed * m_speed3;
    m_value4 += m_elapsed * m_speed4;
  }
}
