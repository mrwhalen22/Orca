#pragma once

#ifdef HZ_PLATFORM_WINDOWS

// Allows client using Hazel to define their own application and use this as their entry point
extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv) {

	Hazel::Log::Init();
	HZ_CORE_TRACE("Hazel Application Initialized...");
	HZ_INFO("Welcome to your app!");

	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif
