#include "ShaderProgram.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShaderProgram.h"

namespace Hazel {

	ShaderProgram* ShaderProgram::create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:    return nullptr;
		case RendererAPI::API::OpenGL:  return new OpenGLShaderProgram(vertexSource, fragmentSource);
		}

		return nullptr;
	}

}