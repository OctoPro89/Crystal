#pragma once

#include "Crystal.h"
#include "Crystal/Renderer/ParticleSystem.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/ConsolePanel.h"

namespace Crystal {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		inline ConsolePanel* GetConsole() { return &Console; }
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

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path path);
		void SaveScene();
		void SaveSceneAs();

		void OnScenePlay();
		void OnSceneStop();

		void OnSimulationPlay();
		void OnSimulationStop();

		void OnDuplicateEntity();

		void SerializeScene(Ref<Scene> scene, const std::string& path);

		// Ui Panels
		void UI_Toolbar();
		void UI_Preferences();
	private:
		//temp
		Ref<VertexArray> m_VertexArray;
		Ref<Shader> m_Shader;
		Ref<FrameBuffer> m_FrameBuffer;

		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;
		Entity m_CameraEntity;
		Entity m_CameraEntity2;
		Entity m_SquareEntity;

		Entity m_HoveredEntity;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		float m_ParticlePos[2] = {0.0f,0.0f};

		bool useParticles = false;
		bool performanceWindow = false;
		bool settingsWindow = false;
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		bool preferencesWindow = false;
		bool m_CamSwitch = true;

		Ref<SubTexture2D> m_SubTex;
		Ref<Texture2D> m_Player;

		EditorCamera m_EditorCamera;

		std::string m_ScenePath;

		ParticleProps m_Particle;
		ParticleSystem m_ParticleSystem = ParticleSystem(100000);

		int m_GizmoType = 0;

		enum class SceneState
		{
			Edit = 0, Play = 1, Simulate = 2
		};

		SceneState m_SceneState = SceneState::Edit;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;
		ConsolePanel Console;

		// Editor Resources
		Ref<Texture2D> m_IconPlay, m_IconStop, m_IconSimulate;

		// Preferences
		struct Preferences
		{
			enum Font
			{
				Default, DefaultLight, DefaultDark, Dark, Monochrome
			};

			glm::vec4 SphereColliderColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			glm::vec4 QuadColliderColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			glm::vec4 EntityOutlineColor = glm::vec4(1, 0, 0, 1);
			bool ShowPhysicsColliders = false;
		};

		Preferences CrntPreferences;
	};
}
