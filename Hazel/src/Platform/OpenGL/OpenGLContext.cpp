#include "OpenGLContext.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Hazel/Core/Log.h"

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
		HZ_CORE_INFO("OpenGL Info:");
		HZ_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		HZ_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		HZ_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));

	}

	void OpenglContext::swapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}

}