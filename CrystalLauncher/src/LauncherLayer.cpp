#include "LauncherLayer.h"

#include <imgui/imgui.h>

namespace Crystal
{
	LauncherLayer::LauncherLayer()
	{

	}

	void LauncherLayer::OnAttach()
	{
		FrameBufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(fbSpec);
	}

	void LauncherLayer::OnDetach()
	{}

	void LauncherLayer::OnImGuiRender()
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = 0;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruDockspace)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become unlocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dock space/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSize = style.WindowMinSize.x;
		style.WindowMinSize.x = 370;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSize;


		if (ImGui::Begin("Crystal Launcher"))
		{
			ImGui::Text("Welcome");
			ImGui::Separator();
			
			if (ImGui::Button("New Project")) { shouldCreateProject = true; }
		}

		ImGui::End();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y)
		{
			m_FrameBuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}

		if (shouldCreateProject)
		{
			UI_CreateNewProject();
		}

		ImGui::End();
	}

	void LauncherLayer::OnUpdate(Timestep ts)
	{
		if (FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		Renderer2D::ResetStats();
		m_FrameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		m_FrameBuffer->Unbind();
	}

	void LauncherLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(CRYSTAL_BIND_EVENT_FN(LauncherLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(CRYSTAL_BIND_EVENT_FN(LauncherLayer::OnMouseButtonPressed));
	}

	bool LauncherLayer::OnKeyPressed(KeyPressedEvent & e)
	{
		return false;
	}

	bool LauncherLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		return false;
	}

	void LauncherLayer::OnOverlayRender()
	{

	}

	bool LauncherLayer::UI_CreateNewProject()
	{
		if (ImGui::Begin("New Project"))
		{
			ImGui::InputText("Project Name", crntProjectNameBuf, 100, 0);
		}

		ImGui::End();

		return true;
	}
}
