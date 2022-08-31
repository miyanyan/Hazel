#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "ShaderProgram.h"

namespace Hazel {

	class Renderer
	{
	public:
		static void beginScene(OrthographicCamera& camera);
		static void endScene();

		static void submit(const std::shared_ptr<ShaderProgram>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
		};
		static SceneData* s_sceneData;
	};


}
