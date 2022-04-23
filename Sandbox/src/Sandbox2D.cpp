#include "Sandbox2D.h"

using namespace Orca;

Sandbox2D::Sandbox2D() 
	: Layer("Sandbox2D"), m_CameraController(16.0f / 9.0f)
{

}

void Sandbox2D::OnAttach() {
	


}


void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnUpdate(Orca::Timestep ts) {
	m_CameraController.OnUpdate(ts);

	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	RenderCommand::Clear();
	

	Renderer2D::BeginScene(m_CameraController.GetCamera());
	Renderer2D::DrawQuad({0.0f, 0.0f}, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Renderer2D::EndScene();
}


void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Color", glm::value_ptr(m_Color));
	ImGui::End();
}


void Sandbox2D::OnEvent(Orca::Event& e) {
	m_CameraController.OnEvent(e);
}

