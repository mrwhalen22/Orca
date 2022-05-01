#include "Sandbox2D.h"
#include <chrono>

#include <glm/gtx/common.hpp>

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

	m_CameraController.OnUpdate(ts);

	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	RenderCommand::Clear();

	Renderer2D::BeginScene(m_CameraController.GetCamera());
	Renderer2D::DrawQuad({ -1.0f, -0.75f, 0.0f }, { 2,1 },  m_Color);
	Renderer2D::DrawQuad({ -0.5f, 0.6f, 0.0f }, { 0.5f, 0.5f }, glm::radians(45.0f), m_Texture, {1.0f,1.0f,1.0f,1.0f});
	Renderer2D::DrawQuad({ { 0.5f, -0.75f, 0.0f }, { 1.0f, 1.0f }, angle, m_Texture, m_Color });
	Renderer2D::EndScene();
	
}


void Sandbox2D::OnImGuiRender() {
	OA_PROFILE_FUNCTION();
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));

	ImGui::End();
}


void Sandbox2D::OnEvent(Orca::Event& e) {
	OA_PROFILE_FUNCTION();
	m_CameraController.OnEvent(e);
}

