#include "pch.h"
#include "Camera.h"

#include <glm\gtc\matrix_transform.hpp>

Camera::Camera()
{
	RecalculateProjection();
}

Camera::Camera(float fov, float aspectRatio, float near, float far)
	: m_Fov(fov), m_AspectRatio(aspectRatio), m_Near(near), m_Far(far)
{
	RecalculateProjection();
}

void Camera::SetProjection(float fov, float aspectRatio, float near, float far)
{
	m_Fov = fov;
	m_AspectRatio = aspectRatio;
	m_Near = near;
	m_Far = far;

	RecalculateProjection();
}

void Camera::RecalculateProjection()
{
	m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_Near, m_Far);
}
