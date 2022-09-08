#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Hazel {

	RendererAPI::API RendererAPI::s_API = API::OpenGL;

	std::unique_ptr<RendererAPI> RendererAPI::create()
	{
		switch (s_API)
		{
		case RendererAPI::API::None:    return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_unique<OpenGLRendererAPI>();
		}

		return nullptr;
	}

}