#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/LayerStack.h"
#include "Hazel/Window.h"

namespace Hazel {
	class ImGuiLayer;
	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

		Window& getWindow();
		static Application& get();

	private:
		bool onWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_window;
		bool m_running = true;
		LayerStack m_layerStack;
		ImGuiLayer* m_imguiLayer;

		float m_lastFrameTime = 0.0f;

	private:
		static Application* s_instance;
	};

	// To be defined in CLIENT
	Application* createApplication();
}
