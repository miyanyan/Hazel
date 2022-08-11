#include "Application.h"

#include "Log.h"
#ifdef _WIN32
#include "Platform/Windows/WindowsWindow.h"
#else
#error "only support windows now"
#endif // _WIN32


namespace Hazel {
	Application::Application()
	{
		m_window = std::make_unique<WindowsWindow>();
		Window::EventCallbackFn fn = [this](Event& e) {onEvent(e); };
		m_window->setEventCallback(fn);
	}

	Application::~Application()
	{
	}

	void Application::run()
	{
		while (m_running) {
			m_window->onUpdate();
		}
	}

	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		EventDispatcher::EventFn<WindowCloseEvent> fn = [this](WindowCloseEvent& e) {return onWindowClose(e); };
		dispatcher.dispatch(fn);
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}
}

