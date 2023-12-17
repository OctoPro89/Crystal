#pragma once
extern Crystal::Application* Crystal::CreateApplication(ApplicationCommandLineArgs args); /* Extern the command line arguments */
int main(int argc, char** argv)
{
	Crystal::Log::Init(); /* Initialize the logging system */
	
	CRYSTAL_PROFILE_BEGIN_SESSION("Startup", "CrystalProfile-Startup.json"); /* Begin the startup session for profiling */
	Crystal::Application* app = Crystal::CreateApplication({argc, argv}); /* Create the application */
	CRYSTAL_PROFILE_END_SESSION(); /* End the startup session for profiling */

	CRYSTAL_PROFILE_BEGIN_SESSION("Runtime", "CrystalProfile-Runtime.json"); /* Begin the runtime session for profiling */
	app->Run(); /* Tell the app to run */
	CRYSTAL_PROFILE_END_SESSION(); /* End the runtime session for profiling */

	CRYSTAL_PROFILE_BEGIN_SESSION("Shutdown", "CrystalProfile-Shutdown.json"); /* Begin the shutdown session for profiling */
	delete app; /* delete the app from memory */
	CRYSTAL_PROFILE_END_SESSION(); /* End the shutdown session for profiling */
}
