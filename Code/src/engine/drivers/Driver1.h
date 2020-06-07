#pragma once

#include <drivers/Driver.h>

namespace Greet {

  class Driver1 : public Driver
  {
    protected:
      float& m_value;
      float m_start;

    public:
      Driver1(float& value, float time, DriverListener* listener)
        : Driver(time,listener), m_value(value), m_start(value)
      {}
  };

}
