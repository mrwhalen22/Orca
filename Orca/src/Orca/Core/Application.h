#pragma once
#include "Orca/Core/Core.h"
#include "Orca/Core/Window.h"
#include "Orca/Core/LayerStack.h"
#include "Orca/Core/Timestep.h"

#include "Orca/Events/Event.h"
#include "Orca/Events/ApplicationEvent.h"

#include "Orca/ImGui/ImGuiLayer.h"




namespace Orca {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		// Singleton
		static Application* s_Instance;

		// Window Ref
		Scope<Window> m_Window;
		// Single ImGui Layer and Layer Stack
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0;
		bool m_Running = true;
		bool m_Minimized = false;
		

	};

	// To be defined in client application
	Application* CreateApplication();
}
