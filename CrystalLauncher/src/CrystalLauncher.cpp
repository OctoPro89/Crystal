#include <Crystal.h>
#include <Crystal/Core/EntryPoint.h>

#include "LauncherLayer.h"

namespace Crystal {
	class CrystalLauncher : public Crystal::Application
	{
	public:
		CrystalLauncher(const ApplicationSpecification& spec)
			: Application(spec)
		{
			PushLayer(new LauncherLayer());
		}

		~CrystalLauncher()
		{
		}
	};

	Application* Crystal::CreateApplication(ApplicationCommandLineArgs args) {
		ApplicationSpecification spec;
		spec.Name = "Crystal Launcher";
		spec.CommandLineArgs = args;

		return new CrystalLauncher(spec);
	}
}
