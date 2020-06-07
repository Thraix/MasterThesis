#pragma once

#include <drivers/DriverListener.h>

namespace Greet {

  class DriverAdapter : public DriverListener
  {
    public:
      void OnDriverFinished() override {}
  };

}
