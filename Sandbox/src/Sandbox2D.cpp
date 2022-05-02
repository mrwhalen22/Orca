#include "Sandbox2D.h"
#include <chrono>

using namespace Orca;


Sandbox2D::Sandbox2D() 
	: Layer("Sandbox2D"), m_CameraController(16.0f / 9.0f)
{
	
}

void Sandbox2D::OnAttach() {
	OA_PROFILE_FUNCTION();
	m_Texture = Texture2D::Create("assets/textures/orca.png");
}


void Sandbox2D::OnDetach() {
	OA_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Orca::Timestep ts) {
	OA_PROFILE_FUNCTION();
	angle += 1*ts;

	Renderer2D::ResetStats();
	m_CameraController.OnUpdate(ts);

	RenderCommand::Clear();
	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	

	Renderer2D::BeginScene(m_CameraController.GetCamera());
	for (float y = -5.0f; y < 5.0f; y += 0.5f) {
		for (float x = -5.0f; x < 5.0f; x += 0.5f) {
			glm::vec3 color = { (x + 5.0f) / 10.0f, 0.3f, (y + 5.0f) / 10.0f };
			Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, { color.r, color.g, color.b, 0.6f });
		}
	}

	Renderer2D::EndScene();


	Renderer2D::BeginScene(m_CameraController.GetCamera());

	Renderer2D::DrawQuad({ -1.0f, -0.75f, 0.9f }, { 1,2 }, m_Color);
	Renderer2D::DrawQuad({ { 0.5f, -0.75f, 1.0f }, { 1.0f, 1.0f }, angle, m_Texture, m_Color });
	Renderer2D::DrawQuad({ -0.5f, -0.6f, 1.0f }, { 0.5f, 0.5f }, glm::radians(45.0f), m_Texture, { 1.0f,1.0f,1.0f,1.0f });
	

	Renderer2D::EndScene();


	
	
}


void Sandbox2D::OnImGuiRender() {
	OA_PROFILE_FUNCTION();

	auto stats = Renderer2D::GetStats();

	ImGui::Begin("Settings");
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));

	ImGui::End();
}


void Sandbox2D::OnEvent(Orca::Event& e) {
	OA_PROFILE_FUNCTION();
	m_CameraController.OnEvent(e);
}

