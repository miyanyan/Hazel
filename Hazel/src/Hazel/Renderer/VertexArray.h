#pragma once

#include <memory>

#include "Buffer.h"

namespace Hazel {

	class VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void addVertexBuffer(std::shared_ptr<VertexBuffer>& vbo) = 0;
		virtual void setIndexBuffer(std::shared_ptr<IndexBuffer>& ibo) = 0;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() const = 0;

		static VertexArray* create();
	};

}