#pragma once
#include <iostream>
#include <Orca.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

namespace Orca {
	class EditorLayer : public Layer {

	public:
		EditorLayer();
		virtual ~EditorLayer() = default;


		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e);

	private:
		OrthographicCameraController m_CameraController;
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_SpriteSheet;
		Ref<Framebuffer> m_Framebuffer;

		glm::vec2 m_ViewportSize;
		float m_FPS;
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		glm::vec4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float angle = 0.0f;

		uint32_t m_MapWidth, m_MapHeight;
		std::unordered_map<char, Ref<SubTexture2D>> m_TileMap;

	};
}

