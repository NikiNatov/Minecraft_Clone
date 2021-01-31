#pragma once

#include <unordered_map>
#include "Graphics\SubTexture2D.h"

class SpriteManager
{
public:
	SpriteManager(const SpriteManager&) = delete;
	const SpriteManager& operator=(const SpriteManager&) = delete;

	static void AddSprite(const std::string& name, const Ref<SubTexture2D>& subTexture);
	static const Ref<SubTexture2D>& GetSprite(const std::string& name) { return m_Sprites[name]; }
private:
	SpriteManager() = default;
private:
	static std::unordered_map<std::string, Ref<SubTexture2D>> m_Sprites;
};