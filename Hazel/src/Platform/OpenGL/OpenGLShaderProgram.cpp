#include "OpenGLShaderProgram.h"

#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Hazel/Utils/CheckOpenGL.h"
#include "Hazel/Core/Log.h"

namespace Hazel {

	OpenGLShaderProgram::OpenGLShaderProgram()
	{
		CHECK_GL(programId = glCreateProgram());
	}

	OpenGLShaderProgram::~OpenGLShaderProgram()
	{
		CHECK_GL(glDeleteProgram(programId));
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
		CHECK_GL(glAttachShader(programId, shader->shaderId));
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
		CHECK_GL(glLinkProgram(programId));
		int status = GL_TRUE;
		CHECK_GL(glGetProgramiv(programId, GL_LINK_STATUS, &status));
		if (status != GL_TRUE) {
			GLsizei logLen;
			CHECK_GL(glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLen));
			std::vector<GLchar> log(logLen + 1);
			CHECK_GL(glGetProgramInfoLog(programId, logLen, &logLen, log.data()));
			log[logLen] = 0;
			std::string err = (std::string)"Error linking program:\n" + log.data();
			HZ_CORE_ERROR(err);
		}
	}

	void OpenGLShaderProgram::bind()
	{
		CHECK_GL(glUseProgram(programId));
	}

	void OpenGLShaderProgram::unbind()
	{
		CHECK_GL(glUseProgram(0));
	}
	void OpenGLShaderProgram::setUniform(const char* name, int val) const {
		GLuint loc;
		CHECK_GL(loc = glGetUniformLocation(programId, name));
		CHECK_GL(glUniform1i(loc, val));
	}
	void OpenGLShaderProgram::setUniform(const char* name, float val) const {
		GLuint loc;
		CHECK_GL(loc = glGetUniformLocation(programId, name));
		CHECK_GL(glUniform1f(loc, val));
	}
	void OpenGLShaderProgram::setUniform(const char* name, glm::vec2 const& val) const {
		GLuint loc;
		CHECK_GL(loc = glGetUniformLocation(programId, name));
		CHECK_GL(glUniform2fv(loc, 1, glm::value_ptr(val)));
	}
	void OpenGLShaderProgram::setUniform(const char* name, glm::vec3 const& val) const {
		GLuint loc;
		CHECK_GL(loc = glGetUniformLocation(programId, name));
		CHECK_GL(glUniform3fv(loc, 1, glm::value_ptr(val)));
	}
	void OpenGLShaderProgram::setUniform(const char* name, glm::vec4 const& val) const {
		GLuint loc;
		CHECK_GL(loc = glGetUniformLocation(programId, name));
		CHECK_GL(glUniform4fv(loc, 1, glm::value_ptr(val)));
	}
	void OpenGLShaderProgram::setUniform(const char* name, glm::mat3x3 const& val) const {
		GLuint loc;
		CHECK_GL(loc = glGetUniformLocation(programId, name));
		CHECK_GL(glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(val)));
	}
	void OpenGLShaderProgram::setUniform(const char* name, glm::mat4x4 const& val) const {
		GLuint loc;
		CHECK_GL(loc = glGetUniformLocation(programId, name));
		CHECK_GL(glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val)));
	}
	void OpenGLShaderProgram::setUniform(const char* name, glm::mat4x3 const& val) const {
		GLuint loc;
		CHECK_GL(loc = glGetUniformLocation(programId, name));
		CHECK_GL(glUniformMatrix4x3fv(loc, 1, GL_FALSE, glm::value_ptr(val)));
	}
	void OpenGLShaderProgram::setUniform(const char* name, glm::mat3x4 const& val) const {
		GLuint loc;
		CHECK_GL(loc = glGetUniformLocation(programId, name));
		CHECK_GL(glUniformMatrix3x4fv(loc, 1, GL_FALSE, glm::value_ptr(val)));
	}

	void OpenGLShaderProgram::setUniform(const char* name, const std::vector<int>& val) const
	{
		GLuint loc;
		CHECK_GL(loc = glGetUniformLocation(programId, name));
		CHECK_GL(glUniform1iv(loc, val.size(), val.data()));
	}
}