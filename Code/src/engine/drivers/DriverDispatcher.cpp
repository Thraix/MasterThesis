#include "DriverDispatcher.h"

namespace Greet {

  std::vector<Driver*> DriverDispatcher::m_drivers;

  void DriverDispatcher::AddDriver(Driver* driver)
  {
    m_drivers.push_back(driver);
  }

  void DriverDispatcher::Update(float timeElapsed)
  {
    for (std::vector<Driver*>::iterator it = m_drivers.begin(); it != m_drivers.end();)
    {
      (*it)->Preupdate(timeElapsed);
      (*it)->Update();
      if ((*it)->Postupdate())
      {
        delete *it;
        it = m_drivers.erase(it);
      }
      else
        ++it;
    }
  }

}
