#pragma once

#include <functional>
#include <memory>
#include <type_traits>
#include <typeinfo>

#include <glm/glm.hpp>

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
		glm::mat4 transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform) 
			: transform(transform)
		{
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