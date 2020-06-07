#pragma once

#include <vector>
#include <audio/AudioChannel.h>

namespace Greet {
  class AudioChannelManager
  {
    private:
      static std::vector<AudioChannel*> m_channels;
    public:
      static void Add(AudioChannel* channel);
      static AudioChannel* Get(const std::string& channelname);
      static void Destroy();
    private:
      AudioChannelManager(){}
  };
}
