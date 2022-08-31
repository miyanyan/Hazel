#pragma once

#include <string>

#include "glm/glm.hpp"

namespace Hazel {

	class ShaderProgram
	{
	public:
		virtual ~ShaderProgram() = default;

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

	};

}