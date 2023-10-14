#pragma once

#include "Crystal.h"
#include "Crystal/Renderer/ParticleSystem.h"
#include <glm/ext/matrix_transform.hpp>

namespace Crystal {

	class EditorLayer : public Crystal::Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Crystal::Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Crystal::Event& e) override;
	private:
		Crystal::OrthographicCameraController m_CameraController;

		//temp
		Crystal::Ref<Crystal::VertexArray> m_VertexArray;
		Crystal::Ref<Crystal::Shader> m_Shader;
		Crystal::Ref<Crystal::Texture2D> m_Texture;
		Crystal::Ref<Crystal::Texture2D> m_SpriteSheet;
		Crystal::Ref<Crystal::SubTexture2D> m_SpriteGrass;

		Crystal::Ref<Crystal::FrameBuffer> m_FrameBuffer;

		glm::vec2 m_ViewportSize = { 0.0f,0.0f };

		float m_ParticlePos[2] = {0.0f,0.0f};

		bool useParticles = false;

		ParticleProps m_Particle;
		ParticleSystem m_ParticleSystem = ParticleSystem(100000);
	};
}