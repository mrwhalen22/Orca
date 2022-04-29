#pragma once

#ifdef OA_PLATFORM_WINDOWS

// Allows client using Orca to define their own application and use this as their entry point
extern Orca::Application* Orca::CreateApplication();

int main(int argc, char** argv) {
	Orca::Log::Init();

	// Startup code
	OA_PROFILE_BEGIN_SESSION("Startup", "OrcaProfile-Startup.json");
	auto app = Orca::CreateApplication();
	OA_PROFILE_END_SESSION();

	// Runtime code
	OA_PROFILE_BEGIN_SESSION("Runtime", "OrcaProfile-Runtime.json");
	app->Run();
	OA_PROFILE_END_SESSION();

	// Shutdown section
	OA_PROFILE_BEGIN_SESSION("Shutdown", "OrcaProfile-Shutdown.json");
	delete app;
	OA_PROFILE_END_SESSION();
}

#endif
