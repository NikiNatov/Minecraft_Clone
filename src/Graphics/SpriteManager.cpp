#include "pch.h"
#include "SpriteManager.h"

std::unordered_map<std::string, Ref<SubTexture2D>> SpriteManager::m_Sprites;

void SpriteManager::AddSprite(const std::string& name, const Ref<SubTexture2D>& subTexture)
{
	ASSERT(subTexture, "Subtexture is null!");
	m_Sprites[name] = subTexture;
}
