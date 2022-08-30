#pragma once

#include "glad/glad.h"

namespace Hazel {

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void allocate(const void* data, size_t dataSize, GLuint usage = GL_STATIC_DRAW) = 0;

		virtual GLuint getBufferId() const = 0;
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void allocate(const void* data, size_t dataSize, size_t indexCount, GLuint usage = GL_STATIC_DRAW) = 0;

		virtual GLuint getBufferId() const = 0;

		virtual size_t getCount() const = 0;
	};

}