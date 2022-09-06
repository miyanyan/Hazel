#include "Sandbox2D.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D")
	, m_cameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::onAttach()
{
}

void Sandbox2D::onDetach()
{
}

void Sandbox2D::onUpdate(Hazel::Timestep ts)
{
	// Update
	m_cameraController.onUpdate(ts);

	// Render
	Hazel::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Hazel::RenderCommand::clear();

	Hazel::Renderer2D::beginScene(m_cameraController.getCamera());
	Hazel::Renderer2D::drawQuad({ 0.0f, 0.0f, }, { 1.0f, 1.0f }, m_squareColor);
	Hazel::Renderer2D::endScene();
}

void Sandbox2D::onImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));
	ImGui::End();
}

void Sandbox2D::onEvent(Hazel::Event& e)
{
	m_cameraController.onEvent(e);
}
