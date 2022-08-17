#pragma once

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

namespace Hazel {

	struct WindowProps
	{
		std::string title;
		uint32_t width;
		uint32_t height;

		WindowProps(const std::string& title = "Hazel Engine", uint32_t width = 1280, uint32_t height = 720)
			: title(title), width(width), height(height)
		{
		}
	};

	class HAZEL_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void onUpdate() = 0;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		virtual void setEventCallback(const EventCallbackFn& callback) = 0;
		virtual void setVsync(bool enabled) = 0;
		virtual bool isVsync() const = 0;

		virtual void* getNativeWindow() const = 0;
	};
}