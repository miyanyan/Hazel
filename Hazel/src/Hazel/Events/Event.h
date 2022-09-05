#pragma once

#include <string>
#include <functional>
#include <sstream>

#include "Hazel/Core/Core.h"

namespace Hazel {
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum class EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

	inline int operator&(EventCategory lhs, EventCategory rhs)
	{
		return static_cast<int>(lhs) & static_cast<int>(rhs);
	}

	inline int operator|(EventCategory lhs, EventCategory rhs)
	{
		return static_cast<int>(lhs) | static_cast<int>(rhs);
	}

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::##type; }\
								virtual EventType getEventType() const override { return getStaticType(); }\
								virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return static_cast<int>(category); }

	class HAZEL_API Event
	{
	public:
		virtual EventType getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;
		virtual std::string toString() const
		{
			return getName();
		}

		inline bool isInCategory(EventCategory category)
		{
			return getCategoryFlags() & static_cast<int>(category);
		}

		bool isHandled = false;
	};

	class EventDispatcher
	{
	public:
		template<class T>
		using EventFn = std::function<bool(T&)>;

		EventDispatcher(Event& event)
			: m_event(event)
		{
		}

		template<class T>
		inline bool dispatch(EventFn<T> func)
		{
			if (m_event.getEventType() != T::getStaticType()) {
				return false;
			}
			m_event.isHandled = func(*(T*)&m_event);
			return true;
		}

	private:
		Event& m_event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.toString();
	}
}