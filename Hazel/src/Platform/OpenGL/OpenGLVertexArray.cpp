#include "OpenGLVertexArray.h"

#include <glad/glad.h>

#include "Hazel/Utils/CheckOpenGL.h"
#include "Hazel/Core/Log.h"

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
		: m_vertexBufferIndex(0)
	{
		CHECK_GL(glCreateVertexArrays(1, &m_vertexArrayId));
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		CHECK_GL(glDeleteVertexArrays(1, &m_vertexArrayId));
	}

	void OpenGLVertexArray::bind() const
	{
		CHECK_GL(glBindVertexArray(m_vertexArrayId));
	}

	void OpenGLVertexArray::unbind() const
	{
		CHECK_GL(glBindVertexArray(0));
	}

	void OpenGLVertexArray::addVertexBuffer(std::shared_ptr<VertexBuffer>& vbo)
	{
		// assume bindingindex = 0
		GLuint bindingindex = 0;

		auto& layout = vbo->getLayout();
		for (const auto& element : layout) {
			switch (element.type)
			{
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				{
					CHECK_GL(glEnableVertexArrayAttrib(m_vertexArrayId, m_vertexBufferIndex));
					CHECK_GL(glVertexArrayAttribFormat(m_vertexArrayId, m_vertexBufferIndex,
						element.getComponentCount(),
						shaderDataTypeToOpenGLBaseType(element.type),
						element.normalized ? GL_TRUE : GL_FALSE,
						element.offset));
					CHECK_GL(glVertexArrayAttribBinding(m_vertexArrayId, m_vertexBufferIndex, bindingindex));
					m_vertexBufferIndex++;
					break;
				}
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool:
				{
					CHECK_GL(glEnableVertexArrayAttrib(m_vertexArrayId, m_vertexBufferIndex));
					CHECK_GL(glVertexArrayAttribIFormat(m_vertexArrayId, m_vertexBufferIndex,
						element.getComponentCount(),
						shaderDataTypeToOpenGLBaseType(element.type),
						element.offset));
					CHECK_GL(glVertexArrayAttribBinding(m_vertexArrayId, m_vertexBufferIndex, bindingindex));
					m_vertexBufferIndex++;
					break;
				}
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
				{
					auto count = element.getComponentCount();
					for (auto i = 0; i < count; ++i) {
						CHECK_GL(glEnableVertexArrayAttrib(m_vertexArrayId, m_vertexBufferIndex));
						CHECK_GL(glVertexArrayAttribFormat(m_vertexArrayId, m_vertexBufferIndex,
							count,
							shaderDataTypeToOpenGLBaseType(element.type),
							element.normalized ? GL_TRUE : GL_FALSE,
							element.offset + i * count));
						CHECK_GL(glVertexArrayAttribBinding(m_vertexArrayId, m_vertexBufferIndex, bindingindex));
						m_vertexBufferIndex++;
					}
					break;
				}
				default:
					HZ_CORE_ERROR("Unknown ShaderDataType!");
			}
			
		}

		CHECK_GL(glVertexArrayVertexBuffer(m_vertexArrayId, bindingindex, vbo->getBufferId(), 0, layout.getStride()));

		m_vertexBuffers.push_back(vbo);
	}

	void OpenGLVertexArray::setIndexBuffer(std::shared_ptr<IndexBuffer>& ibo)
	{
		CHECK_GL(glVertexArrayElementBuffer(m_vertexArrayId, ibo->getBufferId()));
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