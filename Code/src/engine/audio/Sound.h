#if 0
#pragma once

#include <string>
#include <logging/Log.h>
#include <audio/AudioChannelManager.h>
#include <audio/AudioChannel.h>

#include <ga.h>
#include <gau.h>

namespace Greet{

  class Sound
  {
    private:
      AudioChannel* m_channel;
      ga_Sound *m_sound;
      ga_Handle *m_handle;
      std::string m_name;
      bool m_playing;

    public:
      Sound(const std::string& filename, const std::string& name, std::string channelName);
      Sound(const std::string& filename, const std::string& name, AudioChannel* channel);
      ~Sound();

      void Play();
      void Loop();
      void Pause();
      void Resume();
      void Stop();
      void RefreshChannel();

      inline const AudioChannel* GetAudioChannel() const { return m_channel; }
      inline const std::string& GetName() const { return m_name; }
      inline const bool IsPlaying() const { return m_playing; }

      static void destroy_on_finish(ga_Handle* in_handle, void* in_context);
      static void loop_on_finish(ga_Handle* in_handle, void* in_context);
  };
}
#endif
