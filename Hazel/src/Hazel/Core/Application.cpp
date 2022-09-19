#include "Application.h"

#include "Log.h"
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/ImGui/ImGuiLayer.h"
#include "Platform/OpenGL/OpenGLShaderProgram.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#ifdef _WIN32
#include "Platform/Windows/WindowsWindow.h"
#else
#error "only support windows now"
#endif // _WIN32
#include <GLFW/glfw3.h>


namespace Hazel {

	void msgCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
	{
		auto const src_str = [source]() {
			switch (source)
			{
				case GL_DEBUG_SOURCE_API: return "API";
				case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
				case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
				case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
				case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
				case GL_DEBUG_SOURCE_OTHER: return "OTHER";
			}
			return "";
		}();

		auto const type_str = [type]() {
			switch (type)
			{
				case GL_DEBUG_TYPE_ERROR: return "ERROR";
				case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
				case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
				case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
				case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
				case GL_DEBUG_TYPE_MARKER: return "MARKER";
				case GL_DEBUG_TYPE_OTHER: return "OTHER";
			}
			return "";
		}();

		switch (severity) {
			case GL_DEBUG_SEVERITY_NOTIFICATION: 
				HZ_CORE_TRACE("OpenGL[{}, {}, {}]: {}", src_str, type_str, id, message);
				break;
			case GL_DEBUG_SEVERITY_LOW: 
				HZ_CORE_INFO("OpenGL[{}, {}, {}]: {}", src_str, type_str, id, message);
				break;
			case GL_DEBUG_SEVERITY_MEDIUM: 
				HZ_CORE_WARN("OpenGL[{}, {}, {}]: {}", src_str, type_str, id, message);
				break;
			case GL_DEBUG_SEVERITY_HIGH:
				HZ_CORE_ERROR("OpenGL[{}, {}, {}]: {}", src_str, type_str, id, message);
				break;
		}
	}

	Application* Application::s_instance = nullptr;
	Application::Application(const std::string& name)
	{
		s_instance = this;
		m_window = std::make_unique<WindowsWindow>(WindowProps(name));
		Window::EventCallbackFn fn = [this](Event& e) {onEvent(e); };
		m_window->setEventCallback(fn);

		Renderer::init();

		m_imguiLayer = new ImGuiLayer();
		pushOverlay(m_imguiLayer);

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(msgCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
	}

	Application::~Application()
	{
	}

	void Application::run()
	{
		while (m_running) {
			float time = glfwGetTime();
			Timestep timestep = time - m_lastFrameTime;
			m_lastFrameTime = time;

			if (!m_minimized) {
				for (auto layer : m_layerStack) {
					layer->onUpdate(timestep);
				}
			}

			m_imguiLayer->begin();
			for (auto layer : m_layerStack) {
				layer->onImGuiRender();
			}
			m_imguiLayer->end();

			m_window->onUpdate();
		}
	}

	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) {return onWindowClose(e); });
		dispatcher.dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) {return onWindowResize(e); });

		for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it) {
			if (e.isHandled) {
				break;
			}
			(*it)->onEvent(e);
		}
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& e)
	{
		m_minimized = e.getWidth() == 0 || e.getHeight() == 0;
		Renderer::onWindowResize(e.getWidth(), e.getHeight());
		return false;
	}

	void Application::pushLayer(Layer* layer)
	{
		m_layerStack.pushLayer(layer);
	}

	void Application::pushOverlay(Layer* layer)
	{
		m_layerStack.pushOverlay(layer);
	}

	void Application::close()
	{
		m_running = false;
	}

	ImGuiLayer* Application::getImGuiLayer()
	{
		return m_imguiLayer;
	}

	Window& Application::getWindow()
	{
		return *m_window;
	}

	Application& Application::get()
	{
		return *s_instance;
	}
}

