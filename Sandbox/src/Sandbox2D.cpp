#include "Sandbox2D.h"
#include "imgui/imgui.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	m_VertexArray = (Crystal::VertexArray::Create());

	float squarevertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	m_VertexArray = (Crystal::VertexArray::Create());
	Crystal::Ref<Crystal::VertexBuffer> squareVB;
	squareVB.reset((Crystal::VertexBuffer::Create(squarevertices, sizeof(squarevertices))));
	squareVB->SetLayout(
	{
	{ Crystal::ShaderDataType::Float3, "a_Position" }
	});
	m_VertexArray->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Crystal::Ref<Crystal::IndexBuffer> squareIB;
	squareIB.reset(Crystal::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_VertexArray->SetIndexBuffer(squareIB);

	m_Shader = Crystal::Shader::Create("assets/Shaders/FlatColor.glsl");

	m_Texture = Crystal::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Crystal::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	//Render
	Crystal::RenderCommand::SetClearColor(glm::vec4(0, 0, 0, 1));
	Crystal::RenderCommand::Clear();
	Crystal::Renderer::BeginScene(m_CameraController.GetCamera());
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	std::dynamic_pointer_cast<Crystal::OpenGLShader>(m_Shader)->Bind();
	std::dynamic_pointer_cast<Crystal::OpenGLShader>(m_Shader)->UploadUniformFloat4("u_Color", glm::vec4(color[0], color[1], color[2], color[3]));
	Crystal::Renderer::Submit(m_Shader, m_VertexArray);
	Crystal::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Inspector");
	ImGui::Begin("Materials");
	ImGui::Text("Pick a color for the squares");
	ImGui::ColorEdit3("Material Selection", color);
	ImGui::End();
	ImGui::End();
}

void Sandbox2D::OnEvent(Crystal::Event& e)
{
	m_CameraController.OnEvent(e);
}
