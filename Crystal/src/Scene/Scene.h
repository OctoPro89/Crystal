#pragma once

#include <entt.hpp>

namespace Crystal
{
	class Scene
	{
	public:
		Scene();
		~Scene();
	private:
		entt::registry m_Registry;
	};
}