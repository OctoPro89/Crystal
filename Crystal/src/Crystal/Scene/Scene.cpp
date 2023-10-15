#include "crystalpch.h"
#include "Scene.h"
#include <glm/glm.hpp>

#include "Crystal/Renderer/Renderer2D.h"
#include "Components.h"

namespace Crystal
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}

	void Scene::OnUpdate()
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}

}