#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Hazel {
	Texture2D* Texture2D::create(const char* path)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None: return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLTexture2D(path);
		}

		return nullptr;
	}
}