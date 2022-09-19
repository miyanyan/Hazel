#pragma once

#include <memory>

namespace Hazel {

	struct FramebufferSpecification
	{
		uint32_t width, height;
		uint32_t samples;
		bool swapChainTarget;

		FramebufferSpecification(uint32_t width, uint32_t height, uint32_t samples = 1, bool swapChainTarget = false)
			: width(width)
			, height(height)
			, samples(samples)
			, swapChainTarget(swapChainTarget)
		{
		}
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t getColorAttachmentRendererId() const = 0;
		virtual const FramebufferSpecification& getSpecification() const = 0;

		static std::shared_ptr<Framebuffer> create(const FramebufferSpecification& spec);
	};
}