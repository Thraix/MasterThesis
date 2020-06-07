#pragma once

#include <drivers/Driver.h>

namespace Greet {

  class Driver4 : public Driver
  {
    protected:
      float& m_value1;
      float m_start1;

      float& m_value2;
      float m_start2;

      float& m_value3;
      float m_start3;

      float& m_value4;
      float m_start4;

    public:
      Driver4(float& value1, float& value2, float& value3, float& value4, float time, DriverListener* listener)
        : Driver(time, listener), m_value1(value1), m_start1(value1), m_value2(value2), m_start2(value2), m_value3(value3), m_start3(value3), m_value4(value4), m_start4(value4)
      {}
  };

}
