#include <iostream>
#include <Hazel.h>

#include "imgui/imgui.h"

class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer()
		:Layer("Example")
	{}

	void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::End();
	}

	void OnUpdate() override {
		//HZ_INFO("ExampleLayer::Update");

		if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
			HZ_TRACE("Tab Key Is pressed!");
	}

	void OnEvent(Hazel::Event& event) override {
		//HZ_TRACE("{0}", event);
	}

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

