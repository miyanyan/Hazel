#pragma once

#include <memory>

#include "Hazel/Scene/Scene.h"

namespace Hazel {

	class SceneSerializer
	{
	public:
		SceneSerializer(const std::shared_ptr<Scene>& scene);

		void serialize(const char* filepath);
		void serializeRuntime(const char* filepath);

		bool deserialize(const char* filepath);
		bool deserializeRuntime(const char* filepath);

	private:
		std::shared_ptr<Scene> m_scene;
	};

}