#pragma once

#include "Hazel/Renderer/Camera.h"

namespace Hazel {

	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType
		{
			Perspective = 0,
			Orthographic = 1
		};

		SceneCamera();
		virtual ~SceneCamera() = default;

		void setViewportSize(uint32_t width, uint32_t height);

		void setProjectionType(ProjectionType type);
		ProjectionType getProjectionType();
		
		// perspective
		void setPerspective(float fov, float nearClip, float farClip);

		void setPerspectiveFOV(float fov);
		float getPerspectiveFOV();

		void setPerspectiveNearClip(float nearClip);
		float getPerspectiveNearClip();

		void setPerspectiveFarClip(float farClip);
		float getPerspectiveFarClip();

		// orthographic
		void setOrthographic(float size, float nearClip, float farClip);

		void setOrthographicSize(float size);
		float getOrthographicSize();

		void setOrthographicNearClip(float nearClip);
		float getOrthographicNearClip();

		void setOrthographicFarClip(float farClip);
		float getOrthographicFarClip();

	private:
		void recalculateProjection();

		ProjectionType m_projectionType = ProjectionType::Orthographic;

		float m_perspectiveFOV = glm::radians(45.0f);
		float m_perspectiveNear = 0.01f;
		float m_perspectiveFar = 1000.0f;

		float m_orthographicSize = 10.0f;
		float m_orthographicNear = -1.0f;
		float m_orthographicFar = 1.0f;
		float m_aspectRatio = 0.0f;
	};

}