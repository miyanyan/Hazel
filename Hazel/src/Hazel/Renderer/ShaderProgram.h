#pragma once

#include <string>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "Shader.h"

namespace Hazel {

	class ShaderProgram
	{
	public:
		virtual ~ShaderProgram() = default;

        virtual void addShaderFromSourceCode(Shader::ShaderType type, const std::string& source) = 0;
        virtual void addShaderFromSourceFile(Shader::ShaderType type, const std::string& file) = 0;

        virtual void link() = 0;

		virtual void bind() = 0;
		virtual void unbind() = 0;

        virtual void setUniform(const char* name, int val) const = 0;
        virtual void setUniform(const char* name, float val) const = 0;
        virtual void setUniform(const char* name, const glm::vec2& val) const = 0;
        virtual void setUniform(const char* name, const glm::vec3& val) const = 0;
        virtual void setUniform(const char* name, const glm::vec4& val) const = 0;
        virtual void setUniform(const char* name, const glm::mat3x3& val) const = 0;
        virtual void setUniform(const char* name, const glm::mat4x4& val) const = 0;
        virtual void setUniform(const char* name, const glm::mat3x4& val) const = 0;
        virtual void setUniform(const char* name, const glm::mat4x3& val) const = 0;
        virtual void setUniform(const char* name, const std::vector<int>& val) const = 0;

        static std::shared_ptr<ShaderProgram> create();

	};

}