#include "VertexBuffer.h"

#include <internal/GreetGL.h>

namespace Greet
{
  VertexBuffer::VertexBuffer(const void* data, uint size, BufferDrawType type)
    : Buffer{size, BufferType::ARRAY, type}
  {
    Enable();
    UpdateData(data);
  }

  Ref<VertexBuffer> VertexBuffer::CreateDynamic(const void* data, uint size)
  {
    return Create(data, size, BufferDrawType::DYNAMIC);
  }

  Ref<VertexBuffer> VertexBuffer::CreateStatic(const void* data, uint size)
  {
    return Create(data, size, BufferDrawType::STATIC);
  }

  Ref<VertexBuffer> VertexBuffer::Create(const void* data, uint size, BufferDrawType type)
  {
    return std::shared_ptr<VertexBuffer>(new VertexBuffer(data, size, type));
  }
}
