#pragma once
#include <Crystal.h>

using namespace Crystal;

class CharacterController : public ScriptableEntity
{
public:
	void OnUpdate(Timestep ts)
	{
		glm::vec3& transform = GetComponent<TransformComponent>().Translation;
		GetComponent<SpriteRendererComponent>().Texture = Texture2D::Create("assets/textures/Crystal.png");
		float speed = 5.0f;
		if (Input::IsKeyPressed(Key::A))
			transform.x -= speed * ts;
		if (Input::IsKeyPressed(Key::D))
			transform.x += speed * ts;
		if (Input::IsKeyPressed(Key::W))
			transform.y += speed * ts;
		if (Input::IsKeyPressed(Key::S))
			transform.y -= speed * ts;
	}
private:

};
