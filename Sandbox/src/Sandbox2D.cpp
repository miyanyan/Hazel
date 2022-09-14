#include "Sandbox2D.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Hazel/Utils/Timer.h"

using namespace std::literals;

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D")
	, m_cameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::onAttach()
{
	m_checkerBoardTexture = Hazel::Texture2D::create("assets/textures/Checkerboard.png");
}

void Sandbox2D::onDetach()
{
}

void Sandbox2D::onUpdate(Hazel::Timestep ts)
{
	static float rotation = 0.0f;
	rotation += ts * 50.0f;

	Hazel::Timer timer;
	// Update
	m_cameraController.onUpdate(ts);
	m_profileResults.emplace_back("CameraController::OnUpdate:"s, timer.elapsed<std::chrono::microseconds>());
	timer.reset();

	// Render
	Hazel::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Hazel::RenderCommand::clear();
	m_profileResults.emplace_back("Renderer Prep:"s, timer.elapsed<std::chrono::microseconds>());
	timer.reset();

	Hazel::Renderer2D::beginScene(m_cameraController.getCamera());
	Hazel::Renderer2D::drawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f }, -45.0f);
	Hazel::Renderer2D::drawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});
	Hazel::Renderer2D::drawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_squareColor);
	Hazel::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_checkerBoardTexture, 10.0f);
	Hazel::Renderer2D::drawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, m_checkerBoardTexture, rotation, 20.0f);
	Hazel::Renderer2D::endScene();
	m_profileResults.emplace_back("Renderer Draw:"s, timer.elapsed<std::chrono::microseconds>());
	timer.reset();
}

void Sandbox2D::onImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));

	for (auto& [label, time] : m_profileResults) {
		label += "%.5fus";
		ImGui::Text(label.c_str(), time);
	}
	m_profileResults.clear();
	ImGui::End();
}

void Sandbox2D::onEvent(Hazel::Event& e)
{
	m_cameraController.onEvent(e);
}
