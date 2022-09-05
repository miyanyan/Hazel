#pragma once

#include "Hazel/Renderer/VertexArray.h"

namespace Hazel {

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray() override;

		virtual void bind() const;
		virtual void unbind() const;

		virtual void addVertexBuffer(std::shared_ptr<VertexBuffer>& vbo);
		virtual void setIndexBuffer(std::shared_ptr<IndexBuffer>& ibo);

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const;
		virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() const;

	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
		unsigned int m_vertexArrayId;
		size_t m_vertexBufferIndex;
	};

}