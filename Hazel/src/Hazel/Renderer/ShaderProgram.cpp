#include "ShaderProgram.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShaderProgram.h"

namespace Hazel {

	std::shared_ptr<ShaderProgram> ShaderProgram::create()
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:    return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLShaderProgram>();
		}

		return nullptr;
	}

}