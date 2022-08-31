#include <Hazel.h>

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer() 
		: Layer("Example") 
		, m_camera(-1.6f, 1.6f, -0.9f, 0.9f)
		, m_cameraPosition(0.0f)
	{
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

		std::string vertexSrc = R"(
			#version 450 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
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

		m_shader.reset(Hazel::ShaderProgram::create(vertexSrc, fragmentSrc));
	}
	
	void onUpdate(Hazel::TimeStep ts) override {

		if (Hazel::Input::isKeyPressed(HZ_KEY_LEFT))
			m_cameraPosition.x -= m_cameraMoveSpeed * ts;
		else if (Hazel::Input::isKeyPressed(HZ_KEY_RIGHT))
			m_cameraPosition.x += m_cameraMoveSpeed * ts;

		if (Hazel::Input::isKeyPressed(HZ_KEY_UP))
			m_cameraPosition.y += m_cameraMoveSpeed * ts;
		else if (Hazel::Input::isKeyPressed(HZ_KEY_DOWN))
			m_cameraPosition.y -= m_cameraMoveSpeed * ts;

		if (Hazel::Input::isKeyPressed(HZ_KEY_A))
			m_cameraRotation += m_cameraRotationSpeed * ts;
		if (Hazel::Input::isKeyPressed(HZ_KEY_D))
			m_cameraRotation -= m_cameraRotationSpeed * ts;

		Hazel::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Hazel::RenderCommand::clear();

		m_camera.setPosition(m_cameraPosition);
		m_camera.setRotation(m_cameraRotation);

		Hazel::Renderer::beginScene(m_camera);

		Hazel::Renderer::submit(m_shader, m_vertexArray);

		Hazel::Renderer::endScene();
	}

	void onEvent(Hazel::Event& e) override {
		HZ_INFO("ExampleLayer::event {}", e.toString());
	}

private:
	std::shared_ptr<Hazel::VertexArray> m_vertexArray;
	std::shared_ptr<Hazel::VertexBuffer> m_vertexBuffer;
	std::shared_ptr<Hazel::IndexBuffer> m_indexBuffer;
	std::shared_ptr<Hazel::ShaderProgram> m_shader;

	Hazel::OrthographicCamera m_camera;
	glm::vec3 m_cameraPosition;
	float m_cameraMoveSpeed = 5.0f;

	float m_cameraRotation = 0.0f;
	float m_cameraRotationSpeed = 180.0f;
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