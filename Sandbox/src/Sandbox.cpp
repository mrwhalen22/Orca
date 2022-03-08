#include <iostream>
#include <Orca.h>

#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Orca::Layer {
public:
	ExampleLayer()
		:Layer("Example"), m_Camera(-3.2f, 3.2f, -1.8f, 1.8f), pos(0.0f), m_CameraSpeed(5.0f), m_Transform(0.0f)
	{
		m_VertexArray.reset(Orca::VertexArray::Create());
		m_VertexArray->Bind();


		float vertices[] = {
			-1.0f, -1.0f, 0.0f,	0.9f, 0.7f, 0.1f, 1.0f,
			 1.0f, -1.0f, 0.0f,	0.9f, 0.1f, 0.1f, 1.0f,
			 1.0f,  1.0f,	0.0f,	0.9f, 0.1f, 0.9f, 1.0f,
			-1.0f,  1.0f,	0.0f,	0.1f, 0.1f, 0.9f, 1.0f,
		};

		m_VertexBuffer.reset(Orca::VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			Orca::BufferLayout layout = {
				{Orca::ShaderDataType::Float3, "a_Position"},
				{Orca::ShaderDataType::Float4, "a_Color"}

			};
			m_VertexBuffer->SetLayout(layout);
		}


		unsigned int triangleIndices[] = { 0, 1, 2 };
		m_IndexBuffer.reset(Orca::IndexBuffer::Create(triangleIndices, 3));

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
		m_VertexArray->Unbind();

		m_SquareVA.reset(Orca::VertexArray::Create());
		std::shared_ptr<Orca::VertexBuffer> squareVB;
		squareVB.reset(Orca::VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			Orca::BufferLayout layout = {
				{Orca::ShaderDataType::Float3, "a_Position"},
				{Orca::ShaderDataType::Float4, "a_Color"}

			};
			squareVB->SetLayout(layout);
		}


		unsigned int squareIndices[] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Orca::IndexBuffer> squareIB;
		squareIB.reset(Orca::IndexBuffer::Create(squareIndices, 6));

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


		m_Shader.reset(new Orca::Shader(vertexSrc, fragmentSrc));


	}

	void OnImGuiRender() override {
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

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));

		m_Camera.SetPosition(pos);
		m_Camera.SetRotation(rot);

		Orca::Renderer::BeginScene(m_Camera);
		for(int i = 0; i < 30; i++)
		{
			for (int j = 0; j < 30; j++) {
				glm::vec3 pos(i * 0.5, j * 0.5, 0);
				pos += m_Transform;
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Orca::Renderer::Submit(m_SquareVA, m_Shader, transform);
			}
		}
		Orca::Renderer::EndScene();
	}

	void OnEvent(Orca::Event& event) override {
	}


private:
	// Renderer vars
	std::shared_ptr<Orca::VertexArray> m_VertexArray;
	std::shared_ptr<Orca::Shader> m_Shader;
	std::shared_ptr<Orca::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Orca::IndexBuffer> m_IndexBuffer;

	std::shared_ptr<Orca::VertexArray> m_SquareVA;

	Orca::OrthographicCamera m_Camera;
	float m_CameraSpeed;

	glm::vec3 pos;
	float rot = 0.0f;

	glm::vec3 m_Transform;

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

