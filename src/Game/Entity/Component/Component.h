#pragma once

#include "Core\Base.h"

class Entity;

enum class ComponentType
{
	None = 0,
	TransformComponent,
	CameraComponent,
	ScriptComponent
};

class Component
{
public:
	Component(Entity* entity)
		: m_Entity(entity)
	{
	}

	virtual ~Component() = default;

	virtual ComponentType GetType() const { return m_Type; }
protected:
	ComponentType m_Type;
	Entity* m_Entity;
};