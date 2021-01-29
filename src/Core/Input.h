#pragma once

#include <glm\glm.hpp>

class Input
{
public:
	static bool IsKeyPressed(int keycode);
	static bool IsMouseButtonPressed(int button);

	static const glm::vec2& GetMousePosition();
};