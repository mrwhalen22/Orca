#include "Sandbox2D.h"
#include <chrono>

using namespace Orca;

template<typename Fn>
class Timer {
public:
	Timer(const char* name)
		: m_Name(name), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	Timer(const char* name, Fn&& func)
		: m_Name(name), m_Stopped(false), m_Func(func)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	void Stop() {
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
	
		m_Stopped = true;
		float duration = (end - start) * 0.001f;

		m_Func({ m_Name, duration });
	}

	~Timer() {
		if (!m_Stopped)
			Stop();
	}


private:
	const char* m_Name;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
	Fn m_Func;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](struct ProfileResult profileResult) {m_ProfileResults.push_back(profileResult);}) 

Sandbox2D::Sandbox2D() 
	: Layer("Sandbox2D"), m_CameraController(16.0f / 9.0f)
{
	
}

void Sandbox2D::OnAttach() {
	
	m_Texture = Texture2D::Create("assets/textures/orca.png");
}


void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnUpdate(Orca::Timestep ts) {
	PROFILE_SCOPE("Sandbox2D::OnUpdate");
	angle += 1*ts;
	{
		PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	{
		PROFILE_SCOPE("Rendering Prep");
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();
	}
	
	
	{
		PROFILE_SCOPE("Render Draw");
		Renderer2D::BeginScene(m_CameraController.GetCamera());
		Renderer2D::DrawQuad({ -1.0f, -0.75f });
		Renderer2D::DrawQuad({ 0.5f, 0.5f }, { 1.0f, 1.0f }, angle, m_Color);
		Renderer2D::DrawQuad({ -0.5f, 0.6f }, { 0.5f, 0.5f }, 0, m_Texture);
		Renderer2D::DrawQuad({ 0.5f, -0.75f }, { 1.0f, 2.0f }, angle, m_Texture, m_Color);
		Renderer2D::EndScene();
	}
	
}


void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));
	for (auto& result : m_ProfileResults) {
		ImGui::Text("%.3fms  %s", result.Time, result.Name);
	}
	m_ProfileResults.clear();

	ImGui::End();
}


void Sandbox2D::OnEvent(Orca::Event& e) {
	m_CameraController.OnEvent(e);
}

