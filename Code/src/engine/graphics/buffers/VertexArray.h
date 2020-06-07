#pragma once

#include <vector>
#include <common/Types.h>
#include <common/Memory.h>
#include <graphics/buffers/VertexBuffer.h>

namespace Greet {

  enum class DrawType
  {
    TRIANGLES = 0x0004, LINES = 0x0001
  };

  class VertexArray final
  {
    private:
      uint id;
      std::vector<Ref<VertexBuffer>> buffers;
      Ref<Buffer> indexBuffer;
    private:
      VertexArray();
    public:
      ~VertexArray();

      void Enable() const;
      void Disable();
      void AddVertexBuffer(const Ref<VertexBuffer>& buffer);
      void SetIndexBuffer(const Ref<Buffer>& indexBuffer);

      void Render(DrawType type, uint count);

    public:
      static Ref<VertexArray> Create();
  };

}
