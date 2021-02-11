#pragma once

#include "Game\Entity\Script.h"

class ScriptComponent : public Component
{
public:
	ScriptComponent(Entity* entity);
	virtual ~ScriptComponent();

	template<typename T>
	void Bind()
	{
		m_Instance = new T(m_Entity);
	}

	inline Script* GetInstance() { return m_Instance; }

	static ComponentType GetStaticType() { return ComponentType::ScriptComponent; }
private:
	Script* m_Instance;
};