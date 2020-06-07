#pragma once

#include <drivers/Driver.h>

namespace Greet {

  class Driver2 : public Driver
  {
    protected:
      float& m_value1;
      float m_start1;

      float& m_value2;
      float m_start2;

    public:
      Driver2(float& value1, float& value2, float time, DriverListener* listener)
        : Driver(time, listener), m_value1(value1), m_start1(value1), m_value2(value2), m_start2(value2)
      {}
  };
}
