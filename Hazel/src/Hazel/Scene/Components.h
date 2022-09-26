#pragma once

#include <functional>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Hazel/Scene/SceneCamera.h"
#include "Hazel/Scene/ScriptableEntity.h"

namespace Hazel {

	struct TagComponent
	{
		std::string tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: tag(tag)
		{
		}
	};

	struct TransformComponent
	{
		glm::vec3 translation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation) 
			: translation(translation)
		{
		}

		glm::mat4 getTransform() const
		{
			glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation.x, { 1, 0, 0 })
				* glm::rotate(glm::mat4(1.0f), rotation.y, { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), rotation.z, { 0, 0, 1 });

			return glm::translate(glm::mat4(1.0f), translation) * rotationMatrix * glm::scale(glm::mat4(1.0f), scale);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: color(color)
		{
		}
	};

	struct CameraComponent
	{
		SceneCamera camera;
		bool primary = true;
		bool fixedAspectRatio = false;
	};

	struct NativeScriptComponent
	{
		std::unique_ptr<ScriptableEntity> instance;

		std::function<void()> instantiateFunction;

		template<typename T>
		void bind()
		{
			static_assert(std::is_base_of_v<ScriptableEntity, T>, "T must derived from ScriptableEntity");
			instantiateFunction = [&]() {
				instance = std::make_unique<T>();
			};
		}
	};
}