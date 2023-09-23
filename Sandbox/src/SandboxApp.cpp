#include <Crystal.h>

class Sandbox : public Crystal::Application
{
public:
	Sandbox() 
	{

	}

	~Sandbox()
	{

	}
};

Crystal::Application* Crystal::CreateApplication() {
	return new Sandbox();
}