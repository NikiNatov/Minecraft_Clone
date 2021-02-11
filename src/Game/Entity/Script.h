#pragma once

#include "Entity.h"

class Script
{
public:
	friend class World;

	Script(Entity* entity)
		: m_Entity(entity)
	{
	}

	virtual ~Script() = default;

	template<typename T>
	Ref<T> GetComponent()
	{
		return m_Entity->GetComponent<T>();
	}

	World* GetWorld()
	{
		return m_Entity->GetWorld();
	}
protected:
	virtual void OnCreate() {}
	virtual void OnUpdate(float dt) {}
	virtual void OnDestroy() {}
private:
	Entity* m_Entity;
};