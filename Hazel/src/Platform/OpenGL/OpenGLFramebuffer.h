#pragma once

#include "Hazel/Renderer/Framebuffer.h"

namespace Hazel {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const Specification& spec);
		virtual ~OpenGLFramebuffer();

		virtual void bind() override;
		virtual void unbind() override;

		virtual void resize(uint32_t width, uint32_t height) override;

		virtual uint32_t getColorAttachmentRendererId(uint32_t index = 0) const override;
		virtual const Specification& getSpecification() const override;
	private:
		void invalidate();
		void deleteFBO();

		void createTextures(bool multisampled, uint32_t* ids, uint32_t count);
		void attachTexture(int id, int samples, uint32_t format, uint32_t attachmentType, uint32_t width, uint32_t height);

		uint32_t m_framebufferId;

		Specification m_specification;

		std::vector<uint32_t> m_colorAttachments;
		uint32_t m_depthAttachment = 0;
		
	};

}

