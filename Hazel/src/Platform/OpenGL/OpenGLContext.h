#pragma once

#include "Hazel/Renderer/GraphicsContext.h"

class GLFWwindow;

namespace Hazel {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* window);

		virtual void init() override;
		virtual void swapBuffers() override;

	private:
		GLFWwindow* m_windowHandle;
	};

}