#include "Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

#include "VertexArray.h"
#include "ShaderProgram.h"
#include "RenderCommand.h"

namespace Hazel {

	struct Renderer2DStorage
	{
		std::shared_ptr<VertexArray> quadVertexArray;
		std::shared_ptr<ShaderProgram> textureShader;
		std::shared_ptr<Texture2D> whiteTexture;
	};

	static Renderer2DStorage* s_data;

	void Renderer2D::init()
	{
		s_data = new Renderer2DStorage();
		s_data->quadVertexArray = VertexArray::create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f 
		};

		auto squareVB = Hazel::VertexBuffer::create();
		squareVB->allocate(squareVertices, sizeof(squareVertices));
		squareVB->setLayout({
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float2, "a_TexCoord" }
		});
		s_data->quadVertexArray->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		auto squareIB = Hazel::IndexBuffer::create();
		squareIB->allocate(squareIndices, sizeof(squareIndices), 6);
		s_data->quadVertexArray->setIndexBuffer(squareIB);

		s_data->textureShader = Hazel::ShaderProgram::create();
		s_data->textureShader->addShaderFromSourceFile(Hazel::Shader::VERTEX, "assets/shaders/Texture.vert");
		s_data->textureShader->addShaderFromSourceFile(Hazel::Shader::FRAGMENT, "assets/shaders/Texture.frag");
		s_data->textureShader->link();

		s_data->whiteTexture = Texture2D::create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data->whiteTexture->setData(&whiteTextureData, sizeof(whiteTextureData));
	}

	void Renderer2D::shutdown()
	{
		delete s_data;
		s_data = nullptr;
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		s_data->textureShader->bind();
		s_data->textureShader->setUniform("u_ViewProjection", camera.getViewProjectionMatrix());
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
		s_data->textureShader->setUniform("u_Color", color);
		s_data->textureShader->setUniform("u_TilingFactor", 1.0f);

		s_data->whiteTexture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data->textureShader->setUniform("u_Transform", transform);

		s_data->quadVertexArray->bind();
		RenderCommand::drawIndexed(s_data->quadVertexArray);
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		s_data->textureShader->setUniform("u_Color", tintColor);
		s_data->textureShader->setUniform("u_TilingFactor", tilingFactor);

		texture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data->textureShader->setUniform("u_Transform", transform);

		s_data->quadVertexArray->bind();
		RenderCommand::drawIndexed(s_data->quadVertexArray);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, color, rotation);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation)
	{
		s_data->textureShader->setUniform("u_Color", color);
		s_data->textureShader->setUniform("u_TilingFactor", 1.0f);

		s_data->whiteTexture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
							* glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
							* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data->textureShader->setUniform("u_Transform", transform);

		s_data->quadVertexArray->bind();
		RenderCommand::drawIndexed(s_data->quadVertexArray);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float rotation, float tilingFactor, const glm::vec4& tintColor)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, texture, rotation, tilingFactor, tintColor);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float rotation, float tilingFactor, const glm::vec4& tintColor)
	{
		s_data->textureShader->setUniform("u_Color", tintColor);
		s_data->textureShader->setUniform("u_TilingFactor", tilingFactor);

		texture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
							* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
							* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data->textureShader->setUniform("u_Transform", transform);

		s_data->quadVertexArray->bind();
		RenderCommand::drawIndexed(s_data->quadVertexArray);
	}
}