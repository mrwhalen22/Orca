#include "oapch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Orca
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(glm::mat4(1.0f)), m_Position(glm::vec3(0.0f))
	{
		OA_PROFILE_FUNCTION();
		m_VPMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top) {
		OA_PROFILE_FUNCTION();
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_VPMatrix = m_ProjectionMatrix * m_ViewMatrix;

	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		OA_PROFILE_FUNCTION();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
		transform = glm::rotate(transform, glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		m_ViewMatrix = glm::inverse(transform);
		m_VPMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}





}