#pragma once

#include "Hazel.h"

class Sandbox2D : public Hazel::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void onAttach() override;
	virtual void onDetach() override;

	void onUpdate(Hazel::Timestep ts) override;
	virtual void onImGuiRender() override;
	void onEvent(Hazel::Event& e) override;
private:
	Hazel::OrthographicCameraController m_cameraController;

	// Temp
	std::shared_ptr<Hazel::VertexArray> m_squareVA;
	std::shared_ptr<Hazel::ShaderProgram> m_flatColorShader;

	glm::vec4 m_squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};