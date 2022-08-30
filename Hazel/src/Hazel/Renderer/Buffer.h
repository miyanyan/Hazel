#pragma once

#include "glad/glad.h"

namespace Hazel {

	class Buffer
	{
	public:
		virtual ~Buffer() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void allocate(const void* data, size_t size, GLuint usage = GL_STATIC_DRAW) = 0;

		virtual GLuint getBufferId() const = 0;
	};

}