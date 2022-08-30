#pragma once

#include "Hazel/Renderer/Buffer.h"

namespace Hazel {

	class OpenGLVertexBuffer : public Buffer
	{
	public:
		OpenGLVertexBuffer();
		virtual ~OpenGLVertexBuffer() override;

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void allocate(const void* data, size_t size, GLuint usage) override;

		virtual GLuint getBufferId() const override;

	private:
		unsigned int m_bufferId;
	};

	class OpenGLIndexBuffer : public Buffer
	{
	public:
		OpenGLIndexBuffer();
		virtual ~OpenGLIndexBuffer() override;

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void allocate(const void* data, size_t size, GLuint usage) override;

		virtual GLuint getBufferId() const override;

	private:
		unsigned int m_bufferId;
	};
}