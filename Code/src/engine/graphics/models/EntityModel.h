#pragma once

#include <graphics/models/Material.h>
#include <graphics/models/Mesh.h>

namespace Greet {

  class Renderer3D;

  class EntityModel {
    private:
      Material* material;
      Mesh* mesh;
    private:
      Vec3<float> m_position;
      Vec3<float> m_scale;
      Vec3<float> m_rotation;
      Mat4 m_transformationMatrix;
      bool m_hasChanged;
    public:
      EntityModel(Mesh* mesh, Material* material, float x, float y, float z, float sx, float sy, float sz, float rx, float ry, float rz);
      EntityModel(Mesh* mesh, Material* material, Vec3<float> position, Vec3<float> scale, Vec3<float> rotation);
      EntityModel(Mesh* mesh, Material* material);

      virtual void PreRender() const;
      virtual void Render(const Renderer3D* renderer, const Camera* camera) const;
      virtual void PostRender() const;

      virtual void BindShader(const Renderer3D* renderer, const Camera* camera) const;
      virtual void UnbindShader(const Renderer3D* renderer, const Camera* camera) const;
      virtual void BindTransformation() const;

      Material* GetMaterial() const { return material; }
      void SetMaterial(Material* material) { this->material = material;}
      Mesh* GetMesh() const { return mesh; }
      void SetMesh(Mesh* mesh) { this->mesh = mesh;}
      const Mat4& GetTransformationMatrix() const { return m_transformationMatrix; }
      virtual void Update(float timeElapsed);
      void UpdateTransformation();
      void SetPosition(const Vec3<float>& pos) { m_position = pos; m_hasChanged = true; }
      void SetScale(const Vec3<float>& scale) { m_scale = scale; m_hasChanged = true; }
      void SetRotation(const Vec3<float>& rotation) { m_rotation = rotation; m_hasChanged = true; }
      void Move(float dx, float dy, float dz) { m_position.x += dx;m_position.y = dy; m_position.z = dz; m_hasChanged = true; }
      void Scale(float x, float y, float z) { m_scale.x += x;m_scale.y += y; m_scale.z += z; m_hasChanged = true; }
      void Rotate(float x, float y, float z) { m_rotation.x += x;m_rotation.y += y; m_rotation.z += z; m_hasChanged = true; }

      const Vec3<float>& GetPosition() const { return m_position;}
      const Vec3<float>& GetScale() const { return m_scale;}
      const Vec3<float>& GetRotation() const { return m_rotation;}
  };
}
