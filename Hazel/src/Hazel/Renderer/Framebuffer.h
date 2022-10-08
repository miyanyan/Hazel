#pragma once

#include <vector>
#include <memory>

namespace Hazel {

	class Framebuffer
	{
	public:

		enum class ColorTextureFormat
		{
			None,
			RGBA8
		};

		enum class DepthTextureFormat
		{
			None,
			DEPTH24STENCIL8
		};

		struct Specification
		{
			uint32_t width, height;
			uint32_t samples;
			// default depth attachment
			DepthTextureFormat depthAttachmentFormat = DepthTextureFormat::DEPTH24STENCIL8;
			// default color attachment
			std::vector<ColorTextureFormat> colorAttachmentFormat = { ColorTextureFormat::RGBA8 };
			bool swapChainTarget;

			Specification(uint32_t width, uint32_t height, uint32_t samples = 1, bool swapChainTarget = false)
				: width(width)
				, height(height)
				, samples(samples)
				, swapChainTarget(swapChainTarget)
			{
			}
		};

		virtual ~Framebuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t getColorAttachmentRendererId(uint32_t index = 0) const = 0;
		virtual const Specification& getSpecification() const = 0;

		static std::shared_ptr<Framebuffer> create(const Specification& spec);
	};
}