#pragma once

#include <graphics/layers/Scene.h>
#include <vector>

namespace Greet {
  class SceneManager
  {
    typedef std::pair<std::string, Scene*> SceneElement;
    private:
      // they are the same but 3d scenes are always rendered first
    std::vector<SceneElement> m_scenes2d;
    std::vector<SceneElement> m_scenes3d;
    Scene* focusedScene;

    public:
      SceneManager();
      void Add2DScene(Scene* scene, const std::string& name);
      void Add3DScene(Scene* scene, const std::string& name);

      Scene* Remove2DScene(const std::string& name);
      Scene* Remove3DScene(const std::string& name);
      Scene* Get2DScene(const std::string& name) const;
      Scene* Get3DScene(const std::string& name) const;
      void Render() const;
      void Update(float timeElapsed);
      void OnEvent(Event& event);
  };
}
