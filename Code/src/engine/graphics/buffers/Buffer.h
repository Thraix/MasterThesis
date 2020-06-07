#pragma once

#include <common/Memory.h>

namespace Greet {

  enum class BufferType
  {
    ARRAY = 0x8892, // GL_ARRAY_BUFFER
    INDEX = 0x8893, // GL_ELEMENT_ARRAY_BUFFER
  };

  enum class BufferDrawType : uint
  {
    STATIC = 0x88E4, // GL_STATIC_DRAW
    DYNAMIC = 0x88E8 // GL_DYNAMIC_DRAW
  };

  class Buffer
  {
    protected:
      uint id;
      uint dataSize;
      BufferType type;
      BufferDrawType drawType;

    protected:
      Buffer(uint dataSize, BufferType type, BufferDrawType drawType);

    public:
      virtual ~Buffer();
      void UpdateData(const void* data) const;
      void UpdateData(const void* data, uint dataSize);
      void* MapBuffer() const;
      void UnmapBuffer() const;

      void Enable() const;
      void Disable() const;

      inline uint GetDataSize() const { return dataSize; }

    public:
      static Ref<Buffer> Create(uint dataSize, BufferType type, BufferDrawType drawType);
  };
}
