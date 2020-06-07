#pragma once

#include <graphics/Window.h>
#include <utils/Timer.h>
#include <event/Event.h>

namespace Greet {

  class App
  {
    protected:
      Timer* m_timer;
      uint m_fps;
      uint m_ups;
      double frameCap = 0;
    protected:
      App(const std::string& title, uint width, uint height);

      virtual ~App();

      virtual void Init() = 0;
      virtual void Tick() = 0;
      virtual void Update(float elapsedTime) = 0;
      virtual void Render() = 0;
      virtual void OnEvent(Event& event) {}

      void SetFrameCap(uint frameCap)
      {
        if (frameCap == 0)
          this->frameCap = 0;
        else
          this->frameCap = 1.0 / (double)frameCap;
      }

      uint GetFrameCap()
      {
        if (frameCap == 0)
          return 0;
        return (uint)(1.0 / frameCap);
      }

      uint GetFPS() const { return m_fps; }
      uint GetUPS() const { return m_ups; }

    public:
      void Start();

    private:
      void Run();
  };
}
