#include "OpenGLShaderProgram.h"

#include <vector>

#include "glad/glad.h"

#include "OpenGLShader.h"
#include "Hazel/Log.h"

namespace Hazel {

	OpenGLShaderProgram::OpenGLShaderProgram(const std::string& vertexSource, const std::string& fragmentSource)
	{
		m_vertex = std::make_unique<OpenGLShader>(Shader::VERTEX);
		m_vertex->compile(vertexSource);
		m_fragment = std::make_unique<OpenGLShader>(Shader::FRAGMENT);
		m_fragment->compile(fragmentSource);

		programId = glCreateProgram();
		attach(m_vertex.get());
		attach(m_fragment.get());
		link();
	}

	OpenGLShaderProgram::~OpenGLShaderProgram()
	{
		glDeleteProgram(programId);
	}

	void OpenGLShaderProgram::attach(const OpenGLShader* shader)
	{
		glAttachShader(programId, shader->shaderId);
	}

	void OpenGLShaderProgram::link()
	{
		glLinkProgram(programId);
		int status = GL_TRUE;
		glGetProgramiv(programId, GL_LINK_STATUS, &status);
		if (status != GL_TRUE) {
			GLsizei logLen;
			glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLen);
			std::vector<GLchar> log(logLen + 1);
			glGetProgramInfoLog(programId, logLen, &logLen, log.data());
			log[logLen] = 0;
			std::string err = (std::string)"Error linking program:\n" + log.data();
			HZ_CORE_ERROR(err);
		}
	}

	void OpenGLShaderProgram::bind()
	{
		glUseProgram(programId);
	}

	void OpenGLShaderProgram::unbind()
	{
		glUseProgram(0);
	}
}