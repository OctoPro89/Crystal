#include <Crystal.h>
#include <Crystal/Core/EntryPoint.h>
#include "imgui/imgui.h"
#include <glm/ext/matrix_transform.hpp>
#include "EditorLayer.h"

namespace Crystal {
	class CrystalEditor : public Crystal::Application
	{
	public:
		CrystalEditor()
		{
			//PushLayer(new ExampleLayer());
			PushLayer(new EditorLayer());
		}

		~CrystalEditor()
		{
		}
	};

	Application* Crystal::CreateApplication() {
		return new CrystalEditor();
	}
}