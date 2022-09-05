#include "OpenGLShaderProgram.h"

#include <fstream>
#include <sstream>

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

#include "Hazel/Core/Log.h"

namespace Hazel {

	OpenGLShaderProgram::OpenGLShaderProgram()
	{
		programId = glCreateProgram();
	}

	OpenGLShaderProgram::~OpenGLShaderProgram()
	{
		glDeleteProgram(programId);
	}

	void OpenGLShaderProgram::addShaderFromSourceCode(Shader::ShaderType type, const std::string& source)
	{
		auto shader = std::make_unique<OpenGLShader>(type);
		shader->compile(source);
		attach(shader.get());
		m_shaders[shader->shaderId] = std::move(shader);
	}

	void OpenGLShaderProgram::addShaderFromSourceFile(Shader::ShaderType type, const std::string& file)
	{
		auto source = readFile(file);
		addShaderFromSourceCode(type, source);
	}

	void OpenGLShaderProgram::attach(const OpenGLShader* shader)
	{
		glAttachShader(programId, shader->shaderId);
	}

	std::string OpenGLShaderProgram::readFile(const std::string& file)
	{
		std::ifstream in(file);
		std::stringstream buffer;
		buffer << in.rdbuf();
		return buffer.str();
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