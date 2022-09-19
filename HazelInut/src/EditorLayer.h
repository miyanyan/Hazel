#pragma once

#include "Hazel.h"

namespace Hazel {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void onAttach() override;
		virtual void onDetach() override;

		void onUpdate(Timestep ts) override;
		virtual void onImGuiRender() override;
		void onEvent(Event& e) override;
	private:
		Hazel::OrthographicCameraController m_cameraController;

		std::shared_ptr<VertexArray> m_squareVA;
		std::shared_ptr<Shader> m_flatColorShader;
		std::shared_ptr<Framebuffer> m_framebuffer;

		std::shared_ptr<Texture2D> m_checkerboardTexture;

		bool m_viewportFocused = false, m_viewportHovered = false;
		glm::vec2 m_viewportSize = { 0.0f, 0.0f };
		
		glm::vec4 m_squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
		
		struct ProfileResult
		{
			std::string name;
			double time;
			ProfileResult(std::string&& name, double time) : name(std::move(name)), time(time) {}
		};
		std::vector<ProfileResult> m_profileResults;
	};

}