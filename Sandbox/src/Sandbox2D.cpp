#include "Sandbox2D.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	CRYSTAL_PROFILE_FUNCTION();

	m_Texture = Crystal::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	CRYSTAL_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Crystal::Timestep ts)
{
	CRYSTAL_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	{
		CRYSTAL_PROFILE_SCOPE("Renderer Prep");
		Crystal::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Crystal::RenderCommand::Clear();
	}

	{

		static float rotation = 0.0f;
		rotation += ts * 20.0f;
		CRYSTAL_PROFILE_SCOPE("Renderer Draw");
		Crystal::Renderer2D::BeginScene(m_CameraController.GetCamera());
		// Crystal::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
		Crystal::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Crystal::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Crystal::Renderer2D::DrawQuad(glm::vec3(0.0f, 0.0f, -0.1), {10.0f, 10.0f}, m_Texture, 10.0f);
		Crystal::Renderer2D::DrawRotatedQuad({ -2.0f, -0.5f, 0.0f }, { 1.0f, 1.0f }, rotation, m_Texture, 10.0f);
		Crystal::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	CRYSTAL_PROFILE_FUNCTION();
}

void Sandbox2D::OnEvent(Crystal::Event& e)
{
	m_CameraController.OnEvent(e);
}