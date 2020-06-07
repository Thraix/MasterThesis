#include "FrameBufferObject.h"

#include <graphics/Window.h>
#include <internal/GreetGL.h>
#include <math/Maths.h>

namespace Greet {

  FrameBufferObject::FrameBufferObject(uint width, uint height)
    : m_width(width), m_height(height)
  {
    GLCall(glGenFramebuffers(1, &m_fbo));

    // Color buffer
    //attachDepthTexture(); AttachColorTexture(GL_COLOR_ATTACHMENT0);

    //return;
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER,m_fbo));
    GLCall(glGenRenderbuffers(1,&m_depthBuffer));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER,m_depthBuffer));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,m_width,m_height));

    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,m_depthBuffer));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
  }

  FrameBufferObject::~FrameBufferObject()
  {
    GLCall(glDeleteFramebuffers(1, &m_fbo));
    GLCall(glDeleteBuffers(1,&m_depthBuffer));
    m_colorTextures.clear();
  }

  void FrameBufferObject::AttachColorTexture(uint attachmentId)
  {
    if (attachmentId < GL_COLOR_ATTACHMENT0 || attachmentId > GL_COLOR_ATTACHMENT15)
    {
      Log::Error("Color attachment is out of range: ",attachmentId);
      return;
    }
    if (m_colorTextures.find(attachmentId) != m_colorTextures.end())
    {
      Log::Error("The given attachment is already in use: ", attachmentId);
    }
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
    Texture2D colorTexture(m_width,m_height,TextureParams(TextureFilter::LINEAR,TextureWrap::NONE,TextureInternalFormat::RGB));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentId, GL_TEXTURE_2D, colorTexture.GetTexId(), 0));
    m_colorTextures.emplace(attachmentId,colorTexture);
    uint size = m_colorTextures.size();
    uint* colorBuffers = new uint[size];
    uint i = 0;
    for (auto it = m_colorTextures.begin();it != m_colorTextures.end();it++)
      colorBuffers[i++] = it->first;
    GLCall(glDrawBuffers(size,colorBuffers));
    delete[] colorBuffers;
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER,0));
  }

  void FrameBufferObject::AttachDepthTexture()
  {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
    m_depthTexture = Texture2D(m_width,m_height,TextureParams(TextureFilter::NEAREST,TextureWrap::NONE,TextureInternalFormat::DEPTH_COMPONENT));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture.GetTexId(), 0));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER,0));
  }

  void FrameBufferObject::Bind()
  {
    GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER,m_fbo));
    GLCall(glClearColor(0,0,0,0));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GLCall(glViewport(0,0,m_width,m_height));
  }

  void FrameBufferObject::Unbind()
  {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GLCall(glViewport(0,0,Window::GetWidth(),Window::GetHeight()));
  }

  const Texture2D& FrameBufferObject::GetColorTexture(uint attachmentId) const
  {
    auto it = m_colorTextures.find(attachmentId);
    if (it != m_colorTextures.end())
      return it->second;
    Log::Error("There is no Texture with the given attachmentId: ",attachmentId);
    return m_colorTextures.begin()->second;
  }
}
