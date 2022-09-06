#include "Sandbox2D.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D")
	, m_cameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::onAttach()
{
	m_squareVA = Hazel::VertexArray::create();

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
	m_squareVA->addVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	auto squareIB = Hazel::IndexBuffer::create();
	squareIB->allocate(squareIndices, sizeof(squareIndices), 6);
	m_squareVA->setIndexBuffer(squareIB);

	m_flatColorShader = Hazel::ShaderProgram::create();
	m_flatColorShader->addShaderFromSourceFile(Hazel::Shader::VERTEX, "assets/shaders/FlatColor.vert");
	m_flatColorShader->addShaderFromSourceFile(Hazel::Shader::FRAGMENT, "assets/shaders/FlatColor.frag");
	m_flatColorShader->link();
}

void Sandbox2D::onDetach()
{
}

void Sandbox2D::onUpdate(Hazel::Timestep ts)
{
	// Update
	m_cameraController.onUpdate(ts);

	// Render
	Hazel::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Hazel::RenderCommand::clear();

	Hazel::Renderer::beginScene(m_cameraController.getCamera());

	m_flatColorShader->bind();
	m_flatColorShader->setUniform("u_Color", m_squareColor);

	Hazel::Renderer::submit(m_flatColorShader, m_squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Hazel::Renderer::endScene();
}

void Sandbox2D::onImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));
	ImGui::End();
}

void Sandbox2D::onEvent(Hazel::Event& e)
{
	m_cameraController.onEvent(e);
}
