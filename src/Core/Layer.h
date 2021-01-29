#pragma once

#include "Core\Events\Event.h"

class Layer
{
public:
	Layer() = default;
	~Layer() = default;

	virtual void OnAttach() {}
	virtual void OnDetach() {}

	virtual void OnUpdate(float dt) {}
	virtual void OnEvent(Event& e) {}
};