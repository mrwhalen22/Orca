#include <iostream>
#include <Hazel.h>

class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer()
		:Layer("Example"), m_Camera(-3.2f, 3.2f, -1.8f, 1.8f), pos(0.0f)
	{
		m_VertexArray.reset(Hazel::VertexArray::Create());
		m_VertexArray->Bind();


		float vertices[] = {
			-0.75f, -0.75f, 0.0f,	0.9f, 0.7f, 0.1f, 1.0f,
			 0.75f, -0.75f, 0.0f,	0.9f, 0.1f, 0.1f, 1.0f,
			 0.75f,  0.75f,	0.0f,	0.9f, 0.1f, 0.9f, 1.0f,
			-0.75f,  0.75f,	0.0f,	0.1f, 0.1f, 0.9f, 1.0f,
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

			out vec3 v_Position;
			out vec4 v_Color;
	
			void main() {
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_VPMatrix * vec4(a_Position, 1.0);
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

	void OnUpdate() override {
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();

		if (Hazel::Input::IsKeyPressed(HZ_KEY_UP)) pos.y += 0.05f;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN)) pos.y -= 0.05f;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT)) pos.x += 0.05f;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT)) pos.x -= 0.05f;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_Q)) rot -= 2.0f;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_E)) rot += 2.0f;

		if(Hazel::Input::IsKeyPressed(HZ_KEY_R))
		{
			pos = { 0,0,0 };
			rot = 0;
		}


		m_Camera.SetPosition(pos);
		m_Camera.SetRotation(rot);

		Hazel::Renderer::BeginScene(m_Camera);
		Hazel::Renderer::Submit(m_SquareVA, m_Shader);
		Hazel::Renderer::EndScene();
	}

	void OnEvent(Hazel::Event& event) override {
		Hazel::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Hazel::KeyPressedEvent& event)
	{
		


		return false;
	}

private:
	// Renderer vars
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;
	std::shared_ptr<Hazel::Shader> m_Shader;
	std::shared_ptr<Hazel::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Hazel::IndexBuffer> m_IndexBuffer;

	std::shared_ptr<Hazel::VertexArray> m_SquareVA;

	Hazel::OrthographicCamera m_Camera;

	glm::vec3 pos;
	float rot = 0.0f;

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

