#include "WindowsWindow.h"

#include "Hazel/Log.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include "GLFW/glfw3.h"

namespace Hazel {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		HZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		shutdown();
	}

	void WindowsWindow::init(const WindowProps& props)
	{
		m_data.title = props.title;
		m_data.width = props.width;
		m_data.height = props.height;

		HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

		if (!s_GLFWInitialized) {
			int success = glfwInit();
			if (success == GLFW_FALSE) {
				HZ_CORE_ERROR("GLFW init failed!");
				return;
			}
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
		
		m_context = std::make_unique<OpenglContext>(m_window);
		m_context->init();

		glfwSetWindowUserPointer(m_window, &m_data);
		setVsync(true);

		// set glfw callbacks
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
			data->width = width;
			data->height = height;
			WindowResizeEvent event(width, height);
			data->eventCallback(event);
		});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data->eventCallback(event);
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mod) {
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
			switch (action) {
				case GLFW_PRESS: 
				{
					KeyPressedEvent event(key, 0);
					data->eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data->eventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data->eventCallback(event);
					break;
				}	
			}
		});

		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode) {
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(keycode);
			data->eventCallback(event);
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data->eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data->eventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data->eventCallback(event);
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData* data = (WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data->eventCallback(event);
		});
	}

	void WindowsWindow::shutdown()
	{
		glfwDestroyWindow(m_window);
	}

	void WindowsWindow::onUpdate()
	{
		glfwPollEvents();
		m_context->swapBuffers();
	}

	uint32_t WindowsWindow::getWidth() const
	{
		return m_data.width;
	}

	uint32_t WindowsWindow::getHeight() const
	{
		return m_data.height;
	}

	void WindowsWindow::setEventCallback(const EventCallbackFn& callback)
	{
		m_data.eventCallback = callback;
	}

	void WindowsWindow::setVsync(bool enabled)
	{
		int interval = enabled ? 1 : 0;
		glfwSwapInterval(interval);
		m_data.vsync = enabled;
	}

	bool WindowsWindow::isVsync() const
	{
		return m_data.vsync;
	}
	void* WindowsWindow::getNativeWindow() const
	{
		return m_window;
	}
}
