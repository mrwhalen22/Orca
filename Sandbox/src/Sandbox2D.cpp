#include "Sandbox2D.h"

Sandbox2D::Sandbox2D() 
	: Layer("Sandbox2D"), m_CameraController(16.0 / 9.0)
{

}

void Sandbox2D::OnAttach() {
	float vertices[] = {
			-1.0f, -1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
	};



	m_VertexArray = Orca::VertexArray::Create();
	Orca::Ref<Orca::VertexBuffer> squareVB;
	squareVB.reset(Orca::VertexBuffer::Create(vertices, sizeof(vertices)));

	{
		Orca::BufferLayout layout = {
			{Orca::ShaderDataType::Float3, "a_Position"}
		};
		squareVB->SetLayout(layout);
	}


	unsigned int squareIndices[] = { 0, 1, 2, 2, 3, 0 };
	Orca::Ref<Orca::IndexBuffer> squareIB;
	squareIB.reset(Orca::IndexBuffer::Create(squareIndices, 6));

	m_VertexArray->AddVertexBuffer(squareVB);
	m_VertexArray->SetIndexBuffer(squareIB);



	std::string ShaderPath = "assets/shaders/FlatColor.glsl";
	m_Shader = Orca::Shader::Create(ShaderPath);


}


void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnUpdate(Orca::Timestep ts) {
	m_CameraController.OnUpdate(ts);

	Orca::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Orca::RenderCommand::Clear();

	std::dynamic_pointer_cast<Orca::OpenGLShader>(m_Shader)->Bind();
	std::dynamic_pointer_cast<Orca::OpenGLShader>(m_Shader)->UploadUniformFloat4("u_Color", m_Color);
	

	Orca::Renderer::BeginScene(m_CameraController.GetCamera());
	Orca::Renderer::Submit(m_VertexArray, m_Shader);
	Orca::Renderer::EndScene();
}


void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Color", glm::value_ptr(m_Color));
	ImGui::End();
}


void Sandbox2D::OnEvent(Orca::Event& e) {
	m_CameraController.OnEvent(e);
}

