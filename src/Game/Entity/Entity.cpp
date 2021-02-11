#include "pch.h"
#include "Entity.h"

#include "Game\Terrain\World.h"

Entity::Entity(World* world)
	: m_World(world)
{
	AddComponent<TransformComponent>();
}

Entity::Entity(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, World* world)
	: m_World(world)
{
	AddComponent<TransformComponent>(position, rotation, scale);
}
