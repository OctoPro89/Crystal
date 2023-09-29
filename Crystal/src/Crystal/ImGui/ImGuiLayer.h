#pragma once

#include "Crystal/Layer.h"

#include "Crystal/Events/MouseEvent.h"
#include "Crystal/Events/KeyEvent.h"
#include "Crystal/Events/ApplicationEvent.h"

namespace Crystal {
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}