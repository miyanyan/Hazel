#pragma once

#include <string>

namespace Hazel {

	class ShaderProgram
	{
	public:
		virtual ~ShaderProgram(){};
		virtual void bind() = 0;
		virtual void unbind() = 0;
	};

}