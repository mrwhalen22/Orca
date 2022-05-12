#pragma once
#include <iostream>
#include <Orca.h>

#include "Orca/Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

using namespace Orca;

class Sandbox2D : public Layer {

public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;


	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Event& e);

private:
	OrthographicCameraController m_CameraController;
	Ref<Texture2D> m_Texture;
	Ref<Texture2D> m_SpriteSheet;

	glm::vec4 m_Color = { 0.6, 0.6, 0.6, 1.0 };
	float angle = 0.0f;

	uint32_t m_MapWidth, m_MapHeight;
	std::unordered_map<char, Ref<SubTexture2D>> m_TileMap;
	
};