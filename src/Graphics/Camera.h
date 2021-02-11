#pragma once

#include <glm\glm.hpp>

class Camera
{
public:
	Camera();
	Camera(float fov, float aspectRatio, float near, float far);

	inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
	
	void SetProjection(float fov, float aspectRatio, float near, float far);

	inline void SetFOV(float fov) { m_Fov = fov; }
	inline void SetNear(float near) { m_Near = near; }
	inline void SetFar(float far) { m_Far = far; }

	inline float GetFOV() const { return m_Fov; }
	inline float GetNear() const { return m_Near; }
	inline float GetFar() const { return m_Far; }
private:
	void RecalculateProjection();
private:
	glm::mat4 m_ProjectionMatrix;

	float m_Fov = 45.0f;
	float m_AspectRatio = 16.0f / 9.0f;
	float m_Near = 0.01f;
	float m_Far = 1000.0f;
};