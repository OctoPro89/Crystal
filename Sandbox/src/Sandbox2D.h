#pragma once

#include "Crystal.h"
#include "Crystal/Renderer/ParticleSystem.h"
#include <glm/ext/matrix_transform.hpp>

class Sandbox2D : public Crystal::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

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

	ParticleProps m_Particle;
	ParticleSystem m_ParticleSystem = ParticleSystem(100000);
};