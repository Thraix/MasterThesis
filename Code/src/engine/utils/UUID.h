#pragma once

#include <common/Types.h>

#include <set>
#include <random>

namespace Greet {


  class UUID
  {
    public:
      friend class Window;
    private:
      std::default_random_engine m_randomEngine;
      std::uniform_int_distribution<uint> m_distribution;
      std::set<uint> m_usedUUID;

    public:
      uint GetUUID();
    public:
      static UUID& GetInstance() { return s_instance; }
    private:
      UUID();
      ~UUID() { }
      uint GetRandomNumber();
      uint GenNewUUID();
      static UUID s_instance;
  };
}
