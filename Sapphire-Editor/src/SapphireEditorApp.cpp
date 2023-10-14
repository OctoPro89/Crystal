#include <Crystal.h>
#include <Crystal/Core/EntryPoint.h>
#include "imgui/imgui.h"
#include <glm/ext/matrix_transform.hpp>
#include "EditorLayer.h"

namespace Crystal {
	class SapphireEditor : public Crystal::Application
	{
	public:
		SapphireEditor()
			: Application("Crystal Editor")
		{
			PushLayer(new EditorLayer());
		}

		~SapphireEditor()
		{
		}
	};

	Application* Crystal::CreateApplication() {
		return new SapphireEditor();
	}
}