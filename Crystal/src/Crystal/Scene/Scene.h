#pragma once

#include <entt.hpp>

namespace Crystal
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		// TEMP
		entt::registry& Reg() { return m_Registry; }

		void OnUpdate();
	private:
		entt::registry m_Registry;
	};
}