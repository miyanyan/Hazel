#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

	SceneCamera::SceneCamera()
	{
		recalculateProjection();
	}

	void SceneCamera::setViewportSize(uint32_t width, uint32_t height)
	{
		m_aspectRatio = 1.0f * width / height;
		recalculateProjection();
	}

	void SceneCamera::setProjectionType(ProjectionType type)
	{
		m_projectionType = type;
	}
	SceneCamera::ProjectionType SceneCamera::getProjectionType()
	{
		return m_projectionType;
	}

	// perspective
	void SceneCamera::setPerspective(float fov, float nearClip, float farClip)
	{
		m_perspectiveFOV = fov;
		m_perspectiveNear = nearClip;
		m_perspectiveFar = farClip;
		recalculateProjection();
	}

	void SceneCamera::setPerspectiveFOV(float fov)
	{
		m_perspectiveFOV = fov;
		recalculateProjection();
	}

	float SceneCamera::getPerspectiveFOV()
	{
		return m_perspectiveFOV;
	}

	void SceneCamera::setPerspectiveNearClip(float nearClip)
	{
		m_perspectiveNear = nearClip;
		recalculateProjection();
	}

	float SceneCamera::getPerspectiveNearClip()
	{
		return m_perspectiveNear;
	}

	void SceneCamera::setPerspectiveFarClip(float farClip)
	{
		m_perspectiveFar = farClip;
		recalculateProjection();
	}

	float SceneCamera::getPerspectiveFarClip()
	{
		return m_perspectiveFar;
	}

	void SceneCamera::setOrthographic(float size, float neraClip, float farClip)
	{
		m_orthographicSize = size;
		m_orthographicNear = neraClip;
		m_orthographicFar = farClip;
		recalculateProjection();
	}


	void SceneCamera::setOrthographicSize(float size)
	{
		m_orthographicSize = size;
		recalculateProjection();
	}

	float SceneCamera::getOrthographicSize()
	{
		return m_orthographicSize;
	}

	void SceneCamera::setOrthographicNearClip(float nearClip)
	{
		m_orthographicNear = nearClip;
		recalculateProjection();
	}

	float SceneCamera::getOrthographicNearClip()
	{
		return m_orthographicNear;
	}

	void SceneCamera::setOrthographicFarClip(float farClip)
	{
		m_orthographicFar = farClip;
		recalculateProjection();
	}

	float SceneCamera::getOrthographicFarClip()
	{
		return m_orthographicFar;
	}

	void SceneCamera::recalculateProjection()
	{
		if (m_projectionType == ProjectionType::Perspective) {
			m_projection = glm::perspective(m_perspectiveFOV, m_aspectRatio, m_perspectiveNear, m_perspectiveFar);
		}
		else {
			float orthoLeft = -m_orthographicSize * m_aspectRatio * 0.5f;
			float orthoRight = m_orthographicSize * m_aspectRatio * 0.5f;
			float orthoBottom = -m_orthographicSize * 0.5f;
			float orthoTop = m_orthographicSize * 0.5f;

			m_projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_orthographicNear, m_orthographicFar);
		}
	}
}