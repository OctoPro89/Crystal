#include <Crystal.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}

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
		PushOverlay(new Crystal::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Crystal::Application* Crystal::CreateApplication() {
	return new Sandbox();
}