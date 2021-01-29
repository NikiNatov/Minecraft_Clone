#include "pch.h"
#include "GameLayer.h"

#include "Core\Base.h"

#include <iostream>

GameLayer::GameLayer()
{
}

GameLayer::~GameLayer()
{
}

void GameLayer::OnAttach()
{
	std::cout << "Game Layer: Layer attached" << std::endl;
}

void GameLayer::OnDetach()
{
	std::cout << "Game Layer: Layer detached" << std::endl;
}

void GameLayer::OnUpdate(float dt)
{
}

void GameLayer::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);

	dispatcher.Dispatch<KeyPressedEvent>(BIND_FN(GameLayer::OnKeyPressed));
}

bool GameLayer::OnKeyPressed(KeyPressedEvent& e)
{
	std::cout << "Game Layer: " << e.ToString() << std::endl;
	return false;
}