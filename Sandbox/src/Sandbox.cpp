#include <iostream>
#include <Hazel.h>

#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer()
		:Layer("Example"), m_Camera(-3.2f, 3.2f, -1.8f, 1.8f), pos(0.0f), m_CameraSpeed(5.0f), m_Transform(0.0f)
	{
		m_VertexArray.reset(Hazel::VertexArray::Create());
		m_VertexArray->Bind();


		float vertices[] = {
			-1.0f, -1.0f, 0.0f,	0.9f, 0.7f, 0.1f, 1.0f,
			 1.0f, -1.0f, 0.0f,	0.9f, 0.1f, 0.1f, 1.0f,
			 1.0f,  1.0f,	0.0f,	0.9f, 0.1f, 0.9f, 1.0f,
			-1.0f,  1.0f,	0.0f,	0.1f, 0.1f, 0.9f, 1.0f,
		};

		m_VertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			Hazel::BufferLayout layout = {
				{Hazel::ShaderDataType::Float3, "a_Position"},
				{Hazel::ShaderDataType::Float4, "a_Color"}

			};
			m_VertexBuffer->SetLayout(layout);
		}


		unsigned int triangleIndices[] = { 0, 1, 2 };
		m_IndexBuffer.reset(Hazel::IndexBuffer::Create(triangleIndices, 3));

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
		m_VertexArray->Unbind();

		m_SquareVA.reset(Hazel::VertexArray::Create());
		std::shared_ptr<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			Hazel::BufferLayout layout = {
				{Hazel::ShaderDataType::Float3, "a_Position"},
				{Hazel::ShaderDataType::Float4, "a_Color"}

			};
			squareVB->SetLayout(layout);
		}


		unsigned int squareIndices[] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, 6));

		m_SquareVA->AddVertexBuffer(squareVB);
		m_SquareVA->SetIndexBuffer(squareIB);


		std::string vertexSrc = R"(
			#version 410 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;			

			uniform mat4 u_VPMatrix;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;
	
			void main() {
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_VPMatrix * u_Transform * vec4(a_Position, 1.0);
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


		m_Shader.reset(new Hazel::Shader(vertexSrc, fragmentSrc));


	}

	void OnImGuiRender() override {
	}

	void OnUpdate(Hazel::Timestep ts) override {
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();

		if (Hazel::Input::IsKeyPressed(HZ_KEY_UP)) pos.y += m_CameraSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN)) pos.y -= m_CameraSpeed * ts;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT)) pos.x += m_CameraSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT)) pos.x -= m_CameraSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_Q)) rot -= m_CameraSpeed * 3 * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_E)) rot += m_CameraSpeed * 3  * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_W)) m_Transform.y += m_CameraSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_S)) m_Transform.y -= m_CameraSpeed * ts;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_D)) m_Transform.x += m_CameraSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_A)) m_Transform.x -= m_CameraSpeed * ts;

		if(Hazel::Input::IsKeyPressed(HZ_KEY_R))
		{
			pos = { 0,0,0 };
			rot = 0;
			m_Transform = { 0,0,0 };
		}

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));

		m_Camera.SetPosition(pos);
		m_Camera.SetRotation(rot);

		Hazel::Renderer::BeginScene(m_Camera);
		for(int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 30; j++) {
				glm::vec3 pos(i * 0.5, j * 0.5, 0);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Hazel::Renderer::Submit(m_SquareVA, m_Shader, transform);
			}
		}
		Hazel::Renderer::EndScene();
	}

	void OnEvent(Hazel::Event& event) override {
	}


private:
	// Renderer vars
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;
	std::shared_ptr<Hazel::Shader> m_Shader;
	std::shared_ptr<Hazel::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Hazel::IndexBuffer> m_IndexBuffer;

	std::shared_ptr<Hazel::VertexArray> m_SquareVA;

	Hazel::OrthographicCamera m_Camera;
	float m_CameraSpeed;

	glm::vec3 pos;
	float rot = 0.0f;

	glm::vec3 m_Transform;

};


class Sandbox : public Hazel::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox();
}

