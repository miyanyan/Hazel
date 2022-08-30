#include "Application.h"

#include <glad/glad.h>

#include "Log.h"
#include "Hazel/ImGui/ImGuiLayer.h"
#include "Platform/OpenGL/OpenGLShaderProgram.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#ifdef _WIN32
#include "Platform/Windows/WindowsWindow.h"
#else
#error "only support windows now"
#endif // _WIN32


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

		auto const severity_str = [severity]() {
			switch (severity) {
				case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
				case GL_DEBUG_SEVERITY_LOW: return "LOW";
				case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
				case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
			}
			return "";
		}();
		HZ_CORE_INFO("OpenGL Debug[{0}, {1}, {2}, {3}]: {4}", src_str, type_str, severity_str, id, message);
	}

	Application* Application::s_instance = nullptr;
	Application::Application()
	{
		s_instance = this;
		m_window = std::make_unique<WindowsWindow>();
		Window::EventCallbackFn fn = [this](Event& e) {onEvent(e); };
		m_window->setEventCallback(fn);

		m_imguiLayer = new ImGuiLayer();
		pushOverlay(m_imguiLayer);

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		unsigned int indices[3] = { 0, 1, 2 };

		m_vertexBuffer = std::make_shared<OpenGLVertexBuffer>();
		m_vertexBuffer->allocate(vertices, sizeof(vertices));
		m_vertexBuffer->setLayout(BufferLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" }
			})
		);

		m_indexBuffer = std::make_shared<OpenGLIndexBuffer>();
		m_indexBuffer->allocate(indices, sizeof(indices), 3);

		m_vertexArray = std::make_shared<OpenGLVertexArray>();
		m_vertexArray->addVertexBuffer(m_vertexBuffer);
		m_vertexArray->setIndexBuffer(m_indexBuffer);

		std::string vertexSrc = R"(
			#version 450 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
				v_Color = a_Color;
			}
		)";

		std::string fragmentSrc = R"(
			#version 450 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_shader = std::make_shared<OpenGLShaderProgram>(vertexSrc, fragmentSrc);

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
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_shader->bind();
			m_vertexArray->bind();
			glDrawElements(GL_TRIANGLES, m_indexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);

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

