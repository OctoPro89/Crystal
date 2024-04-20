#pragma once

#include <Crystal.h>
#include <glm/glm/vec2.hpp>

namespace Crystal
{
	class LauncherLayer : public Layer
	{
	public:
		LauncherLayer();

		virtual ~LauncherLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		void OnOverlayRender();

		bool UI_CreateNewProject();

	private:
		Ref<FrameBuffer> m_FrameBuffer;

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		bool shouldCreateProject = false;

		char crntProjectNameBuf[100];
	};
}
