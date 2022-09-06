#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Hazel {

	std::shared_ptr<VertexBuffer> VertexBuffer::create()
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:    return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexBuffer>();
		}

		return nullptr;
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::create()
	{
		switch (Renderer::getAPI())
		{
		case RendererAPI::API::None:    return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLIndexBuffer>();
		}

		return nullptr;
	}

}