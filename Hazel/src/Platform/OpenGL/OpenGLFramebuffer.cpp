#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

#include "Hazel/Utils/CheckOpenGL.h"
#include "Hazel/Core/Log.h"

namespace Hazel {

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: m_specification(spec)
		, m_framebufferId(0)
		, m_colorAttachment(0)
		, m_depthAttachment(0)
	{
		invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		CHECK_GL(glDeleteFramebuffers(1, &m_framebufferId));
		CHECK_GL(glDeleteTextures(1, &m_colorAttachment));
		CHECK_GL(glDeleteTextures(1, &m_depthAttachment));
	}

	void OpenGLFramebuffer::invalidate()
	{
		if (m_framebufferId) {
			CHECK_GL(glDeleteFramebuffers(1, &m_framebufferId));
			CHECK_GL(glDeleteTextures(1, &m_colorAttachment));
			CHECK_GL(glDeleteTextures(1, &m_depthAttachment));
		}

		CHECK_GL(glCreateFramebuffers(1, &m_framebufferId));

		// color attachment
		CHECK_GL(glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachment));
		CHECK_GL(glBindTexture(GL_TEXTURE_2D, m_colorAttachment));
		CHECK_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_specification.width, m_specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
		CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		CHECK_GL(glNamedFramebufferTexture(m_framebufferId, GL_COLOR_ATTACHMENT0, m_colorAttachment, 0));

		// depth attachment
		CHECK_GL(glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachment));
		CHECK_GL(glTextureStorage2D(m_depthAttachment, 1, GL_DEPTH24_STENCIL8, m_specification.width, m_specification.height));
		CHECK_GL(glNamedFramebufferTexture(m_framebufferId, GL_DEPTH_STENCIL_ATTACHMENT, m_depthAttachment, 0));

		if (glCheckNamedFramebufferStatus(m_framebufferId, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			HZ_CORE_ERROR("Framebuffer is incomplete!");
		}
	}

	void OpenGLFramebuffer::bind()
	{
		CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferId)); 
		glViewport(0, 0, m_specification.width, m_specification.height);
	}

	void OpenGLFramebuffer::unbind()
	{
		CHECK_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void OpenGLFramebuffer::resize(uint32_t width, uint32_t height)
	{
		m_specification.width = width;
		m_specification.height = height;

		invalidate();
	}

	uint32_t OpenGLFramebuffer::getColorAttachmentRendererId() const
	{
		return m_colorAttachment;
	}

	const FramebufferSpecification& OpenGLFramebuffer::getSpecification() const
	{
		return m_specification;
	}

}