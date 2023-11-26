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

int main(int argc, char** argv); /* Int main / entry point forward declaration */

namespace Crystal {
	struct ApplicationCommandLineArgs /* Command Line Args struct to hold it's data */
	{
		int Count = 0; /* the amount */
		char** Args = nullptr; /* the actuall string form args */

		const char* operator[](int index) const /* override operator */
		{
			CRYSTAL_CORE_ASSERT(index < Count, "index smaller than count in command line args"); /* Assert index < Count*/
			return Args[index]; /* return the Arguments at the index */
		}
	};

	struct ApplicationSpecification /* Application Specification */
	{
		std::string Name = "Crystal App"; /* Default app name (can be renamed) */
		std::string WorkingDirectory; /* String for working directory */
		ApplicationCommandLineArgs CommandLineArgs; /* Command line args */
	};

	class Application /* Application class */
	{
	public:
		Application(const ApplicationSpecification& specification); /* Constructor for application that takes in an ApplicationSpecification */
		virtual ~Application(); /* Virtual application destructor for inherited / base classes */

		void OnEvent(Event& e); /* OnEvent function that takes in a reference to an event */

		void PushLayer(Layer* layer); /* Push Layer function which takes in a pointer to a layer */
		void PushOverlay(Layer* layer); /* Push Overlay function which takes in a pointer to a layer */

		Window& GetWindow() { return *m_Window; } /* GetWindow function that returns a reference to a window */

		void Close(); /* Function to close */

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; } /* A function which returns a pointer to the ImGuiLayer class */

		static Application& Get() { return *s_Instance; } /* A static function which return a reference to the application class */

		const ApplicationSpecification& GetSpecification() const { return m_Specification; } /* A funciton that returns a const */
	private:
		void Run(); /* the main run function */
		bool OnWindowClose(WindowCloseEvent& e); /* An event callback function which returns a bool that takes in a WindowCloseEvent Reference */
		bool OnWindowResize(WindowResizeEvent& e); /* An event callback function which returns a bool that takes in a WindowResizeEvent Reference*/
	private:
		ApplicationSpecification m_Specification; /* The application specification data holder for this class */
		Scope<Window> m_Window; /* A Scoped pointer to a window */
		ImGuiLayer* m_ImGuiLayer; /* A normal pointer to an ImGuiLayer */
		bool m_Running = true; /* The boolean for seeing if the app is running */
		bool m_Minimized = false; /* The boolean for seeing if the app is minimized */
		LayerStack m_LayerStack; /* A layerstack object */
		float m_LastFrameTime = 0.0f; /* A floating point number since the last frame time */
	private:
		static Application* s_Instance; /* a static pointer to an application */
		friend int ::main(int argc, char** argv); /* a friend int to the main function */
	};

	// To be defined in CLIENT
	Application* CreateApplication(ApplicationCommandLineArgs args); /* An application function which returns a pointer to an application that takes in ApplicationCommandLine args that needs to be defined in the client */
}
