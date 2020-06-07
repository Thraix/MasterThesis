#if 0
#pragma once

#include <vector>
#include <audio/Sound.h>
#include <common/Types.h>
#include <utils/ErrorHandle.h>

#include <ga.h>
#include <gau.h>

namespace Greet {
  class SoundManager
  {
    private:
      friend class Sound;
      static gau_Manager* m_manager;
      static ga_Mixer* m_mixer;
      static std::vector<Sound*> m_sounds;
    public:
      static void Init();
      static void Add(Sound* sound);
      static Sound* Get(const std::string& soundname);
      static void Destroy();
      static void Update();
    private:
      SoundManager(){}
  };
}
#endif
