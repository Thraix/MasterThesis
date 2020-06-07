#include "AudioChannel.h"

#include <audio/SoundManager.h>

namespace Greet{

  AudioChannel::AudioChannel(std::string name, float volume, float pitch, float pan)
    : m_name(name), m_volume(volume), m_pitch(pitch), m_pan(pan)
  {

  }

  AudioChannel::AudioChannel(std::string name, float volume)
    : m_name(name), m_volume(volume), m_pitch(1), m_pan(0)
  {

  }

  AudioChannel::AudioChannel(std::string name)
    : m_name(name), m_volume(1), m_pitch(1), m_pan(0)
  {

  }

  void AudioChannel::SetVolume(float volume)
  {
    m_volume = volume;
  }

  void AudioChannel::SetPitch(float pitch)
  {
    m_pitch = pitch;
  }

  void AudioChannel::SetPan(float pan)
  {
    m_pan = pan;
  }
}
