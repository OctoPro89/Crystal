#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <chrono>

template<typename Fn>
class Timer
{
public:
	Timer(const char* name, Fn&& func)
		: m_Name(name), m_Func(func), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_Stopped)
			Stop();
	}

	void Stop()
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		float duration = (end - start) * 0.001f;
		m_Func({ m_Name, duration });
	}
private:
	const char* m_Name;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
	Fn m_Func;
};

#define PROFILE_SCOPE(name) Timer timer##__Line__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	m_Texture = Crystal::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Crystal::Timestep ts)
{
	PROFILE_SCOPE("Sandbox::OnUpdate");

	//Update
	{
		PROFILE_SCOPE("CameraController Update");
		m_CameraController.OnUpdate(ts);
	}

	//Render
	{
		PROFILE_SCOPE("Renderer Prep");
		Crystal::RenderCommand::SetClearColor(glm::vec4(0, 0, 0, 1));
		Crystal::RenderCommand::Clear();
	}

	{
		PROFILE_SCOPE("Renderer Draw");
		Crystal::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Crystal::Renderer2D::DrawQuad({ squareTransform[0], squareTransform[1] }, { size3[0], size3[1] }, { color[0], color[1], color[2], color[3] });
		Crystal::Renderer2D::DrawRotatedQuad({ squareTransform2[0], squareTransform2[1] }, rot, { size2[0], size2[1] }, { color2[0], color2[1], color2[2], color2[3] });
		Crystal::Renderer2D::DrawRotatedQuad({ squareTransform3[0], squareTransform3[1], -0.1f }, rot2, { size[0], size[1] }, m_Texture);
		Crystal::Renderer2D::EndScene();
	}

	if (ok)
	{
		m_Texture = Crystal::Texture2D::Create(crntTex);
	}
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Performance");
	for (ProfileResult& result : m_ProfileResults)
	{
		char label[50];
		strcpy(label, "%.3fms ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);
	}
	m_ProfileResults.clear();
	ImGui::End();
	ImGui::Begin("Inspector");
	ImGui::Begin("Transforms");
	ImGui::Begin("Square");
	ImGui::Text("Square Transform");
	ImGui::DragFloat2("Position", squareTransform, 0.005f);
	ImGui::DragFloat2("Size", size3, 0.005f);
	ImGui::End();
	ImGui::Begin("Rotatable Square");
	ImGui::DragFloat2("Position", squareTransform2, 0.005f);
	ImGui::DragFloat("Rotation", &rot, 0.0005f);
	ImGui::DragFloat2("Size", size2, 0.005f);
	ImGui::End();
	ImGui::Begin("Textured Square");
	ImGui::DragFloat2("Position", squareTransform3, 0.005f);
	ImGui::DragFloat("Rotation", &rot2, 0.0005f);
	ImGui::DragFloat2("Size", size, 0.005f);
	ImGui::End();
	ImGui::Begin("Materials");
	ImGui::Text("Pick a color for the squares");
	ImGui::ColorEdit4("Square Material", color);
	ImGui::ColorEdit4("Rotatable Square Material", color2);
	ImGui::Begin("Textures");
	ImGui::InputText("Texture", crntTex, sizeof(crntTex));
	ok = ImGui::Button("Okay", { 50.0f, 30.0f });
	ImGui::End();
	ImGui::End();
	ImGui::End();
	ImGui::End();
}

void Sandbox2D::OnEvent(Crystal::Event& e)
{
	m_CameraController.OnEvent(e);
}