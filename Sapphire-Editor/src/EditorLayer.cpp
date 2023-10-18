#include "EditorLayer.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Crystal {
	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f)
	{
	}

	void EditorLayer::OnAttach()
	{
		CRYSTAL_PROFILE_FUNCTION();

		m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
		m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
		m_Particle.LifeTime = 0.5f;
		m_Particle.Velocity = { 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 3.0f, 1.0f };
		m_Particle.Position = { 0.0f, 0.0f };

		FrameBufferSpecification spec;
		spec.Width = 1280;
		spec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(spec);

		m_ActiveScene = CreateRef<Scene>();

		Entity square = m_ActiveScene->CreateEntity("Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f,1.0f,1.0f,1.0f });

		Entity square2 = m_ActiveScene->CreateEntity("Square2");
		square2.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f,1.0f,1.0f,1.0f });

		m_SquareEntity = square;

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
		m_CameraEntity.AddComponent<CameraComponent>().Primary = true;
		m_CameraEntity.GetComponent<CameraComponent>().FixedAspectRatio = false;

		m_CameraEntity2 = m_ActiveScene->CreateEntity("Camera B");
		m_CameraEntity2.AddComponent<CameraComponent>().Primary = false;

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
			}

			void OnDestroy()
			{
			}

			void OnUpdate(Timestep ts)
			{
				glm::vec3& transform = GetComponent<TransformComponent>().Translation;
				float speed = 5.0f;
				if (Input::IsKeyPressed(KeyCode::A))
					transform.x -= speed * ts;
				if (Input::IsKeyPressed(KeyCode::D))
					transform.x += speed * ts;
				if (Input::IsKeyPressed(KeyCode::W))
					transform.y += speed * ts;
				if (Input::IsKeyPressed(KeyCode::S))
					transform.y -= speed * ts;
			}
		};
		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_CameraEntity2.AddComponent<NativeScriptComponent>().Bind<CameraController>();
	}

	void EditorLayer::OnDetach()
	{
		CRYSTAL_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		CRYSTAL_PROFILE_FUNCTION();

		if (FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		// Update
		if(m_ViewportFocused)
			m_CameraController.OnUpdate(ts);

		// Render
		Renderer2D::ResetStats();
		m_FrameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		// Update Scene
		m_ActiveScene->OnUpdate(ts);
		Renderer2D::BeginScene(m_CameraController.GetCamera());

		if(useParticles)
			m_ParticleSystem.Emit(m_Particle);

		Renderer2D::EndScene();
		m_FrameBuffer->Unbind();
		m_Particle.Position = { m_ParticlePos[0], m_ParticlePos[1] };
		m_ParticleSystem.OnUpdate(ts);
		m_ParticleSystem.OnRender(m_CameraController.GetCamera());
	}

	void EditorLayer::OnImGuiRender()
	{
		CRYSTAL_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = 0;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (!m_Fullscreen)
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

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling full screen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View"))
			{
				// Disabling full screen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Settings")) settingsWindow = !settingsWindow;
				if (ImGui::MenuItem("Performance")) performanceWindow = !performanceWindow;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Preferences")) preferencesWindow = !preferencesWindow;
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}


		if (settingsWindow)
		{
			ImGui::Begin("Settings");
			ImGui::Checkbox("Use Particles", &useParticles);
			ImGui::DragFloat2("Particle System Position", m_ParticlePos, 0.01f);
			ImGui::End();
		}

		if (preferencesWindow)
		{
			ImGui::Begin("Preferences");
			if (ImGui::TreeNodeEx("Fonts"))
			{
				if (ImGui::MenuItem("Open Sans"))
				{
					auto& io = ImGui::GetIO(); io.FontDefault = io.Fonts->Fonts[5];
				}
				if (ImGui::MenuItem("Open Sans Bold"))
				{
					auto& io = ImGui::GetIO(); io.FontDefault = io.Fonts->Fonts[4];
				}
				if (ImGui::MenuItem("Roboto"))
				{
					auto& io = ImGui::GetIO(); io.FontDefault = io.Fonts->Fonts[3];
				}
				if (ImGui::MenuItem("Roboto Bold"))
				{
					auto& io = ImGui::GetIO(); io.FontDefault = io.Fonts->Fonts[2];
				}
				if (ImGui::MenuItem("Kalam"))
				{
					auto& io = ImGui::GetIO(); io.FontDefault = io.Fonts->Fonts[1];
				}
				if (ImGui::MenuItem("Kalam Bold"))
				{
					auto& io = ImGui::GetIO(); io.FontDefault = io.Fonts->Fonts[0];
				}
				ImGui::TreePop();
			}
			if (ImGui::TreeNodeEx("Themes & Colors"))
			{
				if (ImGui::MenuItem("Default"))
				{
					Application::Get().GetImGuiLayer()->SetDefaultThemeColors();
				}
				if (ImGui::MenuItem("Default Light"))
				{
					Application::Get().GetImGuiLayer()->SetDefaultLightColors();
				}				
				if (ImGui::MenuItem("Default Dark"))
				{
					Application::Get().GetImGuiLayer()->SetDefaultDarkColors();
				}
				if (ImGui::MenuItem("Dark Blue"))
				{
					Application::Get().GetImGuiLayer()->SetDarkThemeColors();
				}
				if (ImGui::MenuItem("Monochrome"))
				{
					Application::Get().GetImGuiLayer()->SetMonochromeTheme();
				}
				ImGui::TreePop();
			}
			ImGui::End();
		}

		m_SceneHierarchyPanel.OnImGuiRender();

		if (performanceWindow) {
			ImGui::Begin("Performance");

			auto stats = Renderer2D::GetStats();
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

			ImGui::End();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0,0});
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y)
		{
			m_FrameBuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
		}
		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}