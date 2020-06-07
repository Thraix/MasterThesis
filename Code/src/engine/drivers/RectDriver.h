#pragma once

#include <drivers/Driver4.h>

namespace Greet {

  class RectDriver : public Driver4
  {
    private:
      float m_speed1;
      float m_speed2;
      float m_speed3;
      float m_speed4;
    public:
      RectDriver(float& x, float& y, float& width, float& height, float dx, float dy, float dwidth, float dheight, float time, bool relativeChange, DriverListener* listener);
      void Update() override;
  };

}
