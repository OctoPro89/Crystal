#include <Crystal.h>

#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_opengl3.h"
#include "imgui/examples/imgui_impl_Glfw.h"

class ExampleLayer : public Crystal::Layer 
{
public:
	ExampleLayer()
		: Layer("Example") 
	{}

	void OnUpdate() override
	{
		if (Crystal::Input::IsKeyPressed(CRYSTAL_KEY_TAB)) {
			CRYSTAL_TRACE("Tab key pressed");
		}
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::ColorEdit4("", new float[4]);
		ImGui::End();
	}

	void OnEvent(Crystal::Event& event) override
	{
		if (event.GetEventType() == Crystal::EventType::KeyPressed)
		{
			Crystal::KeyPressedEvent& e = (Crystal::KeyPressedEvent&)event;
			CRYSTAL_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class Sandbox : public Crystal::Application
{
public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
	}
};

Crystal::Application* Crystal::CreateApplication() {
	return new Sandbox();
}