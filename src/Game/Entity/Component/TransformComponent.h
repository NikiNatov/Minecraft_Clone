#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

#include "Game\Entity\Component\Component.h"

class TransformComponent : public Component
{
public:
	TransformComponent(Entity* entity);
	TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, Entity* entity);

	glm::mat4 GetTransformMatrix() const;

	inline glm::vec3& GetPosition() { return m_Position; }
	inline glm::vec3& GetRotation() { return m_Rotation; }
	inline glm::vec3& GetScale() { return m_Scale; }

	glm::vec3 GetUp() const;
	glm::vec3 GetRight() const;
	glm::vec3 GetFront() const;

	static ComponentType GetStaticType() { return ComponentType::TransformComponent; }

private:
	glm::quat GetOrientation() const;
private:
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;
};