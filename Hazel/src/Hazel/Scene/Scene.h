#pragma once

#include <entt/entt.hpp>

#include "Hazel/Core/Timestep.h"
#include "Hazel/Renderer/EditorCamera.h"

namespace Hazel {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity createEntity(const std::string& name = std::string());
		void destroyEntity(Entity& entity);

		void onUpdateRuntime(Timestep ts);
		void onUpdateEditor(Timestep ts, EditorCamera& camera);
		void onViewResize(uint32_t width, uint32_t height);

		Entity getPrimaryCameraEntity();

	private:
		template<typename T>
		void onComponentAdded(Entity& entity, T& component);

	private:
		entt::registry m_registry;
		uint32_t m_viewportWidth;
		uint32_t m_viewportHeight;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};

}