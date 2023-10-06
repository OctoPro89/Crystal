#include <Crystal.h>

#include "imgui/imgui.h"
#include <glm/ext/matrix_transform.hpp>
#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Crystal::Layer 
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_SquarePos(0.0f)
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

		std::string vertexSrc = R"(
			#version 330 core

			layout(location=0) in vec3 a_Position;
			layout(location=1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;
			
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location=0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;
			
			void main()
			{
				color = vec4(v_Position * .5 + .5,1.0);
				color = v_Color;
			}
		)";

		m_Shader = (Crystal::Shader::Create("RGBTriangle", vertexSrc, fragmentSrc));

		std::string flatShaderVertexSrc = R"(
			#version 330 core

			layout(location=0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string flatShaderFragmentSrc = R"(
			#version 330 core

			layout(location=0) out vec4 color;

			in vec3 v_Position;

			uniform vec4 u_Color;
			
			void main()
			{
				color = u_Color;
			}
		)";

		m_Shader2 = (Crystal::Shader::Create("FlatColor", flatShaderVertexSrc, flatShaderFragmentSrc));

		Crystal::Ref<Crystal::Shader> textureShader = m_ShaderLibrary.Load("assets/Shaders/Texture.glsl");

		m_Texture = Crystal::Texture2D::Create("assets/textures/Checkerboard.png");
		m_CrystalLogo = Crystal::Texture2D::Create("assets/textures/crystal.png");

		std::dynamic_pointer_cast<Crystal::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Crystal::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Crystal::Timestep ts) override
	{
		CRYSTAL_TRACE("Delta Time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());
		if (Crystal::Input::IsKeyPressed(CRYSTAL_KEY_RIGHT))
		{
			m_CameraPosition.x += m_CameraSpeed * ts;
		}
		else if (Crystal::Input::IsKeyPressed(CRYSTAL_KEY_LEFT))
		{
			m_CameraPosition.x -= m_CameraSpeed * ts;
		}
		if (Crystal::Input::IsKeyPressed(CRYSTAL_KEY_UP))
		{
			m_CameraPosition.y += m_CameraSpeed * ts;
		}
		else if (Crystal::Input::IsKeyPressed(CRYSTAL_KEY_DOWN))
		{
			m_CameraPosition.y -= m_CameraSpeed * ts;
		}
		if (Crystal::Input::IsKeyPressed(CRYSTAL_KEY_A))
			m_CameraRotation -= m_CameraRotSpeed * ts;
		else if (Crystal::Input::IsKeyPressed(CRYSTAL_KEY_D))
			m_CameraRotation += m_CameraRotSpeed * ts;
		if (Crystal::Input::IsKeyPressed(CRYSTAL_KEY_L))
		{
			m_SquarePos.x += m_CameraSpeed * ts;
		}
		else if (Crystal::Input::IsKeyPressed(CRYSTAL_KEY_J))
		{
			m_SquarePos.x -= m_CameraSpeed * ts;
		}
		if (Crystal::Input::IsKeyPressed(CRYSTAL_KEY_I))
		{
			m_SquarePos.y += m_CameraSpeed * ts;
		}
		else if (Crystal::Input::IsKeyPressed(CRYSTAL_KEY_K))
		{
			m_SquarePos.y -= m_CameraSpeed * ts;
		}
		Crystal::RenderCommand::SetClearColor(glm::vec4(0, 0, 0, 1));
		Crystal::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Crystal::Renderer::BeginScene(m_Camera);

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
				Crystal::Renderer::Submit(m_Shader2, m_SquareVA, transform * glm::translate(glm::mat4(1.0f), m_SquarePos));
			}
		}

		Crystal::Ref<Crystal::Shader> textureShader = m_ShaderLibrary.Get("Texture");

		//Triangle
		//Crystal::Renderer::Submit(m_Shader, m_VertexArray);
		m_Texture->Bind();
		Crystal::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_CrystalLogo->Bind();
		Crystal::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Crystal::Renderer::EndScene();
	}

	void OnEvent(Crystal::Event& event) override
	{
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Materials");
		ImGui::Text("Pick a color for the squares");
		ImGui::ColorEdit3("Material Selection", color);
		ImGui::End();
	}
private:
	Crystal::ShaderLibrary m_ShaderLibrary;
	Crystal::Ref<Crystal::Shader> m_Shader;
	Crystal::Ref<Crystal::VertexArray> m_VertexArray;

	Crystal::Ref<Crystal::Shader> m_Shader2;
	Crystal::Ref<Crystal::VertexArray> m_SquareVA;

	Crystal::Ref<Crystal::Texture2D> m_Texture, m_CrystalLogo;

	Crystal::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;
	float m_CameraRotSpeed = 180.0f;
	float m_CameraSpeed = 5.0f;

	glm::vec3 m_SquarePos;

	float color[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
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