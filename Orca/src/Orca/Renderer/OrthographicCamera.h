#pragma once

#include <glm/glm.hpp>

namespace Orca
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetPosition(const glm::vec3& position)
		{
			m_Position = position;
			RecalculateViewMatrix();
		}
		void SetRotation(const float rotation)
		{
			m_Rotation = rotation;
			RecalculateViewMatrix();
		}

		inline const glm::vec3& GetPosition() const { return m_Position; }
		inline const float GetRotation() const { return m_Rotation; }

		void SetProjection(float left, float right, float bottom, float top);

		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_VPMatrix; }

		
	private:
		void RecalculateViewMatrix();


	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_VPMatrix;

		glm::vec3 m_Position;
		float m_Rotation = 0.0f;

	};


}
