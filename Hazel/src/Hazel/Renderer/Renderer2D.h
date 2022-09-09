#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"

namespace Hazel {

	class Renderer2D
	{
	public:
		static void init();
		static void shutdown();

		static void beginScene(const OrthographicCamera& camera);
		static void endScene();

		// primitives
		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation);
		static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation);

		static void drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float rotation, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float rotation, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
	};

}