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
			-1.0f, -1.0f, 0.0f,	
			 1.0f, -1.0f, 0.0f,	
			 1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f
		};



		m_SquareVA.reset(Orca::VertexArray::Create());
		Orca::Ref<Orca::VertexBuffer> squareVB;
		squareVB.reset(Orca::VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			Orca::BufferLayout layout = {
				{Orca::ShaderDataType::Float3, "a_Position"},
			};
			squareVB->SetLayout(layout);
		}


		unsigned int squareIndices[] = { 0, 1, 2, 2, 3, 0 };
		Orca::Ref<Orca::IndexBuffer> squareIB;
		squareIB.reset(Orca::IndexBuffer::Create(squareIndices, 6));

		m_SquareVA->AddVertexBuffer(squareVB);
		m_SquareVA->SetIndexBuffer(squareIB);


		std::string vertexSrc = R"(
			#version 410 core

			layout(location = 0) in vec3 a_Position;			

			uniform mat4 u_VPMatrix;
			uniform mat4 u_Transform;

	
			void main() {
				gl_Position = u_VPMatrix * u_Transform * vec4(a_Position, 1.0);
			}	
			
		)";
		std::string fragmentSrc = R"(
			#version 410 core

			layout(location = 0) out vec4 color;			

			uniform vec4 u_Color;

			void main() {
				color = u_Color;
			}	
			
		)";


		m_FlatShader.reset(Orca::Shader::Create(vertexSrc, fragmentSrc));


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

		std::dynamic_pointer_cast<Orca::OpenGLShader>(m_FlatShader)->Bind();
		std::dynamic_pointer_cast<Orca::OpenGLShader>(m_FlatShader)->UploadUniformFloat4("u_Color", m_Color);

		Orca::Renderer::BeginScene(m_Camera);
		Orca::Renderer::Submit(m_SquareVA, m_FlatShader, glm::translate(glm::mat4(1.0f), m_Transform));
		Orca::Renderer::EndScene();
	}

	void OnEvent(Orca::Event& event) override {
	}


private:
	// Renderer vars
	Orca::Ref<Orca::Shader> m_FlatShader;
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

