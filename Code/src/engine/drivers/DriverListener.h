#pragma once


namespace Greet {

  class DriverListener
  {
    public:
      virtual void OnDriverFinished() = 0;
      virtual void OnDriverUpdated() {};
  };

}
