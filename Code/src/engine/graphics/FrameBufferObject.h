#pragma once

#include <common/Types.h>
#include <graphics/textures/Texture2D.h>
#include <map>

namespace Greet {



  class FrameBufferObject
  {
    private:
      uint m_fbo;
      uint m_width;
      uint m_height;
      std::map<uint,const Texture2D&> m_colorTextures;
      Texture2D m_depthTexture;
      uint m_depthBuffer;
    public:
      FrameBufferObject(uint width, uint height);
      virtual ~FrameBufferObject();
      void Bind();
      void Unbind();
      void AttachColorTexture(uint attachementId);
      void AttachDepthTexture();
      const Texture2D& GetColorTexture(uint attachmentId) const;
      inline const Texture2D& GetDepthTexture() const { return m_depthTexture; }
    private:
  };
}
