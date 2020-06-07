#include "Mesh.h"

#include <utils/ErrorHandle.h>
#include <internal/GreetGL.h>
#include <graphics/RenderCommand.h>
#include <logging/Logger.h>


namespace Greet {

  Mesh::Mesh(const Pointer<Vec3<float>>& vertices, const Pointer<uint>& indices)
    : m_clockwise(false)
  {
    m_drawMode = DrawType::TRIANGLES;
    m_vertexCount = vertices.Size();
    m_indexCount = indices.Size();

    vao = VertexArray::Create();
    ibo = Buffer::Create((uint)(indices.Size()*sizeof(uint)), BufferType::INDEX, BufferDrawType::STATIC);
    ibo->UpdateData((void*)indices.Data());

    // Attributes
    AddAttribute({MESH_VERTICES_LOCATION, BufferAttributeType::VEC3}, vertices); // vertices

    // Set default color to white
    GLCall(glVertexAttrib4f(MESH_COLORS_LOCATION,1.0f,1.0f,1.0f,1.0f));

    // Unbind
    ibo->Disable();
    vao->SetIndexBuffer(ibo);
    vao->Disable();
  }

  Mesh::Mesh(const MeshData& data)
    : Mesh(data.GetVertices(), data.GetIndices())
  {
    vao->Enable();
    for (auto&& [attribute, bufferData] : data.m_data)
    {
      AddAttribute(attribute, bufferData);
    }
    vao->Disable();
  }

  void Mesh::Render() const
  {
    vao->Render(m_drawMode, m_indexCount);
  }

  void Mesh::Bind() const
  {
    RenderCommand::EnableCulling(m_culling);
    RenderCommand::SetCullFace(m_clockwise ? CullFaceDirection::CW : CullFaceDirection::CCW);

    if(wireframe)
    {
      GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    }

    vao->Enable();
  }

  void Mesh::Unbind() const
  {
    vao->Disable();
    RenderCommand::ResetCulling();
    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
  }

  void Mesh::AddAttribute(const BufferAttribute& attribute, const Pointer<char>& data)
  {
    Ref<VertexBuffer> buffer = VertexBuffer::Create(data.Data(), data.ByteSize());
    buffer->SetStructure({attribute});
    vao->AddVertexBuffer(buffer);
  }

  bool Mesh::HasVBO(uint location) const
  {
    if (m_vbos.find(location) != m_vbos.end())
    {
      ErrorHandle::SetErrorCode(GREET_ERROR_MESH_LOCATION);
      Log::Error("Shader location already used in mesh: ",location);
      return true;
    }
    return false;
  }

  void Mesh::SetDefaultAttribute4f(uint location, const Vec4& data)
  {
    vao->Enable();
    GLCall(glVertexAttrib4f(location,data.x,data.y,data.z,data.w));
    vao->Disable();
  }

  void Mesh::SetDefaultAttribute3f(uint location, const Vec3<float>& data)
  {
    vao->Enable();
    GLCall(glVertexAttrib3f(location,data.x,data.y,data.z));
    vao->Disable();
  }
}
