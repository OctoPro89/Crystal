#include "Sandbox2D.h"
#include "imgui/imgui.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{

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
	Crystal::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Crystal::Renderer2D::DrawQuad({ squareTransform[0], squareTransform[1] }, { 1.0f,1.0f }, { color[0], color[1], color[2], color[3] });
	Crystal::Renderer2D::DrawRotatedQuad({ squareTransform2[0], squareTransform2[1] }, rot, { 1.0f,1.0f }, { color[0], color[1], color[2], color[3] });
	Crystal::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Inspector");
	ImGui::Begin("Transforms");
	ImGui::Begin("Square");
	ImGui::Text("Square Transform");
	ImGui::DragFloat2("Position", squareTransform, 0.05f);
	ImGui::End();
	ImGui::Begin("Rotatable Square");
	ImGui::DragFloat2("Position", squareTransform2, 0.05f);
	ImGui::DragFloat("Rotation", &rot, 0.0005f);
	ImGui::End();
	ImGui::Begin("Materials");
	ImGui::Text("Pick a color for the squares");
	ImGui::ColorEdit3("Material Selection", color);
	ImGui::End();
	ImGui::End();
	ImGui::End();
}

void Sandbox2D::OnEvent(Crystal::Event& e)
{
	m_CameraController.OnEvent(e);
}
