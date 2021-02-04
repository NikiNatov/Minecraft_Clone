#pragma once

#include <glm\glm.hpp>

#include "Graphics\Texture2D.h"

class Renderer2D
{
public:
	static void Init();

	static void BeginScene(const glm::mat4& projection);

	static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture);
};