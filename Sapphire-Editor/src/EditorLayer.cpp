#include "EditorLayer.h"
#include <imgui/imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Crystal/Scene/SceneSerializer.h>
#include <Crystal/Utils/PlatformUtils.h>
#include <Crystal/Math/Math.h>
#include <Crystal/Scripting/ScriptEngine.h>

namespace Crystal {
	EditorLayer* EditorLayer::s_Instance = nullptr;

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
		s_Instance = this;
	}

	void EditorLayer::OnAttach()
	{
		CRYSTAL_PROFILE_FUNCTION();

		m_IconPlay = Texture2D::Create("Resources/Icons/Play.png");
		m_IconPause = Texture2D::Create("Resources/Icons/Pause.png");
		m_IconStep = Texture2D::Create("Resources/Icons/Step.png");
		m_IconSimulate = Texture2D::Create("Resources/Icons/PhysicsPlay.png");
		m_IconStop = Texture2D::Create("Resources/Icons/Stop.png");

		FrameBufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(fbSpec);

		m_EditorScene = CreateRef<Scene>();
		m_ActiveScene = m_EditorScene;

		auto commandLineArgs = Application::Get().GetSpecification().CommandLineArgs;
		if (commandLineArgs.Count > 1)
		{
			auto projectFilePath = commandLineArgs[1];
			OpenProject(projectFilePath);
			m_ContentBrowserPanel = CreateScope<ContentBrowserPanel>();
		}
		else
		{
			/* TODO HERE USE LAUNCHER */
			CRYSTAL_CORE_ASSERT(false, "No Project");
		}

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
		Renderer2D::SetLineWidth(3.0f);
		Console.Log("Editor Setup");
	}

	void EditorLayer::OnDetach()
	{
		CRYSTAL_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		CRYSTAL_PROFILE_FUNCTION();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		if (FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}

		// Render
		Renderer2D::ResetStats();
		m_FrameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		//Clear entity id attachment to -1
		m_FrameBuffer->ClearAttachment(1, -1);

		switch (m_SceneState) {
			case SceneState::Edit:
			{
				m_EditorCamera.OnUpdate(ts);
				// Update Scene
				m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
				break;
			}
			case SceneState::Simulate:
			{
				m_EditorCamera.OnUpdate(ts);
				// Update Scene
				m_ActiveScene->OnUpdateSimulation(ts, m_EditorCamera);
				break;
			}
			case SceneState::Play:
			{
				// Update Scene
				m_ActiveScene->OnUpdateRuntime(ts);
				break;
			}
		}

		auto[mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)(viewportSize.x) && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_FrameBuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
		}

		OnOverlayRender();

		m_FrameBuffer->Unbind();
	}

	void EditorLayer::StopSceneForReload()
	{
		OnSceneStop();
		Console.Log("Stopping scene for script assembly reload.");
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

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling full screen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();
				}
				if (ImGui::MenuItem("Save", "Ctrl+S")) {
					SceneSerializer serializer(m_ActiveScene);
					serializer.Serialize("assets/scenes/Untitled.crystal");
				}
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();
				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
					NewScene();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Preferences")) preferencesWindow = !preferencesWindow;
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
			if (ImGui::BeginMenu("Scripting"))
			{
				if (ImGui::MenuItem("Reload C# Assemblies", ""))
				{
					if (m_SceneState != SceneState::Edit)
						Console.Warn("Could not reload script assemblies because scene is running! Waiting until scene is not running");
					else 
					{
						ScriptEngine::ReloadAssembly();
						Console.Log("Script Assemblies reloaded!");
					}
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel->OnImGuiRender();
		Console.OnImGuiRender();

		if (settingsWindow)
		{
			ImGui::Begin("Settings");
			ImGui::End();
		}

		if (performanceWindow) {
			ImGui::Begin("Performance & Stats");

			std::string name = "None";
			if (m_HoveredEntity)
				name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
			ImGui::Text("Hovered Entity: %s", name.c_str());
			auto stats = Renderer2D::GetStats();
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

			ImGui::End();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Viewport");
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y)
		{
			m_FrameBuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}
		uint64_t textureID = m_FrameBuffer->GetColorAttachmentRendererID(0);
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

		// Drag & Drop
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(path);
			}
			ImGui::EndDragDropTarget();
		}

		// Gizmo's
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			float windowWidth = ImGui::GetWindowWidth();
			float windowHeight = ImGui::GetWindowHeight();
			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);
			
			//Camera 
			if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate)
			{
				//Editor Camera
				const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
				glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

				// Entity transform
				auto& tc = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 transform = tc.GetTransform();

				// Snapping
				bool snap = Input::IsKeyPressed(Key::LeftControl);
				float snapValue = m_GizmoType == ImGuizmo::OPERATION::ROTATE ? 45.0f : 0.5f;

				float snapValues[3]{ snapValue, snapValue, snapValue };

				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

				if (ImGuizmo::IsUsing())
				{
					glm::vec3 translation, rotation, scale;
					Math::DecomposeTransform(transform, translation, rotation, scale);

					glm::vec3 deltaRotation = rotation - tc.Rotation;
					tc.Translation = translation;
					tc.Rotation += deltaRotation;
					tc.Scale = scale;
				}
			}
			else if (m_SceneState == SceneState::Play)
			{
				// Runtime Camera
				auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
				const auto& camera = cameraEntity.GetComponent<CameraComponent>();
				const glm::mat4& cameraProjection = camera.Camera.GetProjection();
				glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

				// Entity transform
				auto& tc = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 transform = tc.GetTransform();

				// Snapping
				bool snap = Input::IsKeyPressed(Key::LeftControl);
				float snapValue = m_GizmoType == ImGuizmo::OPERATION::ROTATE ? 45.0f : 0.5f;

				float snapValues[3]{ snapValue, snapValue, snapValue };

				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

				if (ImGuizmo::IsUsing())
				{
					glm::vec3 translation, rotation, scale;
					Math::DecomposeTransform(transform, translation, rotation, scale);

					glm::vec3 deltaRotation = rotation - tc.Rotation;
					tc.Translation = translation;
					tc.Rotation += deltaRotation;
					tc.Scale = scale;
				}
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		UI_Toolbar();
		if (preferencesWindow) { UI_Preferences(); }

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_EditorCamera.OnEvent(e);
		if (m_SceneState == SceneState::Edit)
		{
			m_EditorCamera.OnEvent(e);
		}
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(CRYSTAL_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(CRYSTAL_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		//Shortcuts
		if (e.IsRepeat())
			return false;

		bool control = (Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl));
		bool shift = (Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift));
		switch (e.GetKeyCode())
		{
		case Key::N:
		{
			if (control)
			{
				NewScene();
			}
			break;
		}
		case Key::O:
		{
			if (control)
			{
				OpenScene();
			}
			break;
		}
		case Key::S:
		{
			if (control)
			{
				if (shift)
					SaveSceneAs();
				else
					SaveScene();
			}
			break;
		}
		case Key::D:
		{
			if (control)
			{
				OnDuplicateEntity();
				break;
			}
		}
		//Gizmo's
		case Key::Q:
			m_GizmoType = -1;
			break;
		case Key::W:
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		case Key::E:
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		case Key::R:
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		case Key::T:
			m_GizmoType = ImGuizmo::OPERATION::ROTATE | ImGuizmo::OPERATION::SCALE | ImGuizmo::OPERATION::TRANSLATE;
			break;
		}
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == MouseButton::ButtonLeft)
		{
			if (m_ViewportHovered && !ImGuizmo::IsOver())
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		}
		return false;
	}

	void EditorLayer::OnOverlayRender()
	{
		if (m_SceneState == SceneState::Play)
		{
			Entity camera = m_ActiveScene->GetPrimaryCameraEntity();
			if (!camera)
				return;
			Renderer2D::BeginScene(camera.GetComponent<CameraComponent>().Camera, camera.GetComponent<TransformComponent>().GetTransform());
		}
		else
			Renderer2D::BeginScene(m_EditorCamera);
		if (CrntPreferences.ShowPhysicsColliders) {
			// Circle Collider
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, cc2d] = view.get<TransformComponent, CircleCollider2DComponent>(entity);

					glm::vec3 scale = tc.Scale * glm::vec3(cc2d.Radius * 2.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), tc.Translation)
						* glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
						* glm::translate(glm::mat4(1.0f), glm::vec3(cc2d.Offset, 0.001f))
						* glm::scale(glm::mat4(1.0f), scale);

					Renderer2D::DrawCircle(transform, CrntPreferences.SphereColliderColor, 0.05f);
				}
			}

			// Box Colliders
			{
				auto view = m_ActiveScene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(entity);
					
					glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), tc.Translation)
						* glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
						* glm::translate(glm::mat4(1.0f), glm::vec3(bc2d.Offset, 0.001f))
						* glm::scale(glm::mat4(1.0f), scale);

					Renderer2D::DrawRect(transform, CrntPreferences.QuadColliderColor);
				}
			}
		}
		// Draw outline
		if (Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity()) {
			const TransformComponent& transform = selectedEntity.GetComponent<TransformComponent>();
			Renderer2D::DrawRect(transform.GetTransform(), CrntPreferences.EntityOutlineColor);
		}

		Renderer2D::EndScene();
	}

	void EditorLayer::NewProject()
	{
		Project::New();
	}

	void EditorLayer::OpenProject(const std::filesystem::path& path)
	{
		if (Project::Load(path))
		{
			auto startScenePath = Project::GetAssetFileSystemPath(Project::GetActive()->GetConfig().StartScene);
			OpenScene(startScenePath);
			m_ContentBrowserPanel = CreateScope<ContentBrowserPanel>();
		}
	}

	void EditorLayer::SaveProject()
	{
		// Project::SaveActive();
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_EditorScene = m_ActiveScene;
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		Application::Get().GetWindow().SetWindowTitle(std::string("Crystal - Untitled*"));
		m_ScenePath = std::string();
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Crystal Scene(*.crystal)\0*.crystal\0");

		if (!filepath.empty())
			OpenScene(filepath);
	}

	void EditorLayer::OpenScene(const std::filesystem::path path)
	{
		if (m_SceneState != SceneState::Edit)
			OnSceneStop();
		if (path.extension().string() != ".crystal")
		{
			Console.Error("Could not load Scene - not a scene file");
			return;
		}

		Ref<Scene> newScene = CreateRef<Scene>();
		SceneSerializer serializer(newScene);
		if (serializer.Deserialize(path.string()))
		{
			m_EditorScene = newScene;
			m_SceneHierarchyPanel.SetContext(m_EditorScene);
			Application::Get().GetWindow().SetWindowTitle("Crystal - " + path.string());

			m_ActiveScene = m_EditorScene;

			m_ScenePath = path.string();
		}
	}

	void EditorLayer::SaveScene()
	{
		CRYSTAL_INFO("Saving Scene {0}", m_ScenePath);
		Console.Log("Saving Scene");
		if (!m_ScenePath.empty())
		{
			SerializeScene(m_ActiveScene, m_ScenePath);
		}
		else
			SaveSceneAs();
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Crystal Scene(*.crystal)\0*.crystal\0");
		if (!filepath.empty())
		{
			SerializeScene(m_ActiveScene, filepath);
			m_ScenePath = filepath;
		}
		Application::Get().GetWindow().SetWindowTitle("Crystal - " + filepath);
		Console.Log("Scene Saved Successfully!");
	}

	void EditorLayer::OnScenePause()
	{
		if (m_SceneState == SceneState::Edit)
			return;
		m_ActiveScene->SetPaused(true);
	}

	void EditorLayer::OnScenePlay()
	{
		if (m_SceneState == SceneState::Simulate)
			OnSceneStop();
		m_SceneState = SceneState::Play;
		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnRuntimeStart();
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnSceneStop()
	{
		m_SceneState = SceneState::Edit;
		m_ActiveScene->OnRuntimeStop();
		m_ActiveScene = m_EditorScene;
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnSimulationPlay()
	{
		if (m_SceneState == SceneState::Play)
			OnSceneStop();
		m_SceneState = SceneState::Simulate;
		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnSimulationStart();
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnSimulationStop()
	{
		m_SceneState = SceneState::Edit;
		m_ActiveScene->OnSimulationStop();
		m_ActiveScene = m_EditorScene;
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDuplicateEntity()
	{
		if (m_SceneState != SceneState::Edit)
			return;

		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity)
			m_EditorScene->DuplicateEntity(selectedEntity);
	}

	void EditorLayer::SerializeScene(Ref<Scene> scene, const std::string& path)
	{
		SceneSerializer serializer(scene);
		serializer.Serialize(path);
	}

	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0,1 });
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, { 0,0 });
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f,0.0f,0.0f,0.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.0f,0.0f,0.0f,0.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.5f,0.5f,0.5f,0.5f });
		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		bool toolbarEnabled = (bool)m_ActiveScene;

		ImVec4 tintColor = toolbarEnabled ? ImVec4{ 1, 1, 1, 1 } : ImVec4{ 1, 1, 1, 1 };
		
		float size = ImGui::GetWindowHeight() - 4;
		{
			Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate) ? m_IconPlay : m_IconStop;
			ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * .5f));
			if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), { 0,0 }, { 1,1 }, 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate)
					OnScenePlay();
				else if (m_SceneState == SceneState::Play)
					OnSceneStop();
			}
		}
		ImGui::SameLine();
		{
			Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play) ? m_IconSimulate : m_IconStop;
			if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), { 0,0 }, { 1,1 }, 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play)
					OnSimulationPlay();
				else if (m_SceneState == SceneState::Simulate)
					OnSceneStop();
			}
		}
		if (m_SceneState == SceneState::Play || m_SceneState == SceneState::Simulate) 
		{
			bool isPaused = m_ActiveScene->IsPaused();
			ImGui::SameLine();
			{
				Ref<Texture2D> icon = m_IconPause;
				if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), { 0,0 }, { 1,1 }, 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
				{
					m_ActiveScene->SetPaused(!isPaused);
				}
			}

			if (isPaused)
			{
				ImGui::SameLine();
				{
					Ref<Texture2D> icon = m_IconStep;
					if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), { 0,0 }, { 1,1 }, 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
					{
						m_ActiveScene->Step(1);
					}
				}
			}
		}
		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar(2);
		ImGui::End();
	}

	void EditorLayer::UI_Preferences()
	{
		ImGui::Begin("Preferences");
		if (ImGui::TreeNodeEx("Display"))
		{
			if (ImGui::Checkbox("Use V-Sync", &CrntPreferences.VSync))
			{
				Application::Get().GetWindow().SetVSync(CrntPreferences.VSync);
			}

			if (ImGui::TreeNodeEx("Rendering"))
			{
				if (ImGui::DragFloat("Line Thickness", &CrntPreferences.LineThickness, 0.01f, 0.0f, 10.0f))
				{
					Renderer2D::SetLineWidth(CrntPreferences.LineThickness);
				}

				if (ImGui::TreeNodeEx("Rendering API"))
				{
					std::string& renderingApiStr = "Current Rendering API: " + RendererAPI::CrystalAPITypeToString(RendererAPI::GetAPI());
					ImGui::Text(renderingApiStr.c_str());
					if (ImGui::MenuItem("OpenGL")) {}
					ImGui::TreePop();
				}

				ImGui::TreePop();

				if (ImGui::TreeNodeEx("Image Minification and Maxification"))
				{
					if (ImGui::MenuItem("Linear")) { Renderer2D::SetFilteringMode(false); }
					if (ImGui::MenuItem("Nearest")) { Renderer2D::SetFilteringMode(true); }

					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
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
		if (ImGui::TreeNodeEx("Themes"))
		{
			if (ImGui::MenuItem("Default"))
			{
				Application::Get().GetImGuiLayer()->SetDefaultThemeColors();
				CrntPreferences.Font::Default;
			}
			if (ImGui::MenuItem("Default Light"))
			{
				Application::Get().GetImGuiLayer()->SetDefaultLightColors();
				CrntPreferences.Font::DefaultLight;
			}
			if (ImGui::MenuItem("Default Dark"))
			{
				Application::Get().GetImGuiLayer()->SetDefaultDarkColors();
				CrntPreferences.Font::DefaultDark;
			}
			if (ImGui::MenuItem("Dark Blue"))
			{
				Application::Get().GetImGuiLayer()->SetDarkThemeColors();
				CrntPreferences.Font::Dark;
			}
			if (ImGui::MenuItem("Monochrome"))
			{
				Application::Get().GetImGuiLayer()->SetMonochromeTheme();
				CrntPreferences.Font::Monochrome;
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Components"))
		{
			if (ImGui::TreeNodeEx("Physics Colliders"))
			{
				ImGui::ColorEdit4("Sphere Collider Outline", glm::value_ptr(CrntPreferences.SphereColliderColor));
				ImGui::ColorEdit4("Box Collider Outline", glm::value_ptr(CrntPreferences.QuadColliderColor));
				ImGui::Checkbox("Show Physics Colliders", &CrntPreferences.ShowPhysicsColliders);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Entities"))
		{
			ImGui::ColorEdit4("Entity outline color", glm::value_ptr(CrntPreferences.EntityOutlineColor));
			ImGui::TreePop();
		}
		ImGui::End();
	}
}
