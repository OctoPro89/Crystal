#pragma once

#include <Crystal.h>

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
	Crystal::OrthographicCamera m_Camera;

	// Temp
	Crystal::Ref<Crystal::VertexArray> m_SquareVA;
	Crystal::Ref<Crystal::Shader> m_FlatColorShader;

	Crystal::Ref<Crystal::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};
