#include "VertexArray.h"

#include <internal/GreetGL.h>

namespace Greet {

  VertexArray::VertexArray()
    : id{0}
  {
    GLCall(glGenVertexArrays(1, &id));
    Enable();
  }

  VertexArray::~VertexArray()
  {
    glDeleteVertexArrays(1, &id);
  }

  void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& buffer)
  {
    const BufferStructure& structure = buffer->GetStructure();
    for(auto&& attribute : structure)
    {
      if(attribute.location < 0)
        continue;
      GLCall(glEnableVertexAttribArray(attribute.location));
      GLCall(glVertexAttribPointer(
            attribute.location,
            BufferAttributeToCount(attribute.type),
            BufferAttributeToGLPrimitive(attribute.type),
            attribute.normalize ? GL_TRUE : GL_FALSE,
            structure.GetVertexSize(),
            (void*)attribute.offset
            ));
    }
    buffers.push_back(buffer);
  }

  void VertexArray::SetIndexBuffer(const Ref<Buffer>& indexBuffer)
  {
    this->indexBuffer = indexBuffer;
  }

  void VertexArray::Enable() const
  {
    GLCall(glBindVertexArray(id));
    if(indexBuffer)
      indexBuffer->Enable();
  }

  void VertexArray::Render(DrawType type, uint count)
  {
    GLCall(glDrawElements((uint)type, count * sizeof(uint), GL_UNSIGNED_INT, nullptr));
  }

  void VertexArray::Disable()
  {
    if(indexBuffer)
      indexBuffer->Disable();
    GLCall(glBindVertexArray(0));
  }

  Ref<VertexArray> VertexArray::Create()
  {
    return std::shared_ptr<VertexArray>(new VertexArray());
  }
}
