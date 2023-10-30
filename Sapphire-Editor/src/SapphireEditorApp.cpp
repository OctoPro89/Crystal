#include <Crystal.h>
#include <Crystal/Core/EntryPoint.h>
#include "imgui/imgui.h"
#include <glm/ext/matrix_transform.hpp>
#include "EditorLayer.h"

namespace Crystal {
	class SapphireEditor : public Crystal::Application
	{
	public:
		SapphireEditor(const ApplicationSpecification& spec)
			: Application(spec)
		{
			PushLayer(new EditorLayer());
		}

		~SapphireEditor()
		{
		}
	};

	Application* Crystal::CreateApplication(ApplicationCommandLineArgs args) {
		ApplicationSpecification spec;
		spec.Name = "Sapphire-Editor";
		spec.CommandLineArgs = args;

		return new SapphireEditor(spec);
	}
}
