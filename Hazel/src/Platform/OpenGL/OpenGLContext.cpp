#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Hazel/Core/Log.h"

namespace Hazel {

	OpenGLContext::OpenGLContext(GLFWwindow* window)
		: m_windowHandle(window)
	{
		if (window == nullptr)
			HZ_CORE_ERROR("Window handle is null!");
	}

	void OpenGLContext::init()
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

		int versionMajor, versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
		if (versionMajor < 4 || (versionMajor == 4 && versionMinor < 5)) {
			HZ_CORE_ERROR("Hazel requires at least OpenGL version 4.5!");
		}
	}

	void OpenGLContext::swapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}

}