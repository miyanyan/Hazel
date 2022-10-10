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
		glm::vec2 texCoord;
		float texIndex;
		float tilingFactor;

		// editor only
		int entityID;
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
		std::vector<QuadVertex> quadVertexBuffers;
		std::vector<QuadVertex>::iterator quadVertexBufferPtr;

		std::array<std::shared_ptr<Texture2D>, maxtextureSlots> textureSlots;
		uint32_t textureSlotIndex = 1; // 0 = white texture

		glm::vec4 quadVertexPositions[4];

		Renderer2D::Statistics stats;
	};

	static Renderer2DStorage s_data;

	void Renderer2D::init()
	{
		s_data.quadVertexArray = VertexArray::create();
		s_data.quadVertexBuffers.resize(s_data.maxVertices);

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
		s_data.quadVertexBuffer->allocate(s_data.quadVertexBuffers.data(), s_data.quadVertexBuffers.size() * sizeof(QuadVertex), GL_DYNAMIC_DRAW);
		s_data.quadVertexBuffer->setLayout({
			{ ShaderDataType::Float3, "a_position" },
			{ ShaderDataType::Float4, "a_color" },
			{ ShaderDataType::Float2, "a_texCoord" },
			{ ShaderDataType::Float, "a_texIndex" },
			{ ShaderDataType::Float, "a_tilingFactor" },
			{ ShaderDataType::Int, "a_EntityID" }
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

		s_data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_data.quadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_data.quadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_data.quadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::shutdown()
	{
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		s_data.textureShader->bind();
		s_data.textureShader->setUniform("u_ViewProjection", camera.getViewProjectionMatrix());

		startBatch();
	}

	void Renderer2D::beginScene(const Camera& camera, const glm::mat4& transform)
	{
		auto viewProject = camera.getProjection() * glm::inverse(transform);

		s_data.textureShader->bind();
		s_data.textureShader->setUniform("u_ViewProjection", viewProject);

		startBatch();
	}

	void Renderer2D::beginScene(const EditorCamera& camera)
	{
		auto viewproj = camera.getViewProjection();

		s_data.textureShader->bind();
		s_data.textureShader->setUniform("u_ViewProjection", viewproj);

		startBatch();
	}

	void Renderer2D::endScene()
	{
		flush();
	}

	void Renderer2D::flush()
	{
		if (s_data.quadIndexCount == 0) {
			return; // nothing to draw
		}
		// write data
		size_t dataSize = s_data.quadIndexCount * sizeof(QuadVertex);
		s_data.quadVertexBuffer->write(s_data.quadVertexBuffers.data(), dataSize, 0);
		// bind texture
		for (auto i = 0; i < s_data.textureSlotIndex; ++i) {
			s_data.textureSlots[i]->bind(i);
		}
		s_data.quadVertexArray->bind();
		RenderCommand::drawIndexed(s_data.quadVertexArray, s_data.quadIndexCount);
		s_data.stats.drawCalls++;
	}

	void Renderer2D::startBatch()
	{
		s_data.quadIndexCount = 0;
		s_data.quadVertexBufferPtr = s_data.quadVertexBuffers.begin();

		s_data.textureSlotIndex = 1;
	}

	void Renderer2D::nextBatch()
	{
		flush();
		startBatch();
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		drawQuad(transform, color);
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintcolor)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintcolor);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintcolor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		drawQuad(transform, texture, tilingFactor, tintcolor);
	}

	void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		if (s_data.quadIndexCount >= Renderer2DStorage::maxIndices)
			nextBatch();

		constexpr float texIndex = 0.0f; // White Texture
		constexpr float tilingFactor = 1.0f;
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		for (size_t i = 0; i < quadVertexCount; ++i) {
			s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[i];
			s_data.quadVertexBufferPtr->color = color;
			s_data.quadVertexBufferPtr->texCoord = textureCoords[i];
			s_data.quadVertexBufferPtr->texIndex = texIndex;
			s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			s_data.quadVertexBufferPtr->entityID = entityID;
			s_data.quadVertexBufferPtr++;
		}

		s_data.quadIndexCount += 6;

		s_data.stats.quadCount++;
	}

	void Renderer2D::drawQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, int entityID)
	{
		if (s_data.quadIndexCount >= Renderer2DStorage::maxIndices)
			nextBatch();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_data.textureSlotIndex; i++) {
			if (*s_data.textureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			if (s_data.textureSlotIndex >= Renderer2DStorage::maxtextureSlots) {
				nextBatch();
			}
			textureIndex = (float)s_data.textureSlotIndex;
			s_data.textureSlots[s_data.textureSlotIndex] = texture;
			s_data.textureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; ++i) {
			s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[i];
			s_data.quadVertexBufferPtr->color = tintColor;
			s_data.quadVertexBufferPtr->texCoord = textureCoords[i];
			s_data.quadVertexBufferPtr->texIndex = textureIndex;
			s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
			s_data.quadVertexBufferPtr->entityID = entityID;
			s_data.quadVertexBufferPtr++;
		}

		s_data.quadIndexCount += 6;

		s_data.stats.quadCount++;
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		drawQuad(transform, color);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintcolor)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintcolor);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, float tilingFactor, const glm::vec4& tintcolor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		drawQuad(transform, texture, tilingFactor, tintcolor);
	}

	void Renderer2D::drawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID)
	{
		drawQuad(transform, src.color, entityID);
	}

	void Renderer2D::resetStats()
	{
		memset(&s_data.stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::getStats()
	{
		return s_data.stats;
	}
}