#include "AudioChannelManager.h"

#include <logging/Log.h>
#include <utils/ErrorHandle.h>

namespace Greet{

  std::vector<AudioChannel*> AudioChannelManager::m_channels;

  void AudioChannelManager::Add(AudioChannel* channel)
  {
    for (int i = 0;i < m_channels.size();i++)
    {
      if (channel->GetName() == m_channels[i]->GetName())
      {
        ErrorHandle::SetErrorCode(GREET_ERROR_MANAGER_ADD);
        Log::Error("Given channel name already exists: ", channel->GetName().c_str());
        return;
      }
    }
    m_channels.push_back(channel);
  }

  AudioChannel* AudioChannelManager::Get(const std::string& channelname)
  {
    uint size = m_channels.size();

    for (uint i = 0; i < size; i++)
      if (channelname.compare(m_channels[i]->GetName().c_str()) == 0)
        return m_channels[i];
    ErrorHandle::SetErrorCode(GREET_ERROR_MANAGER_GET);
    Log::Error("Could not find the given channel: ", channelname.c_str());
    return m_channels[0];
  }

  void AudioChannelManager::Destroy()
  {
    for (uint i = 0; i < m_channels.size(); i++)
      delete m_channels[i];
  }

}
