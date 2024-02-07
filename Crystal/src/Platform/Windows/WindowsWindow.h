#pragma once

#include "Crystal/Core/Window.h"
#include "Crystal/Renderer/GraphicsContext.h"
#include <GLFW/glfw3.h>


namespace Crystal {
	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		void SetWindowTitle(std::string& title) override;

		inline virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			
			bool VSync;

			EventCallbackFn EventCallback;
		};

		int windowPos[2] = { 0, 0 };
		int windowSize[2] = { 0, 0 };

		WindowData m_Data;
	};
}
