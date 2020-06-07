#include "Buffer.h"

#include <internal/GreetGL.h>

namespace Greet {

  Buffer::Buffer(uint dataSize, BufferType type, BufferDrawType drawType)
    : id{0}, dataSize{dataSize}, type{type}, drawType{drawType}
  {
    GLCall(glGenBuffers(1, &id));
    Enable();
  }

  Buffer::~Buffer()
  {
    GLCall(glDeleteBuffers(1, &id));
  }

  void Buffer::Enable() const
  {
    GLCall(glBindBuffer((uint)type, id));
  }

  void Buffer::UpdateData(const void* data) const
  {
    GLCall(glBufferData((uint)type, dataSize, data, (uint)drawType));
  }

  void Buffer::UpdateData(const void* data, uint dataSize)
  {
    this->dataSize = dataSize;
    GLCall(glBufferData((uint)type, dataSize, data, (uint)drawType));
  }

  void* Buffer::MapBuffer() const
  {
    GLCall(void* buffer = glMapBuffer((uint)type, GL_WRITE_ONLY));
    return buffer;
  }

  void Buffer::UnmapBuffer() const
  {
    GLCall(glUnmapBuffer((uint)type));
  }

  void Buffer::Disable() const
  {
    GLCall(glBindBuffer((uint)type, 0));
  }

  Ref<Buffer> Buffer::Create(uint dataSize, BufferType type, BufferDrawType drawType)
  {
    return std::shared_ptr<Buffer>(new Buffer(dataSize, type, drawType));
  }
}
