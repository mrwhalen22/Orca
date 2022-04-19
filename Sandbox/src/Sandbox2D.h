#pragma once
#include <iostream>
#include <Orca.h>

#include "Orca/Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

class Sandbox2D : public Orca::Layer {

public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;


	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Orca::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Orca::Event& e);

private:
	Orca::OrthographicCameraController m_CameraController;

	Orca::Ref<Orca::VertexArray> m_VertexArray;
	Orca::Ref<Orca::Shader> m_Shader;

	glm::vec4 m_Color = { 0.2, 0.2, 0.2, 1.0 };
	
};