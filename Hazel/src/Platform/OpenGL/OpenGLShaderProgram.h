#pragma once 

#include <memory>

#include "Hazel/Renderer/ShaderProgram.h"
#include "OpenGLShader.h"

namespace Hazel {

	class OpenGLShaderProgram : public ShaderProgram
	{
	public:
		OpenGLShaderProgram(const std::string& vertexSource, const std::string& fragmentSource);
		~OpenGLShaderProgram();

		virtual void bind();
		virtual void unbind();

	public:
		unsigned int programId;

	private:
		void attach(const OpenGLShader* shader);
		void link();

		std::unique_ptr<OpenGLShader> m_vertex, m_fragment;
	};

}