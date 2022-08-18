#pragma once

#include "Hazel/Renderer/GraphicsContext.h"

class GLFWwindow;

namespace Hazel {

	class OpenglContext : public GraphicsContext
	{
	public:
		OpenglContext(GLFWwindow* window);

		virtual void init() override;
		virtual void swapBuffers() override;

	private:
		GLFWwindow* m_windowHandle;
	};

}