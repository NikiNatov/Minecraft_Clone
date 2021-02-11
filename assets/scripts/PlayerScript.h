#pragma once

#include "Core\Input.h"
#include "Core\KeyCodes.h"


#include "Game\Entity\Script.h"

class PlayerScript : public Script
{
public:
	PlayerScript(Entity* entity)
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

		transform->GetRotation().y -= delta.x * m_MouseSensitivity * dt;

		if (Input::IsKeyPressed(Key::W))
			transform->GetPosition() += transform->GetFront() * m_MovementSpeed * dt;
		else if (Input::IsKeyPressed(Key::S))
			transform->GetPosition() -= transform->GetFront() * m_MovementSpeed * dt;
		if (Input::IsKeyPressed(Key::D))
			transform->GetPosition() += transform->GetRight() * m_MovementSpeed * dt;
		else if (Input::IsKeyPressed(Key::A))
			transform->GetPosition() -= transform->GetRight() * m_MovementSpeed * dt;
		if (Input::IsKeyPressed(Key::LeftShift))
			transform->GetPosition().y -= m_MovementSpeed * dt;
		else if(Input::IsKeyPressed(Key::Space))
			transform->GetPosition().y += m_MovementSpeed * dt;
	}

	virtual void OnDestroy() override
	{

	}
private:
	glm::vec2 m_LastMousePos;
	float m_MouseSensitivity = 7.0f;
	float m_MovementSpeed = 10.0f;
};