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
	if (i > 15) i = 0;
	i += ts * 5;

	m_CameraController.OnUpdate(ts);

	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	RenderCommand::Clear();
	

	Renderer2D::BeginScene(m_CameraController.GetCamera());
	for (int n = 0; n < i; n++) {
		Renderer2D::DrawQuad({ 0.2 * n - 1.5f, -0.03f*n }, { 0.1f, 1.4f }, 0.1f*n, m_Color);

	}
	

	
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

