#include "OpenGLBuffer.h"

#include "Hazel/Utils/CheckOpenGL.h"

namespace Hazel {

//---------------------------------------- vertex buffer -------------------------------------------//

	OpenGLVertexBuffer::OpenGLVertexBuffer()
	{
		CHECK_GL(glCreateBuffers(1, &m_bufferId));
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		CHECK_GL(glDeleteBuffers(1, &m_bufferId));
	}

	void OpenGLVertexBuffer::bind() const
	{
		CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, m_bufferId));
	}

	void OpenGLVertexBuffer::unbind() const
	{
		CHECK_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void OpenGLVertexBuffer::allocate(const void* data, size_t dataSize, GLuint usage)
	{
		CHECK_GL(glNamedBufferData(m_bufferId, dataSize, data, usage));
	}

	void OpenGLVertexBuffer::write(const void* data, size_t dataSize, size_t offset)
	{
		CHECK_GL(glNamedBufferSubData(m_bufferId, offset, dataSize, data));
	}

	GLuint OpenGLVertexBuffer::getBufferId() const
	{
		return m_bufferId;
	}

	void OpenGLVertexBuffer::setLayout(const BufferLayout& layout)
	{
		m_layout = layout;
	}

	const BufferLayout& OpenGLVertexBuffer::getLayout()
	{
		return m_layout;
	}

	//----------------------------------------- index buffer --------------------------------------------//

	OpenGLIndexBuffer::OpenGLIndexBuffer()
		: m_indexCount(0)
	{
		CHECK_GL(glCreateBuffers(1, &m_bufferId));
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		CHECK_GL(glDeleteBuffers(1, &m_bufferId));
	}

	void OpenGLIndexBuffer::bind() const
	{
		CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId));
	}

	void OpenGLIndexBuffer::unbind() const
	{
		CHECK_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	void OpenGLIndexBuffer::allocate(const void* data, size_t dataSize, size_t indexCount, GLuint usage)
	{
		m_indexCount = indexCount;
		CHECK_GL(glNamedBufferData(m_bufferId, dataSize, data, usage));
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
