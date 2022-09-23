#pragma once

#include <entt/entt.hpp>

#include "Hazel/Scene/Scene.h"
#include "Hazel/Core/Log.h"

namespace Hazel {

	class Entity
	{
	public:
		Entity() = default;
		Entity(const Entity&) = default;
		Entity(entt::entity handle, Scene* scene);
		~Entity();

		template<typename T, typename... Args>
		T& addComponent(Args&&... args)
		{
			return m_scene->m_registry.emplace<T>(m_entity, std::forward<Args>(args)...);
		}

		template<typename T>
		T& getComponent()
		{
			return m_scene->m_registry.get<T>(m_entity);
		}

		template<typename T>
		void removeComponent()
		{
			m_scene->m_registry.remove<T>(m_entity);
		}

		template<typename T>
		bool hasComponent()
		{
			return m_scene->m_registry.all_of<T>(m_entity);
		}

		operator bool() const { return m_entity != entt::null; }
		operator uint32_t() const { return (uint32_t)m_entity; }

		bool operator==(const Entity& other) const
		{
			return m_entity == other.m_entity && m_scene == other.m_scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		entt::entity m_entity{ entt::null };
		Scene* m_scene{nullptr};
	};

}
