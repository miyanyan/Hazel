#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

#include "Hazel/Utils/CheckOpenGL.h"

namespace Hazel {

	void OpenGLRendererAPI::init()
	{
		CHECK_GL(glEnable(GL_BLEND));
		CHECK_GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		CHECK_GL(glEnable(GL_DEPTH_TEST));
	}

	void OpenGLRendererAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		CHECK_GL(glViewport(x, y, width, height));
	}

	void OpenGLRendererAPI::setClearColor(const glm::vec4& color)
	{
		CHECK_GL(glClearColor(color.r, color.g, color.b, color.a));
	}

	void OpenGLRendererAPI::clear()
	{
		CHECK_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void OpenGLRendererAPI::drawIndexed(const std::shared_ptr<VertexArray>& vao, uint32_t count)
	{
		if (count == 0) {
			count = vao->getIndexBuffer()->getCount();
		}
		CHECK_GL(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
	}

}