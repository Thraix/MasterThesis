#pragma once

#include <graphics/gui/Component.h>
#include <graphics/SceneManager.h>

namespace Greet
{
  class SceneView : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(SceneView);

      SceneManager sceneManager;
    public:
      SceneView(const XMLObject& xmlObject, Component* parent);

      void Update(float timeElapsed) override;
      void Render(GUIRenderer* renderer) const override;
      void OnEventHandler(Event& event, const Vec2& componentPos) override;
      void OnMeasured() override;

      const SceneManager& GetSceneManager() const;
      SceneManager& GetSceneManager();
  };
}

