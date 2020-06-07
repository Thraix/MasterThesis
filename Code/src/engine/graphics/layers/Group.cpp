#include "Group.h"

namespace Greet {

  Group::Group(const Vec2& position)
    : m_transformationMatrix(Mat3::Translate(position))
  {
    enable = true;
  }

  Group::Group(const Mat3& transform)
    :m_transformationMatrix(transform)
  {
    enable = true;
  }

  Group::~Group()
  {
    uint size = m_renderables.size();
    for (uint i = 0; i < size; i++)
      delete m_renderables[i];
  }

  void Group::Add(Renderable* renderable)
  {
    m_renderables.push_back(renderable);
  }

  void Group::Remove(Renderable* renderable)
  {
    for (auto it = m_renderables.begin();it != m_renderables.end();it++)
    {
      if (*it == renderable)
      {
        m_renderables.erase(it);
        return;
      }
    }
  }

  bool Group::Update(float timeElapsed)
  {
    bool update = false;
    if (enable)
      for (uint i = 0; i < m_renderables.size(); i++)
        update |= m_renderables[i]->Update(timeElapsed);
    return update;
  }

  void Group::Begin(Renderer2D* renderer) const
  {
    if (render)
      renderer->PushMatrix(m_transformationMatrix);
  }

  void Group::Submit(Renderer2D* renderer) const
  {
    if (render)
      for (uint i = 0; i < m_renderables.size(); i++)
      {
        m_renderables[i]->Begin(renderer);
        m_renderables[i]->Submit(renderer);
        m_renderables[i]->End(renderer);
      }
  }

  void Group::End(Renderer2D* renderer) const
  {
    if (render)
      renderer->PopMatrix();
  }

}
