#include <Crystal.h>
#include <Crystal/Core/EntryPoint.h>
#include "imgui/imgui.h"
#include <glm/ext/matrix_transform.hpp>
#include "Sandbox2D.h"

class Sandbox : public Crystal::Application
{
public:
	Sandbox(const Crystal::ApplicationSpecification& specification)
		: Application(specification)
	{
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{
	}
};

Crystal::Application* Crystal::CreateApplication(Crystal::ApplicationCommandLineArgs args) {
	Crystal::ApplicationSpecification spec;
	spec.Name = "Sandbox";
	spec.WorkingDirectory = "../Sapphire-Editor";
	spec.CommandLineArgs = args;
	return new Sandbox(spec);
}
