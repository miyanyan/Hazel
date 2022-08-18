#include "Application.h"

#include <glad/glad.h>

#include "Log.h"
#include "Hazel/ImGui/ImGuiLayer.h"
#ifdef _WIN32
#include "Platform/Windows/WindowsWindow.h"
#else
#error "only support windows now"
#endif // _WIN32


namespace Hazel {
	Application* Application::s_instance = nullptr;
	Application::Application()
	{
		s_instance = this;
		m_window = std::make_unique<WindowsWindow>();
		Window::EventCallbackFn fn = [this](Event& e) {onEvent(e); };
		m_window->setEventCallback(fn);

		m_imguiLayer = new ImGuiLayer();
		pushOverlay(m_imguiLayer);
	}

	Application::~Application()
	{
	}

	void Application::run()
	{
		while (m_running) {
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			for (auto layer : m_layerStack) {
				layer->onUpdate();
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
		EventDispatcher::EventFn<WindowCloseEvent> fn = [this](WindowCloseEvent& e) {return onWindowClose(e); };
		dispatcher.dispatch(fn);

		for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it) {
			(*it)->onEvent(e);
			if (e.isHandled) {
				break;
			}
		}
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}

	void Application::pushLayer(Layer* layer)
	{
		m_layerStack.pushLayer(layer);
	}

	void Application::pushOverlay(Layer* layer)
	{
		m_layerStack.pushOverlay(layer);
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

