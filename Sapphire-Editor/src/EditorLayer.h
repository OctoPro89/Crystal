#pragma once

#include "Crystal.h"
#include "Crystal/Renderer/ParticleSystem.h"
#include <imgui/imgui.h>
#include <glm/ext/matrix_transform.hpp>
#include "Panels/SceneHierarchyPanel.h"

namespace Crystal {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		void NewScene();
		void OpenScene();
		void SaveSceneAs();
	private:
		OrthographicCameraController m_CameraController;

		//temp
		Ref<VertexArray> m_VertexArray;
		Ref<Shader> m_Shader;
		Ref<Scene> m_ActiveScene;
		Ref<FrameBuffer> m_FrameBuffer;
		Entity m_CameraEntity;
		Entity m_CameraEntity2;
		Entity m_SquareEntity;

		glm::vec2 m_ViewportSize = { 0.0f,0.0f };

		float m_ParticlePos[2] = {0.0f,0.0f};

		bool useParticles = false;
		bool performanceWindow = false;
		bool settingsWindow = false;
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		bool preferencesWindow = false;
		bool m_CamSwitch = true;

		std::string fp = "";

		ParticleProps m_Particle;
		ParticleSystem m_ParticleSystem = ParticleSystem(100000);

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}