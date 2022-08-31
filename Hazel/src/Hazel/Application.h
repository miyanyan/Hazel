#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/LayerStack.h"
#include "Hazel/Window.h"
#include "Hazel/Renderer/ShaderProgram.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/OrthographicCamera.h"

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

		std::shared_ptr<VertexArray> m_vertexArray;
		std::shared_ptr<VertexBuffer> m_vertexBuffer;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
		std::shared_ptr<ShaderProgram> m_shader;

		OrthographicCamera m_camera;

	private:
		static Application* s_instance;
	};

	// To be defined in CLIENT
	Application* createApplication();
}
