#include "ShaderProgram.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShaderProgram.h"

namespace Hazel {

	ShaderProgram* ShaderProgram::create()
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:    return nullptr;
		case RendererAPI::API::OpenGL:  return new OpenGLShaderProgram();
		}

		return nullptr;
	}

}