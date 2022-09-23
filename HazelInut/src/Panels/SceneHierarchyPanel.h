#pragma once

#include "Hazel/Scene/Scene.h"
#include "Hazel/Scene/Entity.h"

namespace Hazel {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const std::shared_ptr<Scene>& scene);

		void setContext(const std::shared_ptr<Scene>& scene);

		void onImGuiRender();

	private:
		void drawEntityNode(Entity& entity);
		void drawComponents(Entity& entity);

		std::shared_ptr<Scene> m_context;
		Entity m_selectionContext;
	};

}