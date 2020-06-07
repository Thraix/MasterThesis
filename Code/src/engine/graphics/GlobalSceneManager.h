#pragma once

#include <vector>
#include <graphics/layers/Scene.h>
#include <graphics/SceneManager.h>
#include <event/Event.h>

namespace Greet {
  class GlobalSceneManager
  {
    private:
      static SceneManager sceneManager;
    public:
      static SceneManager& GetSceneManager();
  };
}
