#include "Renderer2D.h"

#include <array>
#include <glm/gtc/matrix_transform.hpp>

#include "VertexArray.h"
#include "ShaderProgram.h"
#include "RenderCommand.h"

namespace Hazel {

	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texcoord;
		float texindex;
		float tilingFactor;
	};

	struct Renderer2DStorage
	{
		static constexpr uint32_t maxQuads = 10000;
		static constexpr uint32_t maxVertices = maxQuads * 4;
		static constexpr uint32_t maxIndices = maxQuads * 6;
		static constexpr uint32_t maxtextureSlots = 32;

		std::shared_ptr<VertexArray> quadVertexArray;
		std::shared_ptr<VertexBuffer> quadVertexBuffer;
		std::shared_ptr<ShaderProgram> textureShader;
		std::shared_ptr<Texture2D> whiteTexture;

		uint32_t quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		std::array<std::shared_ptr<Texture2D>, maxtextureSlots> textureSlots;
		uint32_t textureSlotIndex = 1; // 0 = white texture
	};

	static Renderer2DStorage s_data;

	void Renderer2D::init()
	{
		s_data.quadVertexArray = VertexArray::create();
		s_data.quadVertexBufferBase = new QuadVertex[s_data.maxVertices];

		std::vector<uint32_t> quadIndices(s_data.maxIndices);
		uint32_t offset = 0;
		for (auto i = 0; i < s_data.maxIndices; i += 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		s_data.quadVertexBuffer = Hazel::VertexBuffer::create();
		s_data.quadVertexBuffer->allocate(s_data.quadVertexBufferBase, s_data.maxVertices * sizeof(QuadVertex), GL_DYNAMIC_DRAW);
		s_data.quadVertexBuffer->setLayout({
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float4, "a_Color" },
			{ Hazel::ShaderDataType::Float2, "a_TexCoord" },
			{ Hazel::ShaderDataType::Float, "a_TexIndex" },
			{ Hazel::ShaderDataType::Float, "a_TilingFactor" }
		});
		s_data.quadVertexArray->addVertexBuffer(s_data.quadVertexBuffer);

		auto squareIB = Hazel::IndexBuffer::create();
		squareIB->allocate(quadIndices.data(), quadIndices.size() * sizeof(uint32_t), quadIndices.size());
		s_data.quadVertexArray->setIndexBuffer(squareIB);

		s_data.whiteTexture = Texture2D::create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data.whiteTexture->setData(&whiteTextureData, sizeof(whiteTextureData));

		std::vector<int> samplers(s_data.maxtextureSlots);
		for (auto i = 0; i < s_data.maxtextureSlots; i++)
			samplers[i] = i;

		s_data.textureShader = Hazel::ShaderProgram::create();
		s_data.textureShader->addShaderFromSourceFile(Hazel::Shader::VERTEX, "assets/shaders/Texture.vert");
		s_data.textureShader->addShaderFromSourceFile(Hazel::Shader::FRAGMENT, "assets/shaders/Texture.frag");
		s_data.textureShader->link();

		s_data.textureShader->bind();
		s_data.textureShader->setUniform("u_Textures", samplers);

		// Set all texture slots to 0
		s_data.textureSlots[0] = s_data.whiteTexture;
	}

	void Renderer2D::shutdown()
	{
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		s_data.textureShader->bind();
		s_data.textureShader->setUniform("u_ViewProjection", camera.getViewProjectionMatrix());

		s_data.quadIndexCount = 0;
		s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;

		s_data.textureSlotIndex = 1;
	}

	void Renderer2D::endScene()
	{
		uint32_t dataSize = (uint8_t*)s_data.quadVertexBufferPtr - (uint8_t*)s_data.quadVertexBufferBase;
		s_data.quadVertexBuffer->write(s_data.quadVertexBufferBase, dataSize, 0);

		flush();
	}

	void Renderer2D::flush()
	{
		for (auto i = 0; i < s_data.textureSlotIndex; ++i) {
			s_data.textureSlots[i]->bind(i);
		}
		s_data.quadVertexArray->bind();
		RenderCommand::drawIndexed(s_data.quadVertexArray, s_data.quadIndexCount);
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		const float texIndex = 0.0f; // White Texture
		const float tilingFactor = 1.0f;

		s_data.quadVertexBufferPtr->position = position;
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texcoord = { 0.0f, 0.0f };
		s_data.quadVertexBufferPtr->texindex = texIndex;
		s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = { position.x + size.x, position.y, 0.0f };
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texcoord = { 1.0f, 0.0f };
		s_data.quadVertexBufferPtr->texindex = texIndex;
		s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = { position.x + size.x, position.y + size.y, 0.0f };
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texcoord = { 1.0f, 1.0f };
		s_data.quadVertexBufferPtr->texindex = texIndex;
		s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = { position.x, position.y + size.y, 0.0f };
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texcoord = { 0.0f, 1.0f };
		s_data.quadVertexBufferPtr->texindex = texIndex;
		s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_data.quadVertexBufferPtr++;

		s_data.quadIndexCount += 6;

		/*s_data.textureShader->setUniform("u_TilingFactor", 1.0f);
		s_data.whiteTexture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
							* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data.textureShader->setUniform("u_Transform", transform);
		s_data.quadVertexArray->bind();
		RenderCommand::drawIndexed(s_data.quadVertexArray);*/
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_data.textureSlotIndex; i++)
		{
			if (*s_data.textureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_data.textureSlotIndex;
			s_data.textureSlots[s_data.textureSlotIndex] = texture;
			s_data.textureSlotIndex++;
		}

		s_data.quadVertexBufferPtr->position = position;
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texcoord = { 0.0f, 0.0f };
		s_data.quadVertexBufferPtr->texindex = textureIndex;
		s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = { position.x + size.x, position.y, 0.0f };
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texcoord = { 1.0f, 0.0f };
		s_data.quadVertexBufferPtr->texindex = textureIndex;
		s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = { position.x + size.x, position.y + size.y, 0.0f };
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texcoord = { 1.0f, 1.0f };
		s_data.quadVertexBufferPtr->texindex = textureIndex;
		s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_data.quadVertexBufferPtr++;

		s_data.quadVertexBufferPtr->position = { position.x, position.y + size.y, 0.0f };
		s_data.quadVertexBufferPtr->color = color;
		s_data.quadVertexBufferPtr->texcoord = { 0.0f, 1.0f };
		s_data.quadVertexBufferPtr->texindex = textureIndex;
		s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
		s_data.quadVertexBufferPtr++;

		s_data.quadIndexCount += 6;
		/*
		s_data.textureShader->setUniform("u_Color", tintColor);
		s_data.textureShader->setUniform("u_TilingFactor", tilingFactor);

		texture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data.textureShader->setUniform("u_Transform", transform);

		s_data.quadVertexArray->bind();
		RenderCommand::drawIndexed(s_data.quadVertexArray);
		*/
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, color, rotation);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation)
	{
		s_data.textureShader->setUniform("u_Color", color);
		s_data.textureShader->setUniform("u_TilingFactor", 1.0f);

		s_data.whiteTexture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
							* glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f})
							* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data.textureShader->setUniform("u_Transform", transform);

		s_data.quadVertexArray->bind();
		RenderCommand::drawIndexed(s_data.quadVertexArray);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float rotation, float tilingFactor, const glm::vec4& tintColor)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, texture, rotation, tilingFactor, tintColor);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float rotation, float tilingFactor, const glm::vec4& tintColor)
	{
		s_data.textureShader->setUniform("u_Color", tintColor);
		s_data.textureShader->setUniform("u_TilingFactor", tilingFactor);

		texture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
							* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
							* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_data.textureShader->setUniform("u_Transform", transform);

		s_data.quadVertexArray->bind();
		RenderCommand::drawIndexed(s_data.quadVertexArray);
	}
}