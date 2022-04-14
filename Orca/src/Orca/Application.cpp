#include "oapch.h"

#include "Orca/Application.h"
#include "Orca/Input.h"
#include "Orca/Log.h"
#include "Orca/Core/Core.h"
#include "Orca/Renderer/Renderer.h"

#include <GLFW/glfw3.h>



namespace Orca
{

	// Define Application Singleton IE: Only one object instance of Application
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{	
		s_Instance = this;

		m_Window = Scope<Window>(Window::Create());
		m_Window->SetEventCallback(OA_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);

		
		
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	// Sends Events to the event dispatcher to handle each event
	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(OA_BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.IsHandled())
				break;
		}
	}

	// Exit for Application
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	// Main loop for application
	// Updates and Renders Layers
	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;
			

			// Updates every layer
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			// Renders every layer
			m_ImGuiLayer->Begin();
				
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();

			m_ImGuiLayer->End();


			m_Window->OnUpdate();
		}
	}
}
