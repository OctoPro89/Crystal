#include "crystalpch.h"
#include "Entity.h"

namespace Crystal
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{

	}
}