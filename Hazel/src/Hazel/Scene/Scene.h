#pragma once

#include <entt/entt.hpp>

#include "Hazel/Core/Timestep.h"

namespace Hazel {

	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity createEntity();

		entt::registry& reg();

		void onUpdate(Timestep ts);

	private:
		entt::registry m_registry;
	};

}