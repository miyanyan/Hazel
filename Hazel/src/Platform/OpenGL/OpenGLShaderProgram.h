#pragma once 

#include <memory>
#include <unordered_map>

#include "Hazel/Renderer/ShaderProgram.h"
#include "OpenGLShader.h"

namespace Hazel {

	class OpenGLShaderProgram : public ShaderProgram
	{
	public:
        OpenGLShaderProgram();
		~OpenGLShaderProgram();

		virtual void addShaderFromSourceCode(Shader::ShaderType type, const std::string& source) override;
		virtual void addShaderFromSourceFile(Shader::ShaderType type, const std::string& file) override;

		virtual void link() override;

		virtual void bind() override;
		virtual void unbind() override;

		virtual void setUniform(const char* name, int val) const override;
		virtual void setUniform(const char* name, float val) const override;
		virtual void setUniform(const char* name, glm::vec2 const& val) const override;
		virtual void setUniform(const char* name, glm::vec3 const& val) const override;
		virtual void setUniform(const char* name, glm::vec4 const& val) const override;
		virtual void setUniform(const char* name, glm::mat3x3 const& val) const override;
		virtual void setUniform(const char* name, glm::mat4x4 const& val) const override;
		virtual void setUniform(const char* name, glm::mat4x3 const& val) const override;
		virtual void setUniform(const char* name, glm::mat3x4 const& val) const override;
		virtual void setUniform(const char* name, const std::vector<int>& val) const override;

	public:
		unsigned int programId;

	private:
		void attach(const OpenGLShader* shader);
		std::string readFile(const std::string& file);

		std::unordered_map<unsigned int, std::unique_ptr<OpenGLShader>> m_shaders;
	};

}