#pragma once
#include "Orca/Core.h"
#include "Orca/Window.h"
#include "Orca/LayerStack.h"

#include "Orca/Events/Event.h"
#include "Orca/Events/ApplicationEvent.h"

#include "Orca/ImGui/ImGuiLayer.h"

#include "Orca/Core/Timestep.h"



namespace Orca {
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
		float m_LastFrameTime = 0.0;
		bool m_Running = true;
		

	};

	// To be defined in client application
	Application* CreateApplication();
}
