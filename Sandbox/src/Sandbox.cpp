#include <iostream>
#include <Orca.h>

#include "Orca/Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>


class ExampleLayer : public Orca::Layer {
public:
	ExampleLayer()
		:Layer("Example"), m_Camera(-3.2f, 3.2f, -1.8f, 1.8f), pos(0.0f), m_CameraSpeed(5.0f), m_Transform(0.0f)
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
		Orca::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Orca::RenderCommand::Clear();

		if (Orca::Input::IsKeyPressed(OA_KEY_UP)) pos.y += m_CameraSpeed * ts;
		else if (Orca::Input::IsKeyPressed(OA_KEY_DOWN)) pos.y -= m_CameraSpeed * ts;
		if (Orca::Input::IsKeyPressed(OA_KEY_RIGHT)) pos.x += m_CameraSpeed * ts;
		else if (Orca::Input::IsKeyPressed(OA_KEY_LEFT)) pos.x -= m_CameraSpeed * ts;

		if (Orca::Input::IsKeyPressed(OA_KEY_Q)) rot -= m_CameraSpeed * 3 * ts;
		else if (Orca::Input::IsKeyPressed(OA_KEY_E)) rot += m_CameraSpeed * 3  * ts;

		if (Orca::Input::IsKeyPressed(OA_KEY_W)) m_Transform.y += m_CameraSpeed * ts;
		else if (Orca::Input::IsKeyPressed(OA_KEY_S)) m_Transform.y -= m_CameraSpeed * ts;
		if (Orca::Input::IsKeyPressed(OA_KEY_D)) m_Transform.x += m_CameraSpeed * ts;
		else if (Orca::Input::IsKeyPressed(OA_KEY_A)) m_Transform.x -= m_CameraSpeed * ts;

		if(Orca::Input::IsKeyPressed(OA_KEY_R))
		{
			pos = { 0,0,0 };
			rot = 0;
			m_Transform = { 0,0,0 };
		}



		m_Camera.SetPosition(pos);
		m_Camera.SetRotation(rot);

		//std::dynamic_pointer_cast<Orca::OpenGLShader>(m_FlatShader)->UploadUniformFloat4("u_Color", m_Color);

		Orca::Renderer::BeginScene(m_Camera);
		m_Texture2D->Bind();
		Orca::Renderer::Submit(m_SquareVA, m_ShaderLib.Get("Texture"), glm::translate(glm::mat4(1.0f), m_Transform));
		Orca::Renderer::EndScene();
	}

	void OnEvent(Orca::Event& event) override {
	}


private:
	// Renderer vars
	Orca::ShaderLibrary m_ShaderLib;

	Orca::Ref<Orca::Texture2D> m_Texture2D;
	/*Orca::Ref<Orca::VertexBuffer> m_VertexBuffer;
	Orca::Ref<Orca::IndexBuffer> m_IndexBuffer;*/

	Orca::Ref<Orca::VertexArray> m_SquareVA;

	Orca::OrthographicCamera m_Camera;
	float m_CameraSpeed;

	glm::vec3 pos;
	float rot = 0.0f;

	glm::vec3 m_Transform;
	glm::vec4 m_Color;
};


class Sandbox : public Orca::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

Orca::Application* Orca::CreateApplication() {
	return new Sandbox();
}

