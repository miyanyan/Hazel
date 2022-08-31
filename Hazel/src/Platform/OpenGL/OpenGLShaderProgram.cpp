#include "OpenGLShaderProgram.h"

#include <vector>

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

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
	void OpenGLShaderProgram::setUniform(const char* name, int val) const {
		GLuint loc;
		loc = glGetUniformLocation(programId, name);
		glUniform1i(loc, val);
	}
	void OpenGLShaderProgram::setUniform(const char* name, float val) const {
		GLuint loc;
		loc = glGetUniformLocation(programId, name);
		glUniform1f(loc, val);
	}
	void OpenGLShaderProgram::setUniform(const char* name, glm::vec2 const& val) const {
		GLuint loc;
		loc = glGetUniformLocation(programId, name);
		glUniform2fv(loc, 1, glm::value_ptr(val));
	}
	void OpenGLShaderProgram::setUniform(const char* name, glm::vec3 const& val) const {
		GLuint loc;
		loc = glGetUniformLocation(programId, name);
		glUniform3fv(loc, 1, glm::value_ptr(val));
	}
	void OpenGLShaderProgram::setUniform(const char* name, glm::vec4 const& val) const {
		GLuint loc;
		loc = glGetUniformLocation(programId, name);
		glUniform4fv(loc, 1, glm::value_ptr(val));
	}
	void OpenGLShaderProgram::setUniform(const char* name, glm::mat3x3 const& val) const {
		GLuint loc;
		loc = glGetUniformLocation(programId, name);
		glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(val));
	}
	void OpenGLShaderProgram::setUniform(const char* name, glm::mat4x4 const& val) const {
		GLuint loc;
		loc = glGetUniformLocation(programId, name);
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
	}
	void OpenGLShaderProgram::setUniform(const char* name, glm::mat4x3 const& val) const {
		GLuint loc;
		loc = glGetUniformLocation(programId, name);
		glUniformMatrix4x3fv(loc, 1, GL_FALSE, glm::value_ptr(val));
	}
	void OpenGLShaderProgram::setUniform(const char* name, glm::mat3x4 const& val) const {
		GLuint loc;
		loc = glGetUniformLocation(programId, name);
		glUniformMatrix3x4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
	}
}