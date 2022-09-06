#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Hazel {

	std::unique_ptr<RendererAPI> RenderCommand::s_rendererAPI = std::make_unique<OpenGLRendererAPI>();

}