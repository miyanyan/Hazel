#pragma once

#include "Hazel/Renderer/Buffer.h"

namespace Hazel {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer();
		virtual ~OpenGLVertexBuffer() override;

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void allocate(const void* data, size_t dataSize, GLuint usage) override;

		virtual GLuint getBufferId() const override;

	private:
		unsigned int m_bufferId;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer();
		virtual ~OpenGLIndexBuffer() override;

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void allocate(const void* data, size_t dataSize, size_t indexCount, GLuint usage) override;

		virtual GLuint getBufferId() const override;

		virtual size_t getCount() const override;

	private:
		unsigned int m_bufferId;
		size_t m_indexCount;
	};
}