#include "pch.h"
#include "SubTexture2D.h"

SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, float x, float y, float spriteWidth, float spriteHeight)
	: m_Texture(texture)
{
	m_TextureCoords[0] = { (x * spriteWidth) / texture->GetWidth(), (y * spriteWidth) / texture->GetHeight() };
	m_TextureCoords[1] = { ((x + 1) * spriteWidth) / texture->GetWidth(),  (y * spriteWidth) / texture->GetHeight() };
	m_TextureCoords[2] = { ((x + 1) * spriteWidth) / texture->GetWidth(),  ((y + 1) * spriteWidth) / texture->GetHeight() };
	m_TextureCoords[3] = { (x * spriteWidth) / texture->GetWidth(),  ((y + 1) * spriteWidth) / texture->GetHeight() };
}
