#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "Hazel.h"
#include "Hazel/Utils/SourceLocation.h"

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer() 
		: Layer("Example") 
		, m_cameraController(1280.0f / 720.0f, true)
	{
		// triangle
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		unsigned int indices[3] = { 0, 1, 2 };

		m_vertexBuffer.reset(Hazel::VertexBuffer::create());
		m_vertexBuffer->allocate(vertices, sizeof(vertices));
		m_vertexBuffer->setLayout(Hazel::BufferLayout({
				{ Hazel::ShaderDataType::Float3, "a_Position" },
				{ Hazel::ShaderDataType::Float4, "a_Color" }
			})
		);

		m_indexBuffer.reset(Hazel::IndexBuffer::create());
		m_indexBuffer->allocate(indices, sizeof(indices), 3);

		m_vertexArray.reset(Hazel::VertexArray::create());
		m_vertexArray->addVertexBuffer(m_vertexBuffer);
		m_vertexArray->setIndexBuffer(m_indexBuffer);

		// square
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f 
		};
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		std::shared_ptr<Hazel::VertexBuffer> squareVBO;
		squareVBO.reset(Hazel::VertexBuffer::create());
		squareVBO->allocate(squareVertices, sizeof(squareVertices));
		squareVBO->setLayout({
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float2, "a_TexCoord" }
		});

		std::shared_ptr<Hazel::IndexBuffer> squareIBO;
		squareIBO.reset(Hazel::IndexBuffer::create());
		squareIBO->allocate(squareIndices, sizeof(squareIndices), sizeof(squareIndices) / sizeof(uint32_t));

		m_squareVAO.reset(Hazel::VertexArray::create());
		m_squareVAO->addVertexBuffer(squareVBO);
		m_squareVAO->setIndexBuffer(squareIBO);

		// triangle shader
		std::string vertexSrc = R"(
			#version 450 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
				v_Color = a_Color;
			}
		)";

		std::string fragmentSrc = R"(
			#version 450 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_shader.reset(Hazel::ShaderProgram::create());
		m_shader->addShaderFromSourceCode(Hazel::Shader::VERTEX, vertexSrc);
		m_shader->addShaderFromSourceCode(Hazel::Shader::FRAGMENT, fragmentSrc);
		m_shader->link();

		// square shader
		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			
			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_flatColorShader.reset(Hazel::ShaderProgram::create());
		m_flatColorShader->addShaderFromSourceCode(Hazel::Shader::VERTEX, flatColorShaderVertexSrc);
		m_flatColorShader->addShaderFromSourceCode(Hazel::Shader::FRAGMENT, flatColorShaderFragmentSrc);
		m_flatColorShader->link();
	
		// texture shader
		m_textureShader.reset(Hazel::ShaderProgram::create());
		m_textureShader->addShaderFromSourceFile(Hazel::Shader::VERTEX, "./assets/shaders/Texture.vert");
		m_textureShader->addShaderFromSourceFile(Hazel::Shader::FRAGMENT, "./assets/shaders/Texture.frag");
		m_textureShader->link();

		m_textureShader->bind();
		m_textureShader->setUniform("u_Texture", 0);

		m_texture.reset(Hazel::Texture2D::create("./assets/textures/Checkerboard.png"));
		m_logoTexture.reset(Hazel::Texture2D::create("./assets/textures/ChernoLogo.png"));
    }
	
	void onUpdate(Hazel::Timestep ts) override {

		m_cameraController.onUpdate(ts);

		Hazel::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Hazel::RenderCommand::clear();

		Hazel::Renderer::beginScene(m_cameraController.getCamera());

		m_flatColorShader->bind();
		m_flatColorShader->setUniform("u_Color", m_squareColor);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		for (int x = 0; x < 20; ++x) {
			for (int y = 0; y < 20; ++y) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Hazel::Renderer::submit(m_flatColorShader, m_squareVAO, transform);
			}
		}

		Hazel::Renderer::submit(m_shader, m_vertexArray);
		m_texture->bind();
		Hazel::Renderer::submit(m_textureShader, m_squareVAO, glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));
		m_logoTexture->bind();
		Hazel::Renderer::submit(m_textureShader, m_squareVAO, glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));

		Hazel::Renderer::endScene();
	}

	void onEvent(Hazel::Event& e) override {
		m_cameraController.onEvent(e);
	}

	void onImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_squareColor));
		ImGui::End();
	}


private:
	std::shared_ptr<Hazel::VertexArray> m_vertexArray, m_squareVAO;
	std::shared_ptr<Hazel::VertexBuffer> m_vertexBuffer;
	std::shared_ptr<Hazel::IndexBuffer> m_indexBuffer;
	std::shared_ptr<Hazel::Texture2D> m_texture, m_logoTexture;
	std::shared_ptr<Hazel::ShaderProgram> m_shader, m_flatColorShader, m_textureShader;

	Hazel::OrthographicCameraController m_cameraController;

	glm::vec3 m_squareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		pushLayer(new ExampleLayer());
	}
	~Sandbox(){}

private:
	
};

Hazel::Application* Hazel::createApplication()
{
	return new Sandbox();
}