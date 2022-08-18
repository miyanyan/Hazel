#include "OpenGLContext.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Hazel/Log.h"

namespace Hazel {

	OpenglContext::OpenglContext(GLFWwindow* window)
		: m_windowHandle(window)
	{
		if (window == nullptr)
			HZ_CORE_ERROR("Window handle is null!");
	}

	void OpenglContext::init()
	{
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (status == 0) {
			HZ_CORE_ERROR("Failed to init glad!");
		}

	}

	void OpenglContext::swapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}

}