#pragma once

#include <glm\glm.hpp>
#include "Graphics\Texture2D.h"

class SubTexture2D
{
public:
	SubTexture2D(const Ref<Texture2D>& texture, float x, float y, float spriteWidth, float spriteHeight);

	inline const Ref<Texture2D> GetTexture() const { return m_Texture; }
	inline const glm::vec2* GetTextureCoords() const { return m_TextureCoords; }
private:
	Ref<Texture2D> m_Texture;
	glm::vec2 m_TextureCoords[4];
};