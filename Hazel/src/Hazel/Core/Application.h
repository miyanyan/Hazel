#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Core/LayerStack.h"
#include "Hazel/Core/Window.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"

namespace Hazel {
	class ImGuiLayer;
	class HAZEL_API Application
	{
	public:
		Application(const std::string& name = "Hazel App");
		virtual ~Application();

		void run();

		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

		void close();

		ImGuiLayer* getImGuiLayer();

		Window& getWindow();
		static Application& get();

	private:
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);

		std::unique_ptr<Window> m_window;
		bool m_running = true;
		bool m_minimized = false;
		LayerStack m_layerStack;
		ImGuiLayer* m_imguiLayer;

		float m_lastFrameTime = 0.0f;

	private:
		static Application* s_instance;
	};

	// To be defined in CLIENT
	Application* createApplication();
}
