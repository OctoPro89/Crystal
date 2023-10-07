#pragma once

#include "Crystal.h"
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

	float color[4] = { 0.8f,0.2f,0.3f,1.0f };

	float rot = 0.0f;

	float squareTransform[2] = { 0.0f,0.0f };
	float squareTransform2[2] = { 1.5f,0.0f };
};

