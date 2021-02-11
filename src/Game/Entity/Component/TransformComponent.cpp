#include "pch.h"
#include "TransformComponent.h"

#include <glm\gtc\matrix_transform.hpp>

TransformComponent::TransformComponent(Entity* entity)
	: Component(entity), m_Position(glm::vec3(0.0f)), m_Rotation(glm::vec3(0.0f)), m_Scale(glm::vec3(1.0f))
{
}

TransformComponent::TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, Entity* entity)
	: Component(entity), m_Position(position), m_Rotation(rotation), m_Scale(scale)
{
}

glm::mat4 TransformComponent::GetTransformMatrix() const
{
	return glm::translate(glm::mat4(1.0f), m_Position) * 
		   glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f))*
		   glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))*
		   glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))*
		   glm::scale(glm::mat4(1.0f), m_Scale);
}

glm::vec3 TransformComponent::GetUp() const
{
	return glm::normalize(GetOrientation() * glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 TransformComponent::GetRight() const
{
	return glm::normalize(GetOrientation() * glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 TransformComponent::GetFront() const
{
	return glm::normalize(GetOrientation() * glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::quat TransformComponent::GetOrientation() const
{
	return glm::normalize(glm::angleAxis(glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::angleAxis(glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)));
}
