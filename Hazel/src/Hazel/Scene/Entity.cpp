#include "Entity.h"

namespace Hazel {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_entity(handle)
		, m_scene(scene)
	{
	}

	Entity::~Entity()
	{
	}

}
