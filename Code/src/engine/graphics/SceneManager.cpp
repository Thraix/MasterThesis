#include "SceneManager.h"

#include <logging/Log.h>
#include <utils/HotSwapping.h>

namespace Greet {

  SceneManager::SceneManager()
    : focusedScene{nullptr}
  {

  }

  void SceneManager::Add2DScene(Scene* scene, const std::string& name)
  {
    if (scene == NULL)
    {
      Log::Error("Trying to add Scene to SceneManager but it is NULL.");
      return;
    }
    m_scenes2d.push_back({name, scene});
  }

  void SceneManager::Add3DScene(Scene* scene, const std::string& name)
  {
    if (scene == NULL)
    {
      Log::Error("Trying to add Renderer3D to SceneManager but it is NULL.");
      return;
    }
    m_scenes3d.push_back({name, scene});
  }

  Scene* SceneManager::Remove2DScene(const std::string& name)
  {
    auto it = std::find_if(m_scenes2d.begin(), m_scenes2d.end(),
        [name] (const SceneElement& scene) { return scene.first == name; });
    m_scenes2d.erase(it);
    return it->second;
  }

  Scene* SceneManager::Remove3DScene(const std::string& name)
  {
    auto it = std::find_if(m_scenes3d.begin(), m_scenes3d.end(),
        [name] (const SceneElement& scene) { return scene.first == name; });
    m_scenes3d.erase(it);
    return it->second;
  }

  Scene* SceneManager::Get2DScene(const std::string& name) const
  {
    auto it = std::find_if(m_scenes2d.begin(), m_scenes2d.end(),
        [name] (const SceneElement& scene) { return scene.first == name; });
    return it->second;
  }

  Scene* SceneManager::Get3DScene(const std::string& name) const
  {
    auto it = std::find_if(m_scenes3d.begin(), m_scenes3d.end(),
        [name] (const SceneElement& scene) { return scene.first == name; });
    return it->second;
  }

  void SceneManager::Render() const
  {
    for (auto it = m_scenes3d.begin(); it != m_scenes3d.end(); it++)
    {
      it->second->PreRender();
      it->second->Render();
      it->second->PostRender();
    }

    for (auto it = m_scenes2d.begin(); it != m_scenes2d.end(); it++)
    {
      it->second->PreRender();
      it->second->Render();
      it->second->PostRender();
    }
  }

  void SceneManager::Update(float timeElapsed)
  {
    static float time = 0.0f;
    time+=timeElapsed;
    if(time > 1.0f)
    {
      HotSwapping::CheckResources();
      time-=1.0f;
    }
    for (auto it = m_scenes3d.begin(); it != m_scenes3d.end(); it++)
    {
      it->second->PreUpdate(timeElapsed);
      it->second->Update(timeElapsed);
      it->second->PostUpdate(timeElapsed);
    }

    for (auto it = m_scenes2d.begin(); it != m_scenes2d.end(); it++)
    {
      it->second->PreUpdate(timeElapsed);
      it->second->Update(timeElapsed);
      it->second->PostUpdate(timeElapsed);
    }
  }

  void SceneManager::OnEvent(Event& event)
  {
    if(focusedScene)
    {
      focusedScene->OnEvent(event);
      if(event.GetFlags() & EVENT_UNFOCUSED)
        focusedScene = nullptr;
      if(event.GetFlags()  & EVENT_HANDLED)
        return;
    }

    // The last to be rendered will be on top so check input there first.
    for (auto it = m_scenes2d.rbegin(); it != m_scenes2d.rend(); it++)
    {
      if(it->second == focusedScene)
        continue;
      it->second->OnEvent(event);
      // Generally when you request focus you also handle the input.
      // But there might be a case where that is not the case.
      if(event.GetFlags() & EVENT_FOCUSED)
        focusedScene = it->second;
      if(event.GetFlags() & EVENT_HANDLED)
        return;
    }

    for (auto it = m_scenes3d.rbegin(); it != m_scenes3d.rend(); it++)
    {
      if(it->second == focusedScene)
        continue;
      it->second->OnEvent(event);
      // Generally when you request focus you also handle the input.
      // But there might be a case where that is not the case.
      if(event.GetFlags() & EVENT_FOCUSED)
        focusedScene = it->second;
      if(event.GetFlags() & EVENT_HANDLED)
        return;
    }
  }
}
