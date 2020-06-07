#if 0
#include "Sound.h"

#include <audio/SoundManager.h>

namespace Greet {

  Sound::Sound(const std::string& filename, const std::string& name, std::string channelName)
    : m_name(name), m_channel(AudioChannelManager::Get(channelName)), m_playing(false)
  {
    std::vector<std::string> split = StringUtils::split_string(filename,".");
    if (split.size() < 2){
      Log::Error("Invalid file name: ", filename.c_str());
      return;
    }
    m_sound = gau_load_sound_file(("res/sounds/" + filename).c_str(), split.back().c_str());
    if (m_sound == nullptr)
    {
      Log::Error("Could not load file: ", filename.c_str());
      return;
    }

  }

  Sound::Sound(const std::string& filename, const std::string& name, AudioChannel* channel)
    : m_name(name), m_channel(channel), m_playing(false)
  {
    m_sound = gau_load_sound_file(("res/sounds/" + filename).c_str(),"wav");
  }

  Sound::~Sound()
  {
    ga_sound_release(m_sound);
  }

  void Sound::Play()
  {
    gc_int32 quit = 0;
    m_handle = gau_create_handle_sound(SoundManager::m_mixer, m_sound, &destroy_on_finish, &quit, NULL);
    m_handle->sound = this;
    RefreshChannel();
    m_playing = true;
    ga_handle_play(m_handle);
  }

  void Sound::Loop()
  {
    gc_int32 quit = 0;
    m_handle = gau_create_handle_sound(SoundManager::m_mixer, m_sound, &loop_on_finish, &quit, NULL);
    m_handle->sound = this;
    m_playing = false;
    RefreshChannel();
    m_playing = true;
    ga_handle_play(m_handle);
  }

  void Sound::Pause()
  {
    if (!m_playing)
      return;

    m_playing = false;
    ga_handle_stop(m_handle);
  }

  void Sound::Resume()
  {
    if (m_playing)
      return;

    m_playing = true;
    ga_handle_play(m_handle);
  }

  void Sound::Stop()
  {
    if (!m_playing)
      return;

    ga_handle_stop(m_handle);
    //ga_handle_destroy(m_handle);
    m_playing = false;
  }

  void Sound::RefreshChannel()
  {
    if (m_playing)
      return;
    ga_handle_setParamf(m_handle, GA_HANDLE_PARAM_GAIN, m_channel->GetVolume());
    ga_handle_setParamf(m_handle, GA_HANDLE_PARAM_PITCH, m_channel->GetPitch());
    ga_handle_setParamf(m_handle, GA_HANDLE_PARAM_PAN, m_channel->GetPan());
  }

  void Sound::destroy_on_finish(ga_Handle* in_handle, void* in_context)
  {
    ga_handle_destroy(in_handle);
    ((Sound*)in_handle->sound)->Stop();
  }

  void Sound::loop_on_finish(ga_Handle* in_handle, void* in_context)
  {
    Sound* sound = (Sound*)in_handle->sound;
    sound->Loop();
    ga_handle_destroy(in_handle);
  }

}
#endif
