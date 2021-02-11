#pragma once

#include "Core\Input.h"

#include "Game\Entity\Script.h"

class FPSCameraScript : public Script
{
public:
	FPSCameraScript(Entity* entity)
		: Script(entity)
	{
	}

	virtual void OnCreate() override
	{
		m_LastMousePos = Input::GetMousePosition();
	}

	virtual void OnUpdate(float dt) override
	{
		glm::vec2 mousePos = Input::GetMousePosition();
		glm::vec2 delta = mousePos - m_LastMousePos;
		m_LastMousePos = mousePos;

		auto transform = GetComponent<TransformComponent>();

		transform->GetRotation().x -= delta.y * m_MouseSensitivity * dt;

		if (transform->GetRotation().x > 89.0f)
			transform->GetRotation().x = 89.0f;
		else if (transform->GetRotation().x < -89.0f)
			transform->GetRotation().x = -89.0f;
	}

	virtual void OnDestroy() override
	{

	}
private:
	glm::vec2 m_LastMousePos;
	float m_MouseSensitivity = 7.0f;
};