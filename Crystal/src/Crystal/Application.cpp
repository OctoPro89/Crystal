#include "crystalpch.h"
#include "Application.h"

#include "Crystal/Events/ApplicationEvent.h"
#include "Crystal/Log.h"

namespace Crystal {
	Application::Application()
	{

	}

	Application::~Application()
	{
	}

	void Application::Run() 
	{
		WindowResizeEvent e(1280, 720);
		CRYSTAL_TRACE(e);

		while (true);
	}
}