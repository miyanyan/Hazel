#pragma once

#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRation, bool rotation = false);
		~OrthographicCameraController();

		void onUpdate(Timestep ts);
		void onEvent(Event& e);

		OrthographicCamera& getCamera();
		const OrthographicCamera& getCamera() const;

	private:
		bool onMouseScrolled(MouseScrolledEvent& e);
		bool onWindowResized(WindowResizeEvent& e);

	private:
		float m_aspectRation;
		float m_zoom;
		OrthographicCamera m_camera;
		bool m_rotation;

		glm::vec3 m_cameraPosition;
		float m_cameraRotation;
		float m_cameraRotationSpeed;
		float m_cameraTranslationSpeed;
	};

}