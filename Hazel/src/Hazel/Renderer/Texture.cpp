#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Hazel {
	std::shared_ptr<Texture2D> Texture2D::create(const char* path)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(path);
		}

		return nullptr;
	}

	std::shared_ptr<Texture2D> Texture2D::create(uint32_t width, uint32_t height)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(width, height);
		}

		return nullptr;
	}
}