#pragma once

#ifdef OA_PLATFORM_WINDOWS

// Allows client using Orca to define their own application and use this as their entry point
extern Orca::Application* Orca::CreateApplication();

int main(int argc, char** argv) {

	Orca::Log::Init();
	OA_CORE_TRACE("Orca Application Initialized...");
	OA_INFO("Welcome to your app!");

	auto app = Orca::CreateApplication();
	app->Run();
	delete app;
}

#endif
