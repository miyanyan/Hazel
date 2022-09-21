#include "Scene.h"

#include "Hazel/Scene/Components.h"
#include "Hazel/Renderer/Renderer2D.h"

namespace Hazel {

	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	entt::entity Scene::createEntity()
	{
		return m_registry.create();
	}

	entt::registry& Scene::reg()
	{
		return m_registry;
	}

	void Scene::onUpdate(Timestep ts)
	{
		auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (const auto& entity : group) {
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::drawQuad(transform.transform, sprite.color);
		}
	}

}