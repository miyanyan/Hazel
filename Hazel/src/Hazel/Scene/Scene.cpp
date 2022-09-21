#include "Scene.h"

#include "Hazel/Scene/Entity.h"
#include "Hazel/Scene/Components.h"
#include "Hazel/Renderer/Renderer2D.h"

namespace Hazel {

	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	Entity Scene::createEntity(const std::string& name)
	{
		Entity entity(m_registry.create(), this);
		entity.addComponent<TransformComponent>();

		auto& tag = entity.addComponent<TagComponent>();
		tag.tag = name.empty() ? "Entity" : name;

		return entity;
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