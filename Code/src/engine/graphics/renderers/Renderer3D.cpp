#include "Renderer3D.h"

namespace Greet{

  void Renderer3D::BindMatrices(const Ref<Shader>& shader, const Camera* camera, bool shouldBindShader) const
  {
    if(shouldBindShader)
      shader->Enable();

    shader->SetUniformMat4("viewMatrix", camera->GetViewMatrix());
    shader->SetUniformMat4("projectionMatrix",camera->GetProjectionMatrix());
  }
}
