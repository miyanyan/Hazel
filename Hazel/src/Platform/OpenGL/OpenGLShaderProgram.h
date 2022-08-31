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

		virtual void setUniform(const char* name, int val) const override;
		virtual void setUniform(const char* name, float val) const override;
		virtual void setUniform(const char* name, glm::vec2 const& val) const override;
		virtual void setUniform(const char* name, glm::vec3 const& val) const override;
		virtual void setUniform(const char* name, glm::vec4 const& val) const override;
		virtual void setUniform(const char* name, glm::mat3x3 const& val) const override;
		virtual void setUniform(const char* name, glm::mat4x4 const& val) const override;
		virtual void setUniform(const char* name, glm::mat4x3 const& val) const override;
		virtual void setUniform(const char* name, glm::mat3x4 const& val) const override;

	public:
		unsigned int programId;

	private:
		void attach(const OpenGLShader* shader);
		void link();

		std::unique_ptr<OpenGLShader> m_vertex, m_fragment;
	};

}