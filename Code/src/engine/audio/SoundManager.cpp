#if 0
#include "SoundManager.h"

#define GAU_THREAD_POLICY_MULTI 2

namespace Greet{

  gau_Manager* SoundManager::m_manager = nullptr;
  ga_Mixer* SoundManager::m_mixer = nullptr;
  std::vector<Sound*> SoundManager::m_sounds;

  void SoundManager::Init()
  {
    gc_initialize(0);
    m_manager = gau_manager_create();
    m_mixer = gau_manager_mixer(m_manager);
  }

  void SoundManager::Add(Sound* sound)
  {
    for (int i = 0;i < m_sounds.size();i++)
    {
      if (sound->GetName() == m_sounds[i]->GetName())
      {
        ErrorHandle::SetErrorCode(GREET_ERROR_MANAGER_ADD);
        Log::Error("Given sound name already exists: ", sound->GetName().c_str());
        return;
      }
    }
    m_sounds.push_back(sound);
  }

  Sound* SoundManager::Get(const std::string& soundname)
  {
    uint size = m_sounds.size();

    for (uint i = 0; i < size; i++)
      if (soundname.compare(m_sounds[i]->GetName().c_str()) == 0)
        return m_sounds[i];
    ErrorHandle::SetErrorCode(GREET_ERROR_MANAGER_GET);
    Log::Error("Could not find the given sound: ", soundname.c_str());
    return m_sounds[0];
  }

  void SoundManager::Destroy()
  {
    for (uint i = 0; i < m_sounds.size(); i++)
      delete m_sounds[i];

    gau_manager_destroy(m_manager);
    gc_shutdown();
  }

  void SoundManager::Update()
  {
    gau_manager_update(m_manager);
  }

}
#endif
