#pragma once

#include <drivers/Driver1.h>

namespace Greet {

  class LinearDriver : public Driver1
  {
    public:
      float m_speed;
    public:

      LinearDriver(float& value, float change, float time, bool relativeChange, DriverListener* listener);
      void Update() override;
  };

}
