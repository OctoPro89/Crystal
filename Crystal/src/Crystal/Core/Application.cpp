#include "crystalpch.h"
#include "Application.h"
#include "Crystal/Renderer/Renderer.h"
#include "Crystal/Core/Timestep.h"
#include "Crystal/Scripting/ScriptEngine.h"
#include "Crystal/Audio/SoundSystem.h"
#include <filesystem>
#include <GLFW/glfw3.h>

namespace Crystal {

	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr; /* Create a static instance of the Application class and set it to a null pointer */

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		CRYSTAL_PROFILE_FUNCTION(); /* Profile the function */

		CRYSTAL_CORE_ASSERT(!s_Instance, "Application already exists!"); /* Assert that another instance of the app is not running */
		s_Instance = this; /* Set the static instance of an application to this one */

		// Set working directory
		if(!m_Specification.WorkingDirectory.empty()) /* See if the working directory is not changed/empty */
			std::filesystem::current_path(m_Specification.WorkingDirectory); /* If so change the working directory */

		m_Window = Window::Create(WindowProps(m_Specification.Name)); /* Create a window using the abstracted window class */
		m_Window->SetEventCallback(CRYSTAL_BIND_EVENT_FN(Application::OnEvent)); /* Set the Application Event Callbacks */

		Renderer::Init(); /* Initialize the Renderer */
		// ScriptEngine::Init(); /* Initialize the ScriptEngine */
		SoundSystem::Init(); /* Initialize the audio system */

		m_ImGuiLayer = new ImGuiLayer(); /* Create the ImGui Layer */
		PushOverlay(m_ImGuiLayer); /* Push the ImGui Layer as an Overlay */
	}

	Application::~Application()
	{
		CRYSTAL_PROFILE_FUNCTION(); /* Profile the function */
		// ScriptEngine::Shutdown(); Do Instead in OnDetach /* Shutdown the ScriptEngine */
		Renderer::Shutdown(); /* Shutdown the renderer */
		SoundSystem::Shutdown(); /* Shutdown the audio system */
	}

	void Application::PushLayer(Layer* layer)
	{
		CRYSTAL_PROFILE_FUNCTION(); /* Profile the function */
		m_LayerStack.PushLayer(layer); /* Push a layer */
		layer->OnAttach(); /* Call the layer's OnAttach Function */
	}

	void Application::PushOverlay(Layer* layer)
	{
		CRYSTAL_PROFILE_FUNCTION(); /* Profile the function */
		m_LayerStack.PushOverlay(layer); /* Push an overlay */
		layer->OnAttach(); /* Call the layer's OnAttach Function */
	}

	void Application::Close()
	{
		m_Running = false; /* Set m_Running to false */
	}

	void Application::OnEvent(Event& e)
	{
		CRYSTAL_PROFILE_FUNCTION(); /* Profile the function */
		EventDispatcher dispatcher(e); /* Create an instance of the EventDispatcher class with the event e */
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose)); /* Tell the dispatcher to dispatch/handle WindowCloseEvents with the OnWindowClose function */
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize)); /* Tell the dispatcher to dispatch/handle WindowResizeEvents with the OnWindowResize function */


		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) /* Iterate through the messages/events */
		{
			if (e.Handled) /* check if the event/message is handled and if it is, then break out of this loop */
				break;
			(*it)->OnEvent(e); /* otherwise call the OnEvent function to handle the event */
		}
	}

	void Application::SubmitToMainThread(const std::function<void()>& func)
	{
		std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex); /* Lock the scope */
		m_MainThreadQueue.emplace_back(func); /* emplace back the function */
	}

	void Application::Run()
	{
		CRYSTAL_PROFILE_FUNCTION(); /* Profile the function */

		while (m_Running) /* Main application loop */
		{
			CRYSTAL_PROFILE_SCOPE("Run Loop"); /* Profile this scope */
			float time = (float)glfwGetTime(); /* Get the time in seconds */
			Timestep timestep = time - m_LastFrameTime; /* Create an instance of the Timestep class to keep track of time and do the appropriate math to get the time */
			m_LastFrameTime = time; /* set the lastframetime to the time since last frame (FPS Counter) */

			ExecuteMainThreadQueue(); /* Execute the main thread queue */

			if (!m_Minimized) { /* See if the window is minimized */
				CRYSTAL_PROFILE_SCOPE("Layerstack OnUpdates"); /* Profile the scope */
				for (Layer* layer : m_LayerStack) /* Basically, update the engine if the app is not minimized, because otherwise it's just using resources for no reason */
					layer->OnUpdate(timestep); /* Update the layers with the time(Timestep class) */
			}

			m_ImGuiLayer->Begin(); /* Begin the ImGui Layer Stuff */
			{
				CRYSTAL_PROFILE_SCOPE("Layerstack OnImGuiRender"); /* Profile the scope */
				for (Layer* layer : m_LayerStack) /* For every imgui layer, update it. EXCEPT this time update imgui llayer even if minimized because you can undock windows that are not the main window, causing them to freeze */
					layer->OnImGuiRender(); /* Update the ImGui layers */
			}
			m_ImGuiLayer->End(); /* End the ImGui stuff */

			m_Window->OnUpdate(); /* Update the window */
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false; /* Set m_Running to false */
		return true; /* return true because the app successfully closed */
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		CRYSTAL_PROFILE_FUNCTION(); /* Profile the function */
		if (e.GetWidth() == 0 || e.GetHeight() == 0) /* See if the window is minimized or not (If its 0 and 0 its minimized) */
		{
			m_Minimized = true; /* set m_Minimized to true */
			return false; /* return false */
		}

		m_Minimized = false; /* Otherwise set minimized to false */

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight()); /* Tell the renderer to resize the window */

		return false; /* return false because the event is not blocked and its not handled */
	}
	void Application::ExecuteMainThreadQueue()
	{
		std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex); /* std vector of std functions */
		for (auto& func : m_MainThreadQueue)
			func();
		m_MainThreadQueue.clear();
	}
}
