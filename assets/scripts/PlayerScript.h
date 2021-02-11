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

		m_Velocity = glm::vec3(0.0f);

		if (Input::IsKeyPressed(Key::W))
			m_Velocity += transform->GetFront() * m_MovementSpeed;
		else if (Input::IsKeyPressed(Key::S))
			m_Velocity += -transform->GetFront() * m_MovementSpeed;
		if (Input::IsKeyPressed(Key::D))
			m_Velocity += transform->GetRight() * m_MovementSpeed;
		else if (Input::IsKeyPressed(Key::A))
			m_Velocity += -transform->GetRight() * m_MovementSpeed;
		if (Input::IsKeyPressed(Key::LeftShift))
			m_Velocity += glm::vec3(0.0f, -m_MovementSpeed, 0.0f);
		else if (Input::IsKeyPressed(Key::Space))
			m_Velocity += glm::vec3(0.0f, m_MovementSpeed, 0.0f);

		transform->GetPosition().x += m_Velocity.x * dt;
		CollisionCheck({ m_Velocity.x * dt, 0.0f, 0.0f });

		transform->GetPosition().y += m_Velocity.y * dt;
		CollisionCheck({ 0.0f, m_Velocity.y * dt, 0.0f });

		transform->GetPosition().z += m_Velocity.z * dt;
		CollisionCheck({ 0.0f, 0.0f, m_Velocity.z * dt });

		m_Velocity.x *= 0.90f;
		m_Velocity.z *= 0.90f;
	}

	virtual void OnDestroy() override
	{

	}

	void CollisionCheck(const glm::vec3& velocity)
	{
		glm::vec3 bounds = { 0.5f, 1.5f, 0.5f };

		auto& position = GetComponent<TransformComponent>()->GetPosition();

		float xFrom = position.x - bounds.x < 0.0f ? std::floor(position.x - bounds.x) : position.x - bounds.x;
		float xTo = position.x + bounds.x;

		float yFrom = position.y - bounds.y < 0.0f ? std::floor(position.y - bounds.y) : position.y - bounds.y;
		float yTo = position.y + 0.5f;

		float zFrom = position.z - bounds.z < 0.0f ? std::floor(position.z - bounds.z) : position.z - bounds.z;
		float zTo = position.z + bounds.z;

		for (int x = xFrom; x < xTo; x++)
		{
			for (int y = yFrom; y < yTo; y++)
			{
				for (int z = zFrom; z < zTo; z++)
				{

					BlockID block = GetWorld()->GetBlock({ x, y, z });
					if (block != BlockID::Air && block != BlockID::Water)
					{
						if (velocity.y > 0)
						{
							position.y = y - 0.5f;
							m_Velocity.y = 0;
						}
						else if (velocity.y < 0)
						{
							position.y = y + bounds.y + 1;
							m_Velocity.y = 0;
						}

						if (velocity.x > 0)
						{
							position.x = x - bounds.x;
						}
						else if (velocity.x < 0)
						{
							position.x = x + bounds.x + 1;
						}

						if (velocity.z > 0)
						{
							position.z = z - bounds.z;
						}
						else if (velocity.z < 0)
						{
							position.z = z + bounds.z + 1;
						}
					}
				}
			}
		}
	}
private:
	glm::vec2 m_LastMousePos;
	float m_MouseSensitivity = 7.0f;
	float m_MovementSpeed = 7.0f;
	glm::vec3 m_Velocity = glm::vec3(0.0f);
};