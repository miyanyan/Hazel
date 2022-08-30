#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace Hazel {

	static GLenum shaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Hazel::ShaderDataType::Float:    return GL_FLOAT;
		case Hazel::ShaderDataType::Float2:   return GL_FLOAT;
		case Hazel::ShaderDataType::Float3:   return GL_FLOAT;
		case Hazel::ShaderDataType::Float4:   return GL_FLOAT;
		case Hazel::ShaderDataType::Mat3:     return GL_FLOAT;
		case Hazel::ShaderDataType::Mat4:     return GL_FLOAT;
		case Hazel::ShaderDataType::Int:      return GL_INT;
		case Hazel::ShaderDataType::Int2:     return GL_INT;
		case Hazel::ShaderDataType::Int3:     return GL_INT;
		case Hazel::ShaderDataType::Int4:     return GL_INT;
		case Hazel::ShaderDataType::Bool:     return GL_BOOL;
		}

		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_vertexArrayId);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_vertexArrayId);
	}

	void OpenGLVertexArray::bind() const
	{
		glBindVertexArray(m_vertexArrayId);
	}

	void OpenGLVertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::addVertexBuffer(std::shared_ptr<VertexBuffer>& vbo)
	{
		// assume bindingindex = 0
		GLuint bindingindex = 0;

		uint32_t index = 0;
		auto& layout = vbo->getLayout();
		for (const auto& element : layout) {
			glEnableVertexArrayAttrib(m_vertexArrayId, index);
			glVertexArrayAttribFormat(m_vertexArrayId, index,
				element.getComponentCount(),
				shaderDataTypeToOpenGLBaseType(element.type),
				element.normalized ? GL_TRUE : GL_FALSE,
				element.offset);
			glVertexArrayAttribBinding(m_vertexArrayId, index, bindingindex);
			index++;
		}

		glVertexArrayVertexBuffer(m_vertexArrayId, bindingindex, vbo->getBufferId(), 0, layout.getStride());

		m_vertexBuffers.push_back(vbo);
	}

	void OpenGLVertexArray::setIndexBuffer(std::shared_ptr<IndexBuffer>& ibo)
	{
		glVertexArrayElementBuffer(m_vertexArrayId, ibo->getBufferId());
		m_indexBuffer = ibo;
	}

	const std::vector<std::shared_ptr<VertexBuffer>>& OpenGLVertexArray::getVertexBuffers() const
	{
		return m_vertexBuffers;
	}

	const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::getIndexBuffer() const
	{
		return m_indexBuffer;
	}

}