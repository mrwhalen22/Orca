#include "hzpch.h"

#include "Hazel/Application.h"
#include "Hazel/Input.h"
#include "Hazel/Log.h"

#include "Hazel/MouseButtonCodes.h"


#include "Renderer/Renderer.h"


namespace Hazel
{
	// Binds an Event type as an event function to be handled
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	// Define Application Singleton IE: Only one object instance of Application
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer;
		PushOverlay(m_ImGuiLayer);

		m_VertexArray.reset(VertexArray::Create());
		m_VertexArray->Bind();


		float vertices[] = {
			-0.75f, -0.75f, 0.0f, 0.9f, 0.7f, 0.1f, 1.0f,
			0.75f, -0.75f, 0.0f, 0.9f, 0.1f, 0.1f, 1.0f,
			0.75f, 0.75f, 0.0f, 0.9f, 0.1f, 0.9f, 1.0f,
			-0.75f, 0.75f, 0.0f, 0.1f, 0.1f, 0.9f, 1.0f,
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			BufferLayout layout = {
				{ShaderDataType::Float3, "a_Position"},
				{ShaderDataType::Float4, "a_Color"}

			};
			m_VertexBuffer->SetLayout(layout);
		}


		unsigned int triangleIndices[] = {0, 1, 2};
		m_IndexBuffer.reset(IndexBuffer::Create(triangleIndices, 3));

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
		m_VertexArray->Unbind();

		m_SquareVA.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			BufferLayout layout = {
				{ShaderDataType::Float3, "a_Position"},
				{ShaderDataType::Float4, "a_Color"}

			};
			squareVB->SetLayout(layout);
		}


		unsigned int squareIndices[] = {0, 1, 2, 2, 3, 0};
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, 6));

		m_SquareVA->AddVertexBuffer(squareVB);
		m_SquareVA->SetIndexBuffer(squareIB);


		std::string vertexSrc = R"(
			#version 410 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;			

			out vec3 v_Position;
			out vec4 v_Color;
	
			void main() {
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
			}	
			
		)";
		std::string fragmentSrc = R"(
			#version 410 core

			layout(location = 0) out vec4 color;			
			in vec3 v_Position;
			in vec4 v_Color;
			

			void main() {
				color = vec4(v_Position + 0.75, 1.0);
				color = v_Color;
			}	
			
		)";


		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
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
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

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
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();


			m_Shader->Bind();

			Renderer::BeginScene();
			Renderer::Submit(m_SquareVA);
			Renderer::EndScene();

			// Updates every layer
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			// Renders every layer
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();

			m_ImGuiLayer->End();


			m_Window->OnUpdate();
		}
	}
}
