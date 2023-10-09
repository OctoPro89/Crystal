#pragma once

#ifdef CRYSTAL_PLATFORM_WINDOWS

extern Crystal::Application* Crystal::CreateApplication();

int main(int argc, char** argv)
{
	Crystal::Log::Init();
	
	CRYSTAL_PROFILE_BEGIN_SESSION("Startup", "CrystalProfile-Startup.json");
	Crystal::Application* app = Crystal::CreateApplication();
	CRYSTAL_PROFILE_END_SESSION();
	CRYSTAL_PROFILE_BEGIN_SESSION("Runtime", "CrystalProfile-Runtime.json");
	app->Run();
	CRYSTAL_PROFILE_END_SESSION();
	CRYSTAL_PROFILE_BEGIN_SESSION("Runtime", "CrystalProfile-Shutdown.json");
	delete app;
	CRYSTAL_PROFILE_END_SESSION();
}

#endif