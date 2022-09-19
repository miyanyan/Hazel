#include "OrthographicCameraController.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"

namespace Hazel {

	OrthographicCameraController::OrthographicCameraController(float aspectRation, bool rotation)
		: m_aspectRation(aspectRation)
		, m_zoom(1.0f)
		, m_camera(OrthographicCamera(-aspectRation * m_zoom, aspectRation* m_zoom, -m_zoom, m_zoom))
		, m_rotation(rotation)
		, m_cameraPosition({0.0f, 0.0f, 0.0f})
		, m_cameraRotation(0.0f)
		, m_cameraRotationSpeed(100.0f)
		, m_cameraTranslationSpeed(5.0f)
	{
	}

	OrthographicCameraController::~OrthographicCameraController()
	{
	}

	void OrthographicCameraController::onUpdate(Timestep ts)
	{
		if (m_rotation) {
			if (Hazel::Input::isKeyPressed(HZ_KEY_A))
				m_cameraRotation += m_cameraRotationSpeed * ts;
			if (Hazel::Input::isKeyPressed(HZ_KEY_D))
				m_cameraRotation -= m_cameraRotationSpeed * ts;

			m_camera.setRotation(m_cameraRotation);
		}

		if (Hazel::Input::isKeyPressed(HZ_KEY_LEFT))
			m_cameraPosition.x -= m_cameraTranslationSpeed * ts;
		else if (Hazel::Input::isKeyPressed(HZ_KEY_RIGHT))
			m_cameraPosition.x += m_cameraTranslationSpeed * ts;

		if (Hazel::Input::isKeyPressed(HZ_KEY_UP))
			m_cameraPosition.y += m_cameraTranslationSpeed * ts;
		else if (Hazel::Input::isKeyPressed(HZ_KEY_DOWN))
			m_cameraPosition.y -= m_cameraTranslationSpeed * ts;

		m_camera.setPosition(m_cameraPosition);

		m_cameraTranslationSpeed = m_zoom;
	}

	void OrthographicCameraController::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>([this](MouseScrolledEvent& e) {return onMouseScrolled(e); });
		dispatcher.dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) {return onWindowResized(e); });
	}

	void OrthographicCameraController::onResize(float width, float height)
	{
		m_aspectRation = width / height;
		m_camera.setProjection(-m_aspectRation * m_zoom, m_aspectRation * m_zoom, -m_zoom, m_zoom);
	}

	OrthographicCamera& OrthographicCameraController::getCamera()
	{
		return m_camera;
	}

	const OrthographicCamera& OrthographicCameraController::getCamera() const
	{
		return m_camera;
	}

	bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e)
	{
		m_zoom -= e.getYOffset() * 0.25f;
		m_zoom = std::max(m_zoom, 0.25f);
		m_camera.setProjection(-m_aspectRation * m_zoom, m_aspectRation * m_zoom, -m_zoom, m_zoom);
		return false;
	}

	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& e)
	{
		onResize(e.getWidth(), e.getHeight());
		return false;
	}

}