#include <iostream>
#include <Orca.h>

#include "Orca/Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

#include "Sandbox2D.h"


// Entry point goes last
#include <Orca/Core/EntryPoint.h>

class ExampleLayer : public Orca::Layer {
public:
	ExampleLayer()
		:Layer("Example"), m_Transform(0.0f), m_CameraController(16.0f/9.0f, true)
	{

		float vertices[] = {
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,	
			 1.0f, -1.0f, 0.0f,	1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f
		};



		m_SquareVA = Orca::VertexArray::Create();
		Orca::Ref<Orca::VertexBuffer> squareVB;
		squareVB.reset(Orca::VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			Orca::BufferLayout layout = {
				{Orca::ShaderDataType::Float3, "a_Position"},
				{Orca::ShaderDataType::Float2, "a_TexCoord"}
			};
			squareVB->SetLayout(layout);
		}


		unsigned int squareIndices[] = { 0, 1, 2, 2, 3, 0 };
		Orca::Ref<Orca::IndexBuffer> squareIB;
		squareIB.reset(Orca::IndexBuffer::Create(squareIndices, 6));

		m_SquareVA->AddVertexBuffer(squareVB);
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string TextureShaderPath = "assets/shaders/Texture.glsl";
		auto textureShader = m_ShaderLib.Load(TextureShaderPath);

		m_Texture2D = Orca::Texture2D::Create("assets/textures/orca.png");

		std::dynamic_pointer_cast<Orca::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Orca::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);

	}

	void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_Color));
		ImGui::End();
	}

	void OnUpdate(Orca::Timestep ts) override {

		m_CameraController.OnUpdate(ts);

		Orca::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Orca::RenderCommand::Clear();

		

		if(Orca::Input::IsKeyPressed(OA_KEY_R))
		{
			m_Transform = { 0,0,0 };
		}

		//std::dynamic_pointer_cast<Orca::OpenGLShader>(m_FlatShader)->UploadUniformFloat4("u_Color", m_Color);

		Orca::Renderer::BeginScene(m_CameraController.GetCamera());
		m_Texture2D->Bind();
		Orca::Renderer::Submit(m_SquareVA, m_ShaderLib.Get("Texture"), glm::translate(glm::mat4(1.0f), m_Transform));
		Orca::Renderer::EndScene();
	}

	void OnEvent(Orca::Event& e) override {
		m_CameraController.OnEvent(e);
	}


private:
	// Renderer vars
	Orca::ShaderLibrary m_ShaderLib;

	Orca::Ref<Orca::Texture2D> m_Texture2D;
	/*Orca::Ref<Orca::VertexBuffer> m_VertexBuffer;
	Orca::Ref<Orca::IndexBuffer> m_IndexBuffer;*/

	Orca::Ref<Orca::VertexArray> m_SquareVA;

	Orca::OrthographicCameraController m_CameraController;

	glm::vec3 m_Transform;
	glm::vec4 m_Color;
};


class Sandbox : public Orca::Application {
public:
	Sandbox() {
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox() {

	}
};

Orca::Application* Orca::CreateApplication() {
	return new Sandbox();
}

