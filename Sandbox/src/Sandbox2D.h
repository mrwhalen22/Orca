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

	glm::vec4 m_Color = { 0.2, 0.2, 0.2, 1.0 };
	
};