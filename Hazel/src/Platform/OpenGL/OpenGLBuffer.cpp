#include "OpenGLBuffer.h"


namespace Hazel {

//---------------------------------------- vertex buffer -------------------------------------------//

	OpenGLVertexBuffer::OpenGLVertexBuffer()
	{
		glCreateBuffers(1, &m_bufferId);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_bufferId);
	}

	void OpenGLVertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
	}

	void OpenGLVertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::allocate(const void* data, size_t dataSize, GLuint usage)
	{
		glNamedBufferData(m_bufferId, dataSize, data, usage);
	}

	GLuint OpenGLVertexBuffer::getBufferId() const
	{
		return m_bufferId;
	}

	//----------------------------------------- index buffer --------------------------------------------//

	OpenGLIndexBuffer::OpenGLIndexBuffer()
		: m_indexCount(0)
	{
		glCreateBuffers(1, &m_bufferId);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_bufferId);
	}

	void OpenGLIndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
	}

	void OpenGLIndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void OpenGLIndexBuffer::allocate(const void* data, size_t dataSize, size_t indexCount, GLuint usage)
	{
		m_indexCount = indexCount;
		glNamedBufferData(m_bufferId, dataSize, data, usage);
	}

	GLuint OpenGLIndexBuffer::getBufferId() const
	{
		return m_bufferId;;
	}

	size_t OpenGLIndexBuffer::getCount() const
	{
		return m_indexCount;
	}

}
