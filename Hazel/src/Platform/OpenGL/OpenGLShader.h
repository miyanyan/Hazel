#pragma once 

#include "Hazel/Renderer/Shader.h"

namespace Hazel {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(ShaderType type);
		~OpenGLShader();

		void compile(const std::string& source) override;

	public:
		unsigned int shaderId;
	};

}