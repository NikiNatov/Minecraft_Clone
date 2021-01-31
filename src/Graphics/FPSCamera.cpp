#include "pch.h"
#include "FPSCamera.h"

#include <glm\gtx\transform.hpp>

#include "Core\Input.h"
#include "Core\KeyCodes.h"
#include "Core\MouseCodes.h"

FPSCamera::FPSCamera(const glm::vec3& position, float fov, float aspectRatio)
	: m_Position(position)
{
	m_ProjectionMatrix = glm::perspective(fov, aspectRatio, 0.1f, 1000.0f);
	m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	m_LastMousePosition = Input::GetMousePosition();
}

void FPSCamera::OnUpdate(float dt)
{
	glm::vec2 currentMousePos = Input::GetMousePosition();
	glm::vec2 mouseDelta = currentMousePos - m_LastMousePosition;
	m_LastMousePosition = currentMousePos;

	if (Input::IsKeyPressed(Key::W))
		m_Position += GetCameraFront() * m_MovementSpeed * dt;
	else if (Input::IsKeyPressed(Key::S))
		m_Position -= GetCameraFront() * m_MovementSpeed * dt;
	if (Input::IsKeyPressed(Key::D))
		m_Position += GetCameraRight() * m_MovementSpeed * dt;
	else if (Input::IsKeyPressed(Key::A))
		m_Position -= GetCameraRight() * m_MovementSpeed * dt;

	float sign = GetCameraUp().y < 0.0f ? -1.0f : 1.0f;
	m_Yaw += sign * mouseDelta.x * m_RotationSpeed * dt;
	m_Pitch += mouseDelta.y * m_RotationSpeed * dt;

	if (m_Pitch >= 89.0f)
		m_Pitch = 89.0f;
	if (m_Pitch <= -89.0f)
		m_Pitch = -89.0f;

	m_Rotation = glm::degrees(glm::eulerAngles(GetOrientation()));
	m_ViewMatrix = glm::lookAt(m_Position, m_Position + GetCameraFront(), glm::vec3(0.0f, 1.0f, 0.0f));
}

void FPSCamera::SetProjection(float fov, float aspectRatio)
{
	m_ProjectionMatrix = glm::perspective(fov, aspectRatio, 0.1f, 1000.0f);
}

glm::quat FPSCamera::GetOrientation() const
{
	return glm::normalize(glm::angleAxis(glm::radians(-m_Yaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::angleAxis(glm::radians(-m_Pitch), glm::vec3(1.0f, 0.0f, 0.0f)));
}

glm::vec3 FPSCamera::GetCameraUp() const
{
	return glm::normalize(GetOrientation() * glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 FPSCamera::GetCameraRight() const
{
	return glm::normalize(GetOrientation() * glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 FPSCamera::GetCameraFront() const
{
	return glm::normalize(GetOrientation() * glm::vec3(0.0f, 0.0f, -1.0f));
}