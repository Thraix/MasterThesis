#pragma once

#include <drivers/DriverListener.h>
#include <common/Types.h>

namespace Greet {

  class Driver
  {
    friend class DriverDispatcher;
    protected:
      float m_time;
      float m_elapsed;
      float m_elapsedSinceStart;
      DriverListener* m_listener;
    protected:

      Driver(float time, DriverListener* listener);


      void Preupdate(float timeElapsed);
      virtual void Update() = 0;
      bool Postupdate();
      void OnFinished();
  };

}
