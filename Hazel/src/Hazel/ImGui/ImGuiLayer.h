#pragma once

#include "Hazel/Core/Layer.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"


namespace Hazel {

	class HAZEL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onEvent(Event& e) override;

		void blockEvents(bool block);

		void begin();
		void end();

		void setDarkThemeColors();

	private:
		float m_Time = 0.0f;
		bool m_blockEvents = true;
	};

}