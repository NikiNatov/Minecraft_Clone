#pragma once

#include "Game\Entity\Component\Component.h"

#include "Graphics\Camera.h"

class CameraComponent : public Component
{
public:
	CameraComponent(Entity* entity);
	CameraComponent(float fov, float aspectRatio, float near, float far, Entity* entity);

	inline Camera& GetCamera() { return m_Camera; }

	static ComponentType GetStaticType() { return ComponentType::CameraComponent; }
private:
	Camera m_Camera;
};