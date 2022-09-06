#include "Renderer2D.h"

#include "VertexArray.h"
#include "ShaderProgram.h"
#include "RenderCommand.h"

namespace Hazel {

	struct Renderer2DStorage
	{
		std::shared_ptr<VertexArray> quadVertexArray;
		std::shared_ptr<ShaderProgram> flatColorShader;
	};

	static Renderer2DStorage* s_data;

	void Renderer2D::init()
	{
		s_data = new Renderer2DStorage();
		s_data->quadVertexArray = VertexArray::create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		auto squareVB = Hazel::VertexBuffer::create();
		squareVB->allocate(squareVertices, sizeof(squareVertices));
		squareVB->setLayout({
			{ Hazel::ShaderDataType::Float3, "a_Position" }
			});
		s_data->quadVertexArray->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		auto squareIB = Hazel::IndexBuffer::create();
		squareIB->allocate(squareIndices, sizeof(squareIndices), 6);
		s_data->quadVertexArray->setIndexBuffer(squareIB);

		s_data->flatColorShader = Hazel::ShaderProgram::create();
		s_data->flatColorShader->addShaderFromSourceFile(Hazel::Shader::VERTEX, "assets/shaders/FlatColor.vert");
		s_data->flatColorShader->addShaderFromSourceFile(Hazel::Shader::FRAGMENT, "assets/shaders/FlatColor.frag");
		s_data->flatColorShader->link();
	}

	void Renderer2D::shutdown()
	{
		delete s_data;
		s_data = nullptr;
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		s_data->flatColorShader->bind();
		s_data->flatColorShader->setUniform("u_ViewProjection", camera.getViewProjectionMatrix());
		s_data->flatColorShader->setUniform("u_Transform", glm::mat4(1.0f));
	}

	void Renderer2D::endScene()
	{

	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_data->flatColorShader->bind();
		s_data->flatColorShader->setUniform("u_Color", color);

		s_data->quadVertexArray->bind();
		RenderCommand::drawIndexed(s_data->quadVertexArray);
	}

}