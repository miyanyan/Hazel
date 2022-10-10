#include "EditorLayer.h"

#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <nfd.h>

#include "Hazel/Utils/Math.h"
#include "Hazel/Utils/Timer.h"
#include "Hazel/Scene/SceneSerializer.h"

namespace Hazel {

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_cameraController(1280.0f / 720.0f), m_squareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
	}

	void EditorLayer::onAttach()
	{
		m_checkerboardTexture = Hazel::Texture2D::create("assets/textures/Checkerboard.png");

		Hazel::Framebuffer::Specification fbSpec(1280, 720);
		m_framebuffer = Hazel::Framebuffer::create(fbSpec);

		m_activeScene = std::make_shared<Hazel::Scene>();

		m_editorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
#if 0
		m_squareEntity = m_activeScene->createEntity("Green Square");
		m_squareEntity.addComponent<Hazel::SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		auto redSquare = m_activeScene->createEntity("Red Square");
		redSquare.addComponent<Hazel::SpriteRendererComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		m_cameraEntity = m_activeScene->createEntity("Camera A");
		m_cameraEntity.addComponent<Hazel::CameraComponent>();

		m_secondCamera = m_activeScene->createEntity("Camera B");
		auto& cc = m_secondCamera.addComponent<CameraComponent>();
		cc.primary = false;

		// camera script
		class CameraController : public ScriptableEntity
		{
		public:
			void onCreate() override
			{
				auto& translation = getComponent<TransformComponent>().translation;
				translation.x = rand() % 10 - 5.0f;
			}

			void onUpdate(Timestep ts) override
			{
				auto& translation = getComponent<TransformComponent>().translation;
				float speed = 5.0f;

				if (Input::isKeyPressed(HZ_KEY_A))
					translation.x -= speed * ts;
				if (Input::isKeyPressed(HZ_KEY_D))
					translation.x += speed * ts;
				if (Input::isKeyPressed(HZ_KEY_W))
					translation.y += speed * ts;
				if (Input::isKeyPressed(HZ_KEY_S))
					translation.y -= speed * ts;
			}
		};

		m_cameraEntity.addComponent<NativeScriptComponent>().bind<CameraController>();
		m_secondCamera.addComponent<NativeScriptComponent>().bind<CameraController>();
#endif
		m_sceneHierarchyPanel.setContext(m_activeScene);
	}

	void EditorLayer::onDetach()
	{
	}

	void EditorLayer::onUpdate(Hazel::Timestep ts)
	{
		// Resize
		if (auto spec = m_framebuffer->getSpecification();
			m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.width != m_viewportSize.x || spec.height != m_viewportSize.y)) {
			m_framebuffer->resize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			m_cameraController.onResize(m_viewportSize.x, m_viewportSize.y);
			m_editorCamera.setViewportSize(m_viewportSize.x, m_viewportSize.y);
			m_activeScene->onViewResize(m_viewportSize.x, m_viewportSize.y);
		}

		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		Hazel::Timer timer;
		// Update
		if (m_viewportFocused) {
			m_cameraController.onUpdate(ts);
		}
		m_editorCamera.onUpdate(ts);
		m_profileResults.emplace_back(std::string("CameraController::OnUpdate:"), timer.elapsed<std::chrono::microseconds>());
		timer.reset();

		// Render
		m_framebuffer->bind();
		Hazel::Renderer2D::resetStats();
		Hazel::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::clear();
		m_profileResults.emplace_back(std::string("Renderer Prep:"), timer.elapsed<std::chrono::microseconds>());
		timer.reset();

		// scene
		m_activeScene->onUpdateEditor(ts, m_editorCamera);

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

				if (ImGui::MenuItem("New", "Ctrl+N")) {
					newScene();
				}

				if (ImGui::MenuItem("Open...", "Ctrl+O")) {
					openScene();
				}

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
					saveSceneAs();
				}

				if (ImGui::MenuItem("Exit")) Hazel::Application::get().close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_sceneHierarchyPanel.onImGuiRender();

		ImGui::Begin("Stats");

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

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_viewportFocused = ImGui::IsWindowFocused;
		m_viewportHovered = ImGui::IsWindowHovered;
		Application::get().getImGuiLayer()->blockEvents(!m_viewportFocused && !m_viewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint32_t textureID = m_framebuffer->getColorAttachmentRendererId();
		ImGui::Image((void*)textureID, ImVec2{ m_viewportSize.x, m_viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		// gizmos
		Entity selectEntity = m_sceneHierarchyPanel.getSelectedEntity();
		if (selectEntity && m_gizmoType != -1) {
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float width = ImGui::GetWindowWidth();
			float height = ImGui::GetWindowHeight();
			float posx = ImGui::GetWindowPos().x;
			float posy = ImGui::GetWindowPos().y;
			ImGuizmo::SetRect(posx, posy, width, height);

			// camera
			const auto& cameraProjection = m_editorCamera.getProjection();
			glm::mat4 cameraView = m_editorCamera.getViewMatrix();

			// entity transform
			auto& tc = selectEntity.getComponent<TransformComponent>();
			glm::mat4 transform = tc.getTransform();

			// snapping
			bool snap = Input::isKeyPressed(HZ_KEY_LEFT_CONTROL);
			float snapValue = m_gizmoType == ImGuizmo::OPERATION::ROTATE ? 45.0f : 0.5f; // 0.5m for translation/scale, 45 degree for rotation
			float snapValues[] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing()) {
				glm::vec3 translation, scale, rotation;
				if (Math::decomposeTransform(transform, translation, rotation, scale)) {
					tc.translation = translation;
					tc.rotation = rotation;
					tc.scale = scale;
				}
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::onEvent(Hazel::Event& e)
	{
		m_cameraController.onEvent(e);
		m_editorCamera.onEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.dispatch<KeyPressedEvent>([this](KeyPressedEvent& e) {return onKeyPressed(e); });
	}

	bool EditorLayer::onKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if (e.getRepeatCount() > 0)
			return false;

		bool control = Input::isKeyPressed(HZ_KEY_LEFT_CONTROL) || Input::isKeyPressed(HZ_KEY_RIGHT_CONTROL);
		bool shift = Input::isKeyPressed(HZ_KEY_LEFT_SHIFT) || Input::isKeyPressed(HZ_KEY_RIGHT_SHIFT);
		switch (e.getKeyCode())
		{
			case HZ_KEY_N:
			{
				if (control)
					newScene();

				break;
			}
			case HZ_KEY_O:
			{
				if (control)
					openScene();

				break;
			}
			case HZ_KEY_S:
			{
				if (control && shift)
					saveSceneAs();

				break;
			}
			case HZ_KEY_Q:
				m_gizmoType = -1;
				break;
			case HZ_KEY_W:
				m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case HZ_KEY_E:
				m_gizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case HZ_KEY_R:
				m_gizmoType = ImGuizmo::OPERATION::SCALE;
				break;
		}
	}

	void EditorLayer::newScene()
	{
		m_activeScene = std::make_shared<Scene>();
		m_activeScene->onViewResize(m_viewportSize.x, m_viewportSize.y);
		m_sceneHierarchyPanel.setContext(m_activeScene);
	}

	void EditorLayer::openScene()
	{
		nfdchar_t* filepath = nullptr;
		nfdresult_t result = NFD_OpenDialog("hazel", nullptr, &filepath);
		if (result == NFD_OKAY) {
			m_activeScene = std::make_shared<Scene>();
			m_activeScene->onViewResize(m_viewportSize.x, m_viewportSize.y);
			m_sceneHierarchyPanel.setContext(m_activeScene);

			SceneSerializer serializer(m_activeScene);
			serializer.deserialize(filepath);
		}
	}

	void EditorLayer::saveSceneAs()
	{
		nfdchar_t* filepath = nullptr;
		nfdresult_t result = NFD_SaveDialog("hazel", nullptr, &filepath);
		if (result == NFD_OKAY) {
			SceneSerializer serializer(m_activeScene);
			std::string path = std::string(filepath) + ".hazel";
			serializer.serialize(path.c_str());
		}
	}

}