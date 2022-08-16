#pragma once 

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

namespace Hazel {
	class HAZEL_API Layer
	{
	public:
		Layer(const std::string name = "Layer");
		virtual ~Layer();

		virtual void onAttach();
		virtual void onDetach();
		virtual void onUpdate();
		virtual void onEvent(Event& event);

	protected:
		std::string m_debugName;
	};
}