#pragma once

#include "Core\Layer.h"
#include "Core\Events\KeyEvent.h"

class GameLayer : public Layer
{
public:
	GameLayer();
	~GameLayer();


	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(float dt) override;
	virtual void OnEvent(Event& e) override;

private:
	bool OnKeyPressed(KeyPressedEvent& e);
};