#include "UUID.h"

#include <graphics/Window.h>
#include <logging/Log.h>

#include <ctime>

namespace Greet {

  UUID UUID::s_instance;

  UUID::UUID()
    : m_randomEngine(time(NULL)), m_distribution()
  {
  }

  uint UUID::GetUUID()
  {
    return GenNewUUID();
  }

  // generates a random 32 bit number
  uint UUID::GetRandomNumber()
  {
    return m_distribution(m_randomEngine);
  }

  uint UUID::GenNewUUID()
  {
    uint i = 1000;
    uint number;
    while (i--)
    {
      number = GetRandomNumber();
      if (number != 0 && m_usedUUID.count(number) == 0)
      {
        m_usedUUID.insert(number);
        return number;
      }
    }
    Log::Warning("Could not generate random UUID. Try again.");
    return 0;
  }
}
