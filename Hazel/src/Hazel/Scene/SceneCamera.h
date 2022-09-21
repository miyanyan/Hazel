#pragma once

#include "Hazel/Renderer/Camera.h"

namespace Hazel {

	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void setOrthographic(float size, float neraClip, float farClip);
		
		void setViewportSize(uint32_t width, uint32_t height);

		void setOrthographicSize(float size);
		float getOrthographicSize();

	private:
		void recalculateProjection();

		float m_orthographicSize = 10.0f;
		float m_orthographicNear = -1.0f;
		float m_orthographicFar = 1.0f;
		float m_aspectRatio = 0.0f;
	};

}