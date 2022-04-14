#include "oapch.h"

#include "Orca/Core/Application.h"
#include "Orca/Core/Input.h"
#include "Orca/Core/Log.h"
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
		dispatcher.Dispatch<WindowResizeEvent>(OA_BIND_EVENT_FN(Application::OnWindowResized));

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

	bool Application::OnWindowResized(WindowResizeEvent& e) {

		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
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
			
			// Don't update layers if the window is minimized
			if (!m_Minimized) {
				// Updates every layer
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);

			}


			// Renders every layer
			m_ImGuiLayer->Begin();
				
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();

			m_ImGuiLayer->End();

			if(!m_Minimized)
				m_Window->OnUpdate();
		}
	}
}
