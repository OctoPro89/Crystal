#pragma once

#include <Crystal.h>
#include <Crystal/Utils/Preferences.h>

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/ConsolePanel.h"

namespace Crystal {
	class EditorLayer : public Layer
	{
	private:
		enum class SceneState
		{
			Edit = 0, Play = 1, Simulate = 2
		};
	public:
		EditorLayer();
		static EditorLayer* GetEditorLayer() { return s_Instance; }
		inline void SetShouldReloadAssemblies(bool should) { shouldReloadAssemblies = should; }
		inline ConsolePanel* GetConsole() { return &Console; }
		inline SceneState GetSceneState() { return m_SceneState; }
		inline bool IsPlaying() { return m_SceneState == SceneState::Play; }

		void StopSceneForReload();
		
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;


	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		void OnOverlayRender();

		void NewProject();
		bool OpenProject();
		void OpenProject(const std::filesystem::path& path);
		void SaveProject();

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path path);
		void SaveScene();
		void SaveSceneAs();

		void OnScenePause();
		void OnScenePlay();
		void OnSceneStop();
		void OnSimulationPlay();
		void OnSimulationStop();
		void OnDuplicateEntity();

		void SerializeScene(Ref<Scene> scene, const std::string& path);

		// UI Panels
		void UI_Toolbar();
		void UI_Preferences();
	private:
		static EditorLayer* s_Instance;

		Ref<FrameBuffer> m_FrameBuffer;

		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;

		Entity m_HoveredEntity;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		bool performanceWindow = false;
		bool settingsWindow = false;
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		bool preferencesWindow = false;
		bool shouldReloadAssemblies = false;

		EditorCamera m_EditorCamera;

		std::string m_ScenePath;

		int m_GizmoType = 0;

		SceneState m_SceneState = SceneState::Edit;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		Scope<ContentBrowserPanel> m_ContentBrowserPanel;
		ConsolePanel Console;

		// Editor Resources
		Ref<Texture2D> m_IconPlay, m_IconPause, m_IconStop, m_IconSimulate, m_IconStep;

		Ref<Preferences> CrntPreferences;
	};
}
