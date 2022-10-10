#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

#include "Hazel/Utils/CheckOpenGL.h"
#include "Hazel/Core/Log.h"

namespace Hazel {

	static GLenum getGLFormat(Framebuffer::ColorTextureFormat format)
	{
		switch (format)
		{
		case Hazel::Framebuffer::ColorTextureFormat::RGBA8: return GL_RGBA8;
		case Hazel::Framebuffer::ColorTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
		}
		return 0;
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const Specification& spec)
		: m_specification(spec)
		, m_framebufferId(0)
	{
		invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		deleteFBO();
	}

	void OpenGLFramebuffer::invalidate()
	{
		if (m_framebufferId) {
			deleteFBO();
			m_colorAttachments.clear();
			m_depthAttachment = 0;
		}

		CHECK_GL(glCreateFramebuffers(1, &m_framebufferId));


		bool multisampled = m_specification.samples > 1;
		// color attachments
		auto& colorFormat = m_specification.colorAttachmentFormat;
		if (!colorFormat.empty()) {
			auto size = colorFormat.size();
			m_colorAttachments.resize(size);
			createTextures(multisampled, m_colorAttachments.data(), size);

			for (size_t i = 0; i < size; ++i) {
				if (colorFormat[i] == ColorTextureFormat::None) continue;

				uint32_t glformat = 0;

				switch (colorFormat[i])
				{
				case ColorTextureFormat::RGBA8:
					glformat = GL_RGBA8;
					break;
				case ColorTextureFormat::RED_INTEGER:
					glformat = GL_R32I;
					break;
				}

				attachTexture(m_colorAttachments[i], m_specification.samples, glformat, GL_COLOR_ATTACHMENT0 + i, m_specification.width, m_specification.height);
			}
		}

		// depth attachment
		auto& depthFormat = m_specification.depthAttachmentFormat;
		if (depthFormat != DepthTextureFormat::None) {
			createTextures(multisampled, &m_depthAttachment, 1);

			uint32_t glformat = 0;

			switch (depthFormat)
			{
			case DepthTextureFormat::DEPTH24STENCIL8:
				glformat = GL_DEPTH24_STENCIL8;
				break;
			}
			
			attachTexture(m_depthAttachment, m_specification.samples, glformat, GL_DEPTH_STENCIL_ATTACHMENT, m_specification.width, m_specification.height);
		}
		
		if (m_colorAttachments.size() > 1) {
			std::vector<GLenum> buffers(m_colorAttachments.size());
			for (size_t i = 0; i < m_colorAttachments.size(); ++i) {
				buffers[i] = GL_COLOR_ATTACHMENT0 + i;
			}
			CHECK_GL(glNamedFramebufferDrawBuffers(m_framebufferId, m_colorAttachments.size(), buffers.data()));
		}
		else if (m_colorAttachments.empty()) {
			// only depth pass
			CHECK_GL(glNamedFramebufferDrawBuffer(m_framebufferId, GL_NONE));
		}

		if (glCheckNamedFramebufferStatus(m_framebufferId, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			HZ_CORE_ERROR("Framebuffer is incomplete!");
		}
	}

	void OpenGLFramebuffer::deleteFBO()
	{
		CHECK_GL(glDeleteFramebuffers(1, &m_framebufferId));
		CHECK_GL(glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data()));
		CHECK_GL(glDeleteTextures(1, &m_depthAttachment));
	}

	void OpenGLFramebuffer::createTextures(bool multisampled, uint32_t* ids, uint32_t count)
	{
		GLenum target = multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		CHECK_GL(glCreateTextures(target, count, ids));
	}

	void OpenGLFramebuffer::attachTexture(int id, int samples, uint32_t format, uint32_t attachmentType, uint32_t width, uint32_t height)
	{
		// muti sample
		if (samples > 1) {
			CHECK_GL(glTextureStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE));
		}
		// one sample
		else {
			CHECK_GL(glTextureStorage2D(id, 1, format, width, height));
			CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
			CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			CHECK_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		}

		CHECK_GL(glNamedFramebufferTexture(m_framebufferId, attachmentType, id, 0));
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

	int OpenGLFramebuffer::readPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y)
	{
		glNamedFramebufferReadBuffer(m_framebufferId, GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void OpenGLFramebuffer::clearAttachment(uint32_t attachmentIndex, int value)
	{
		auto& format = m_specification.colorAttachmentFormat[attachmentIndex];

		glClearTexImage(m_colorAttachments[attachmentIndex], 0, getGLFormat(format), GL_INT, &value);
	}

	uint32_t OpenGLFramebuffer::getColorAttachmentRendererId(uint32_t index) const
	{
		return m_colorAttachments[index];
	}

	const OpenGLFramebuffer::Specification& OpenGLFramebuffer::getSpecification() const
	{
		return m_specification;
	}

}