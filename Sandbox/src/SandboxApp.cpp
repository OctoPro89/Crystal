#include <Crystal.h>

#include "imgui/imgui.h"
#include <glm/ext/matrix_transform.hpp>

class ExampleLayer : public Crystal::Layer 
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f, true)
	{
		m_VertexArray.reset(Crystal::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f, 1.0f
		};

		Crystal::Ref<Crystal::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Crystal::VertexBuffer::Create(vertices, sizeof(vertices)));
		Crystal::BufferLayout layout = {
			{ Crystal::ShaderDataType::Float3, "a_Position" },
			{ Crystal::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		uint32_t indices[3] = { 0, 1, 2 };
		Crystal::Ref<Crystal::IndexBuffer> indexBuffer;
		indexBuffer.reset(Crystal::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		float squarevertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		m_SquareVA.reset(Crystal::VertexArray::Create());
		Crystal::Ref<Crystal::VertexBuffer> squareVB;
		squareVB.reset((Crystal::VertexBuffer::Create(squarevertices, sizeof(squarevertices))));
		squareVB->SetLayout(
			{
			{ Crystal::ShaderDataType::Float3, "a_Position" },
			{ Crystal::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Crystal::Ref<Crystal::IndexBuffer> squareIB;
		squareIB.reset(Crystal::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		// Loading shaders
		m_Shader = m_ShaderLibrary.Load("assets/Shaders/RGBTriangle.glsl");
		m_Shader2 = m_ShaderLibrary.Load("assets/Shaders/FlatColor.glsl");
		Crystal::Ref<Crystal::Shader> textureShader = m_ShaderLibrary.Load("assets/Shaders/Texture.glsl");

		m_Texture = Crystal::Texture2D::Create("assets/textures/Checkerboard.png");
		m_CrystalLogo = Crystal::Texture2D::Create("assets/textures/crystal.png");

		std::dynamic_pointer_cast<Crystal::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Crystal::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Crystal::Timestep ts) override
	{
		m_CameraController.OnUpdate(ts);

		Crystal::RenderCommand::SetClearColor(glm::vec4(0, 0, 0, 1));
		Crystal::RenderCommand::Clear();

		Crystal::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Crystal::OpenGLShader>(m_Shader)->Bind();
		std::dynamic_pointer_cast<Crystal::OpenGLShader>(m_Shader2)->Bind();
		std::dynamic_pointer_cast<Crystal::OpenGLShader>(m_Shader2)->UploadUniformFloat4("u_Color", glm::vec4(color[0], color[1], color[2], 1.0f));

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Crystal::Renderer::Submit(m_Shader2, m_SquareVA, (transform * (glm::translate(glm::mat4(1.0f), glm::vec3(squareTransform[0], squareTransform[1], squareTransform[2])))));
			}
		}

		//Getting Shaders

		Crystal::Ref<Crystal::Shader> textureShader = m_ShaderLibrary.Get("Texture");

		//Triangle
		m_Shader->Bind();
		Crystal::Renderer::Submit(m_Shader, m_VertexArray);
		m_Texture->Bind();
		Crystal::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)) * glm::translate(glm::mat4(1.0f), glm::vec3(checkerBoardTransforms[0], checkerBoardTransforms[1], checkerBoardTransforms[2])));
		m_CrystalLogo->Bind();
		Crystal::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)) * glm::translate(glm::mat4(1.0f), glm::vec3(crystalLogoTransforms[0], crystalLogoTransforms[1], crystalLogoTransforms[2])));

		Crystal::Renderer::EndScene();
	}

	void OnEvent(Crystal::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Inspector");
		ImGui::Begin("Transforms");
		ImGui::DragFloat3("Square Transforms", squareTransform, 0.1f);
		ImGui::DragFloat3("Checkerboard Transforms", checkerBoardTransforms, 0.1f);
		ImGui::DragFloat3("Crystal Logo Transforms", crystalLogoTransforms, 0.1f);
		ImGui::End();
		ImGui::End();
		ImGui::Begin("Materials");
		ImGui::Text("Pick a color for the squares");
		ImGui::ColorEdit3("Material Selection", color);
		ImGui::End();
	}
private:
	// Shader Libraries
	Crystal::ShaderLibrary m_ShaderLibrary;

	// Vertex Arrays
	Crystal::Ref<Crystal::VertexArray> m_SquareVA;
	Crystal::Ref<Crystal::VertexArray> m_VertexArray;

	// Shaders
	Crystal::Ref<Crystal::Shader> m_Shader;
	Crystal::Ref<Crystal::Shader> m_Shader2;

	 // Textures
	Crystal::Ref<Crystal::Texture2D> m_Texture, m_CrystalLogo;

	// Camera Controller
	Crystal::OrthographicCameraController m_CameraController;

	// Square Color
	float color[4] = { 0.0f, 1.0f, 0.0f, 1.0f };

	float squareTransform[3] = { 0.0f,0.0f,0.0f };
	float checkerBoardTransforms[3] = { 0.0f,0.0f,0.0f };
	float crystalLogoTransforms[3] = {0.0f,0.0f,0.0f};
};

class Sandbox : public Crystal::Application
{
public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
	}
};

Crystal::Application* Crystal::CreateApplication() {
	return new Sandbox();
}