#pragma once

#include <common/Types.h>
#include <common/Memory.h>
#include <graphics/buffers/Buffer.h>
#include <logging/Log.h>

namespace Greet
{
  enum class BufferAttributeType
  {
    FLOAT, VEC2, VEC3, VEC4, MAT3, MAT4, UINT, UBYTE4, BYTE4
  };

  static uint BufferAttributeToCount(BufferAttributeType type)
  {
    switch(type)
    {
      case BufferAttributeType::FLOAT:  return 1;
      case BufferAttributeType::VEC2:   return 2;
      case BufferAttributeType::VEC3:   return 3;
      case BufferAttributeType::VEC4:   return 4;
      case BufferAttributeType::MAT3:   return 9;
      case BufferAttributeType::MAT4:   return 16;
      case BufferAttributeType::UINT:   return 1;
      case BufferAttributeType::UBYTE4: return 4;
      case BufferAttributeType::BYTE4:  return 4;
    }

    ASSERT(false, "Unsupported BufferAttributeType");
    return 0;
  }

  static uint BufferAttributeToByteSize(BufferAttributeType type)
  {
    switch(type)
    {
      case BufferAttributeType::FLOAT:  return 1 * sizeof(float);
      case BufferAttributeType::VEC2:   return 2 * sizeof(float);
      case BufferAttributeType::VEC3:   return 3 * sizeof(float);
      case BufferAttributeType::VEC4:   return 4 * sizeof(float);
      case BufferAttributeType::MAT3:   return 9 * sizeof(float);
      case BufferAttributeType::MAT4:   return 16 * sizeof(float);
      case BufferAttributeType::UINT:   return 1 * sizeof(int);
      case BufferAttributeType::UBYTE4: return 4 * sizeof(char);
      case BufferAttributeType::BYTE4:  return 4 * sizeof(char);
    }

    ASSERT(false, "Unsupported BufferAttributeType");
    return 0;
  }

  static uint BufferAttributeToGLPrimitive(BufferAttributeType type)
  {
    switch(type)
    {
      case BufferAttributeType::FLOAT:  return 0x1406; // GL_FLOAT
      case BufferAttributeType::VEC2:   return 0x1406; // GL_FLOAT
      case BufferAttributeType::VEC3:   return 0x1406; // GL_FLOAT
      case BufferAttributeType::VEC4:   return 0x1406; // GL_FLOAT
      case BufferAttributeType::MAT3:   return 0x1406; // GL_FLOAT
      case BufferAttributeType::MAT4:   return 0x1406; // GL_FLOAT
      case BufferAttributeType::UINT:   return 0x1405; // GL_UNSIGNED_INT
      case BufferAttributeType::UBYTE4: return 0x1401; // GL_UNSIGNED_BYTE
      case BufferAttributeType::BYTE4:  return 0x1400; // GL_BYTE
    }

    ASSERT(false, "Unsupported BufferAttributeType");
    return 0;
  }

  struct BufferAttribute
  {
    int location;
    BufferAttributeType type;
    uint size;
    unsigned long long offset;
    bool normalize;

    BufferAttribute(int location, BufferAttributeType type, bool normalize = false)
      : location{location}, type{type}, size{BufferAttributeToByteSize(type)}, offset{0}, normalize{normalize}
    {}
  };

  class BufferStructure
  {
    private:
      std::vector<BufferAttribute> attributes;
      uint vertexSize;

    public:
      BufferStructure() {}
      BufferStructure(const std::initializer_list<BufferAttribute>& attributes)
        : attributes{attributes}, vertexSize{0}
      {
        uint offset = 0;
        for(auto&& attribute : this->attributes)
        {
          attribute.offset = offset;
          offset += attribute.size;
        }
        vertexSize = offset;
      }

      uint GetVertexSize() const
      {
        return vertexSize;
      }

      std::vector<BufferAttribute>::iterator begin() { return attributes.begin(); }
      std::vector<BufferAttribute>::iterator end() { return attributes.end(); }
      std::vector<BufferAttribute>::const_iterator begin() const { return attributes.begin(); }
      std::vector<BufferAttribute>::const_iterator end() const { return attributes.end(); }
  };

  class VertexBuffer : public Buffer
  {
    private:
      BufferStructure structure;
    public:
      VertexBuffer(const void* data, uint size, BufferDrawType type);
      virtual ~VertexBuffer() {}

      void SetStructure(const BufferStructure& structure) { this->structure = structure; }
      const BufferStructure& GetStructure() const { return structure; }

      static Ref<VertexBuffer> CreateDynamic(const void* data, uint size);
      static Ref<VertexBuffer> CreateStatic(const void* data, uint size);
      static Ref<VertexBuffer> Create(const void* data, uint size, BufferDrawType type = BufferDrawType::STATIC);
  };
}
