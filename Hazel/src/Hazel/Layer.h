#pragma once 

#include "Hazel/Core.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/Event.h"

namespace Hazel {
	class HAZEL_API Layer
	{
	public:
		Layer(const std::string name = "Layer");
		virtual ~Layer();

		virtual void onAttach();
		virtual void onDetach();
		virtual void onUpdate(Timestep ts);
		virtual void onImGuiRender();
		virtual void onEvent(Event& event);

	protected:
		std::string m_debugName;
	};
}