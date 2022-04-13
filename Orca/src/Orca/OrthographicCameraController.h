#pragma once

#include "Orca/Renderer/OrthographicCamera.h"
#include "Orca/Core/Timestep.h"
#include "Orca/Events/ApplicationEvent.h"
#include "Orca/Events/MouseEvent.h"


namespace Orca {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false); // 2 units Y

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		inline OrthographicCamera& GetCamera() { return m_Camera; }
		inline const OrthographicCamera& GetCamera() const { return m_Camera; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		float m_CameraRotation;
		glm::vec3 m_CameraPosition = { 0,0,0 };
		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 180.0f;

	};
}


