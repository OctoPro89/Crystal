#pragma once
#include "Crystal/Scene/Scene.h"
#include "Crystal/Scene/Entity.h"
#include "Crystal/Core/Log.h"
#include "Crystal/Core/Core.h"

namespace Crystal
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity);
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:;
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}