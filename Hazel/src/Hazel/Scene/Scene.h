#pragma once

#include <entt/entt.hpp>

#include "Hazel/Core/Timestep.h"

namespace Hazel {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity createEntity(const std::string& name = std::string());

		void onUpdate(Timestep ts);
		void onViewResize(uint32_t width, uint32_t height);

	private:
		entt::registry m_registry;
		uint32_t m_viewportWidth;
		uint32_t m_viewportHeight;

		friend class Entity;
	};

}