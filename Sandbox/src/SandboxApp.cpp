#include <Crystal.h>

#include "imgui/imgui.h"

class ExampleLayer : public Crystal::Layer 
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(Crystal::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f, 1.0f
		};

		std::shared_ptr<Crystal::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Crystal::VertexBuffer::Create(vertices, sizeof(vertices)));
		Crystal::BufferLayout layout = {
			{ Crystal::ShaderDataType::Float3, "a_Position" },
			{ Crystal::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Crystal::IndexBuffer> indexBuffer;
		indexBuffer.reset(Crystal::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		float squarevertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		m_SquareVA.reset(Crystal::VertexArray::Create());
		std::shared_ptr<Crystal::VertexBuffer> squareVB;
		squareVB.reset((Crystal::VertexBuffer::Create(squarevertices, sizeof(squarevertices))));
		squareVB->SetLayout(
			{
			{ Crystal::ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Crystal::IndexBuffer> squareIB;
		squareIB.reset(Crystal::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location=0) in vec3 a_Position;
			layout(location=1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;
			
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

		m_Shader.reset(Crystal::Shader::Create(vertexSrc, fragmentSrc));

		std::string vertexSrc2 = R"(
			#version 330 core

			layout(location=0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc2 = R"(
			#version 330 core

			layout(location=0) out vec4 color;

			in vec3 v_Position;
			
			void main()
			{
				color = vec4(v_Position * .5 + .5,1.0);
			}
		)";

		m_Shader2.reset(Crystal::Shader::Create(vertexSrc2, fragmentSrc2));
	}

	void OnUpdate(Crystal::Timestep ts) override
	{
		CRYSTAL_TRACE("Delta Time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());
		if (Crystal::Input::IsKeyPressed(CRYSTAL_KEY_RIGHT))
		{
			m_CameraPosition.x -= m_CameraSpeed * ts;
		}
		else if (Crystal::Input::IsKeyPressed(CRYSTAL_KEY_LEFT))
		{
			m_CameraPosition.x += m_CameraSpeed * ts;
		}
		if (Crystal::Input::IsKeyPressed(CRYSTAL_KEY_UP))
		{
			m_CameraPosition.y -= m_CameraSpeed * ts;
		}
		else if (Crystal::Input::IsKeyPressed(CRYSTAL_KEY_DOWN))
		{
			m_CameraPosition.y += m_CameraSpeed * ts;
		}
		if (Crystal::Input::IsKeyPressed(CRYSTAL_KEY_A))
			m_CameraRotation -= m_CameraRotSpeed * ts;
		else if (Crystal::Input::IsKeyPressed(CRYSTAL_KEY_D))
			m_CameraRotation += m_CameraRotSpeed * ts;
		Crystal::RenderCommand::SetClearColor(glm::vec4(0, 0, 0, 1));
		Crystal::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Crystal::Renderer::BeginScene(m_Camera);
		Crystal::Renderer::Submit(m_Shader2, m_SquareVA);
		Crystal::Renderer::Submit(m_Shader, m_VertexArray);

		Crystal::Renderer::EndScene();
	}

	void OnEvent(Crystal::Event& event) override
	{
	}
private:
	std::shared_ptr<Crystal::Shader> m_Shader;
	std::shared_ptr<Crystal::VertexArray> m_VertexArray;

	std::shared_ptr<Crystal::Shader> m_Shader2;
	std::shared_ptr<Crystal::VertexArray> m_SquareVA;

	Crystal::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;
	float m_CameraRotSpeed = 180.0f;
	float m_CameraSpeed = 5.0f;
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