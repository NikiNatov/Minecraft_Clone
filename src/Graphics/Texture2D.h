#pragma once

#include "Core\Base.h"

enum class TextureFormat
{
	None = 0,
	RGBA8, 
	RGB8
};

enum class TextureFilter
{
	None = 0,
	Nearest,
	MipmapNearest,
	Linear,
	MipmapLinear
};

enum class TextureWrap
{
	None = 0,
	ClampToEdge,
	ClampToBorder,
	Repeat
};

struct TextureSpecification
{
	uint32_t Width;
	uint32_t Height;
	TextureFormat Format;
	TextureFilter MinFilter;
	TextureFilter MagFilter;
	TextureWrap Wrap;
};

class Texture2D
{
public:
	Texture2D(const std::string& filepath, TextureFilter minFilter, TextureFilter magFilter, TextureWrap wrap, bool mips = false);
	Texture2D(const TextureSpecification& spec, int mipLevels = 0);
	~Texture2D();

	void Bind(uint32_t slot = 0);
	void Unbind(uint32_t slot = 0);

	inline uint32_t GetWidth() const { return m_TextureSpecification.Width; }
	inline uint32_t GetHeight() const { return m_TextureSpecification.Height; }
	inline TextureFormat GetFormat() const { return m_TextureSpecification.Format; }
	inline TextureFilter GetMinFilter() const { return m_TextureSpecification.MinFilter; }
	inline TextureFilter GetMagFilter() const { return m_TextureSpecification.MagFilter; }
	inline TextureWrap GetWrapMode() const { return m_TextureSpecification.Wrap; }
private:
	uint32_t m_RendererID;
	TextureSpecification m_TextureSpecification;
};