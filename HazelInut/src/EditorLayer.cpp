#include "EditorLayer.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Hazel/Utils/Timer.h"

namespace Hazel {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_cameraController(1280.0f / 720.0f), m_squareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
	}

	void EditorLayer::onAttach()
	{
		m_checkerboardTexture = Hazel::Texture2D::create("assets/textures/Checkerboard.png");

		Hazel::FramebufferSpecification fbSpec(1280, 720);
		m_framebuffer = Hazel::Framebuffer::create(fbSpec);

		m_activeScene = std::make_shared<Hazel::Scene>();
		m_squareEntity = m_activeScene->createEntity();
		m_activeScene->reg().emplace<Hazel::TransformComponent>(m_squareEntity);
		m_activeScene->reg().emplace<Hazel::SpriteRendererComponent>(m_squareEntity, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	}

	void EditorLayer::onDetach()
	{
	}

	void EditorLayer::onUpdate(Hazel::Timestep ts)
	{
		// Resize
		if (Hazel::FramebufferSpecification spec = m_framebuffer->getSpecification();
			m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.width != m_viewportSize.x || spec.height != m_viewportSize.y)) {
			m_framebuffer->resize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			m_cameraController.onResize(m_viewportSize.x, m_viewportSize.y);
		}

		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		Hazel::Timer timer;
		// Update
		if (m_viewportFocused) {
			m_cameraController.onUpdate(ts);
		}
		m_profileResults.emplace_back(std::string("CameraController::OnUpdate:"), timer.elapsed<std::chrono::microseconds>());
		timer.reset();

		// Render
		m_framebuffer->bind();
		Hazel::Renderer2D::resetStats();
		Hazel::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::clear();
		m_profileResults.emplace_back(std::string("Renderer Prep:"), timer.elapsed<std::chrono::microseconds>());
		timer.reset();

		Hazel::Renderer2D::beginScene(m_cameraController.getCamera());
		// scene
		m_activeScene->onUpdate(ts);

		Hazel::Renderer2D::endScene();
		m_framebuffer->unbind();
		m_profileResults.emplace_back(std::string("Renderer Draw:"), timer.elapsed<std::chrono::microseconds>());
		timer.reset();
	}

	void EditorLayer::onImGuiRender()
	{

		// Note: Switch this to true to enable dockspace
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit")) Hazel::Application::get().close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");

		auto stats = Hazel::Renderer2D::getStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.drawCalls);
		ImGui::Text("Quads: %d", stats.quadCount);
		ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
		ImGui::Text("Indices: %d", stats.getTotalIndexCount());

		for (auto& [label, time] : m_profileResults) {
			label += "%.5fus";
			ImGui::Text(label.c_str(), time);
		}
		m_profileResults.clear();

		auto& squareColor = m_activeScene->reg().get<Hazel::SpriteRendererComponent>(m_squareEntity).color;
		ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_viewportFocused = ImGui::IsWindowFocused;
		m_viewportHovered = ImGui::IsWindowHovered;
		Application::get().getImGuiLayer()->blockEvents(m_viewportFocused || !m_viewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_framebuffer->getColorAttachmentRendererId();
		ImGui::Image((void*)textureID, ImVec2{ m_viewportSize.x, m_viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::onEvent(Hazel::Event& e)
	{
		m_cameraController.onEvent(e);
	}

}