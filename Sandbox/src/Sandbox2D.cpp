#include "Sandbox2D.h"
#include <chrono>

using namespace Orca;

static const uint32_t s_MapWidth = 24;
static const char* s_MapTiles = "WWWWWWWWWWWWWWWWWWWWWWWW"
								"WWWWWWWDDDDDDDWWWWWWWWWW"
								"WWWWWWDDDDDDDDDDDWWWWWWW"
								"WWWWWWDDDDDDDDDDDDDWWWWW"
								"WWWWDDDDDDDDDDDDDDDDWWWW"
								"WWDDDDDDDDDDDDDDDDDDDWWW"
								"WWDDDDDDDDDDDDDDDDDDDWWW"
								"WWDDDDDDDDDDDDDDDDDDDWWW"
								"WDDDDDDDDDDDDDDDDDDDDDWW"
								"WWWDDDDDDDDDDDDDDDDDWWWW"
								"WWWWDDDDDDDDDDDDDDWWWWWW"
								"WWWWWWWWDDDDDDDWWWWWWWWW"
								"WWWWWWWWWWWWWWWWWWWWWWWW"
								"WWWWWWWWWWWWWWWWWWWWWWWW"
								"WWWWWWWWWWWWWWWWWWWWWWWW"
								"WWWWWWWWWWWWWWWWWWWWWWWW";

Sandbox2D::Sandbox2D() 
	: Layer("Sandbox2D"), m_CameraController(16.0f / 9.0f)
{
	
}

void Sandbox2D::OnAttach() {
	OA_PROFILE_FUNCTION();
	m_Texture = Texture2D::Create("assets/textures/orca.png");
	m_SpriteSheet = Texture2D::Create("assets/game/RPGpack_sheet_2X.png");
	m_TileMap['W'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, {11, 11}, {128, 128}, {1, 1});
	m_TileMap['D'] = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6, 11 }, { 128, 128 }, { 1, 1 });
	m_CameraController.SetZoomLevel(5.0f);

	m_MapWidth = s_MapWidth;
	m_MapHeight = strlen(s_MapTiles) / s_MapWidth;
}



void Sandbox2D::OnDetach() {
	OA_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Orca::Timestep ts) {
	OA_PROFILE_FUNCTION();
	//angle += 1*ts;

	Renderer2D::ResetStats();
	m_CameraController.OnUpdate(ts);

	RenderCommand::Clear();
	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });


	Renderer2D::BeginScene(m_CameraController.GetCamera());
	for (int i = 0; i < m_MapHeight; i++) {
		for (int j = 0; j < m_MapWidth; j++) {
			char tileType = s_MapTiles[j + i * m_MapWidth];
			Renderer2D::DrawQuad({ 1.0f * j - m_MapWidth/2.0f, 1.0f * i - m_MapHeight/2.0f}, {1.0f, 1.0f}, 0.0f, m_TileMap[tileType], {1.0f, 1.0f, 1.0f, 1.0f});


		}
	}
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

