#include "OpenGLShader.h"

#include <vector>

#include <glad/glad.h>

#include "Hazel/Core/Log.h"

namespace Hazel {

	OpenGLShader::OpenGLShader(ShaderType type)
		: shaderId(0)
	{
		if (type == ShaderType::VERTEX) {
			shaderId = glCreateShader(GL_VERTEX_SHADER);
		}
		else if (type == ShaderType::FRAGMENT) {
			shaderId = glCreateShader(GL_FRAGMENT_SHADER);
		}
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteShader(shaderId);
	}

	void OpenGLShader::compile(const std::string& source)
	{
		// send source
		auto src = source.c_str();
		glShaderSource(shaderId, 1, &src, 0);
		// compile
		glCompileShader(shaderId);
		// check
		GLint isCompiled = GL_TRUE;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled != GL_TRUE) {
			GLint logLen = 0;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLen);
			// the maxLen includes the null char
			std::vector<GLchar> infoLog(logLen + 1);
			glGetShaderInfoLog(shaderId, logLen, &logLen, infoLog.data());
			infoLog[logLen] = 0;
			std::string err = "Error compiling shader :\n" + getShaderWithLineInfo(source) + "\n" + infoLog.data();
			HZ_CORE_ERROR(err);
		}

	}
}