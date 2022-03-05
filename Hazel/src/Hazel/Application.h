#pragma once
#include "Hazel/Core.h"
#include "Hazel/Window.h"
#include "Hazel/LayerStack.h"

#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"

#include "Hazel/ImGui/ImGuiLayer.h"



namespace Hazel {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		// Singleton
		static Application* s_Instance;

		// Window Ref
		std::unique_ptr<Window> m_Window;
		// Single ImGui Layer and Layer Stack
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;

		bool m_Running = true;
		

	};

	// To be defined in client application
	Application* CreateApplication();
}
