#include "Renderer.h"

namespace Hazel {

	Renderer::SceneData* Renderer::s_sceneData = new Renderer::SceneData;

	void Renderer::beginScene(OrthographicCamera& camera)
	{
		s_sceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
	}

	void Renderer::endScene()
	{
	}

	void Renderer::submit(const std::shared_ptr<ShaderProgram>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->bind();
		shader->setUniform("u_ViewProjection", s_sceneData->viewProjectionMatrix);

		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}

}