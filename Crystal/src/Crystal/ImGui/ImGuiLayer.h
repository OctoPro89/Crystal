#pragma once

#include "Crystal/Core/Layer.h"
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
		virtual void OnEvent(Event& e) override;

		void BlockEvents(bool block) { m_BlockEvents = block; }

		void Begin();
		void End();
		void SetDarkThemeColors();
		void SetMonochromeTheme();
		void SetDefaultThemeColors();
		void SetDefaultLightColors();
		void SetDefaultDarkColors();

		uint32_t GetActiveWidgetID() const;
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}
