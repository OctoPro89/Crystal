#include "crystalpch.h"
#include "Application.h"

#include "Crystal/Events/ApplicationEvent.h"
#include "Crystal/Log.h"

namespace Crystal {
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
	}

	void Application::Run() 
	{
		while (true);
	}
}