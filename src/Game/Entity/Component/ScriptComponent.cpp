#include "pch.h"
#include "ScriptComponent.h"

ScriptComponent::ScriptComponent(Entity* entity)
	: Component(entity)
{
}

ScriptComponent::~ScriptComponent()
{
	delete m_Instance;
	m_Instance = nullptr;
}
