#include "GlobalSceneManager.h"

#include <logging/Log.h>
#include <utils/HotSwapping.h>

namespace Greet {

  SceneManager GlobalSceneManager::sceneManager;

  SceneManager& GlobalSceneManager::GetSceneManager()
  {
    return sceneManager;
  }
}
