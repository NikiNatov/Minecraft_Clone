#pragma once

#include <unordered_map>

#include "Game\Entity\Component\Component.h"
#include "Game\Entity\Component\TransformComponent.h"

class World;

class Entity
{
public:
	Entity(World* world);
	Entity(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, World* world);

	template<typename T, typename... Args>
	Ref<T> AddComponent(Args&&... args)
	{
		ASSERT(m_Components.find(T::GetStaticType()) == m_Components.end(), "Entity already has this type of component!");
		Ref<T> component = CreateRef<T>(std::forward<Args>(args)..., this);
		m_Components[T::GetStaticType()] = component;
		return component;
	}

	template<typename T>
	Ref<T> GetComponent()
	{
		ASSERT(m_Components.find(T::GetStaticType()) != m_Components.end(), "Entity does not have this type of component!");
		return std::dynamic_pointer_cast<T>(m_Components[T::GetStaticType()]);
	}

	inline World* GetWorld() { return m_World; }
private:
	std::unordered_map<ComponentType, Ref<Component>> m_Components;
	World* m_World = nullptr;
};