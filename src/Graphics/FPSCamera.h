#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

class FPSCamera
{
public:
	FPSCamera(const glm::vec3& position, float fov, float aspectRatio);

	void OnUpdate(float dt);

	void SetProjection(float fov, float aspectRatio);

	inline const glm::vec3& GetPosition() const { return m_Position; }
	inline const glm::vec3& GetEulerRotation() const { return m_Rotation; }

	inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
	inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
private:
	glm::quat GetOrientation() const;
	glm::vec3 GetCameraUp() const;
	glm::vec3 GetCameraRight() const;
	glm::vec3 GetCameraFront() const;
private:
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;

	float m_Pitch = 0.0f;
	float m_Yaw = 180.0f;
	float m_MovementSpeed = 15.0f;
	float m_RotationSpeed = 20.0f;

	glm::vec2 m_LastMousePosition;

	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
};