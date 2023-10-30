#pragma once

#include "Crystal/Core/Core.h"
#include "Crystal/Events/Event.h"
#include "Crystal/Core/Window.h"
#include "Crystal/Core/LayerStack.h"
#include "Crystal/Events/ApplicationEvent.h"
#include "Crystal/ImGui/ImGuiLayer.h"
#include "Crystal/Renderer/Shader.h"
#include "Crystal/Renderer/Buffer.h"
#include "Crystal/Renderer/VertexArray.h"
#include "Crystal/Renderer/OrthographicCamera.h"

int main(int argc, char** argv);

namespace Crystal {
	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			CRYSTAL_CORE_ASSERT(index < Count, "index smaller than count in command line args");
			return Args[index];
		}
	};

	struct ApplicationSpecification
	{
		std::string Name = "Crystal App";
		std::string WorkingDirectory;
		ApplicationCommandLineArgs CommandLineArgs;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& specification);
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		Window& GetWindow() { return *m_Window; }

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		static Application& Get() { return *s_Instance; }

		const ApplicationSpecification& GetSpecification() const { return m_Specification; }
	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		ApplicationSpecification m_Specification;
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// To be defined in CLIENT
	Application* CreateApplication(ApplicationCommandLineArgs args);
}
