#pragma once

#include "Hazel/Window.h"
#include "Hazel/Renderer/GraphicsContext.h"

class GLFWwindow;

namespace Hazel {
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props = WindowProps());
		virtual ~WindowsWindow() override;

		virtual void onUpdate() override;

		virtual uint32_t getWidth() const override;
		virtual uint32_t getHeight() const override;

		virtual void setEventCallback(const EventCallbackFn& callback) override;
		virtual void setVsync(bool enabled) override;
		virtual bool isVsync() const override;

		virtual void* getNativeWindow() const override;

	private:
		virtual void init(const WindowProps& props);
		virtual void shutdown();

		GLFWwindow* m_window;
		std::unique_ptr<GraphicsContext> m_context;

		struct WindowData
		{
			std::string title;
			uint32_t width, height;
			bool vsync;
			EventCallbackFn eventCallback;
		};

		WindowData m_data;
	};
}