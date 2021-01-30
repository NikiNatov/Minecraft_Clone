#include "pch.h"
#include "Texture2D.h"

#include <stb_image.h>

#include <glad\glad.h>

GLenum TextureFormatToGLInternalFormat(TextureFormat format)
{
	switch (format)
	{
		case TextureFormat::RGBA8:	return GL_RGBA8;
		case TextureFormat::RGB8:	return GL_RGB8;
	}

	ASSERT(false, "Texture format not supported!");
	return 0;
}

GLenum TextureFormatToGLFormat(TextureFormat format)
{
	switch (format)
	{
		case TextureFormat::RGBA8:	return GL_RGBA;
		case TextureFormat::RGB8:	return GL_RGB;
	}

	ASSERT(false, "Texture format not supported!");
	return 0;
}

GLenum TextureFilterToGLFilter(TextureFilter filter)
{
	switch (filter)
	{
		case TextureFilter::Linear:		return GL_LINEAR;
		case TextureFilter::Nearest:	return GL_NEAREST;
	}

	ASSERT(false, "Texture filter not supported!");
	return 0;
}

GLenum TextureWrapToGLWrap(TextureWrap wrap)
{
	switch (wrap)
	{
		case TextureWrap::ClampToEdge:		return GL_CLAMP_TO_EDGE;
		case TextureWrap::ClampToBorder:	return GL_CLAMP_TO_BORDER;
		case TextureWrap::Repeat:			return GL_REPEAT;
	}

	ASSERT(false, "Wrap mode not supporterd!");
	return 0;
}

Texture2D::Texture2D(const std::string& filepath, TextureFilter minFilter, TextureFilter magFilter, TextureWrap wrap, bool mips)
{
	stbi_set_flip_vertically_on_load(true);
	int width, height, channels;
	stbi_uc* pixelData = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

	ASSERT(pixelData, "Failed to load texture");

	m_TextureSpecification.Width = width;
	m_TextureSpecification.Height = height;
	m_TextureSpecification.MinFilter = minFilter;
	m_TextureSpecification.MagFilter = magFilter;
	m_TextureSpecification.Wrap = wrap;

	switch (channels)
	{
		case 3: m_TextureSpecification.Format = TextureFormat::RGB8; break;
		case 4: m_TextureSpecification.Format = TextureFormat::RGBA8; break;
		default: ASSERT(false, "Number of channels not supported!");
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, TextureFilterToGLFilter(m_TextureSpecification.MinFilter));
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, TextureFilterToGLFilter(m_TextureSpecification.MagFilter));
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, TextureWrapToGLWrap(m_TextureSpecification.Wrap));
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, TextureWrapToGLWrap(m_TextureSpecification.Wrap));
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, TextureWrapToGLWrap(m_TextureSpecification.Wrap));

	glTexImage2D(GL_TEXTURE_2D, 0, TextureFormatToGLInternalFormat(m_TextureSpecification.Format), m_TextureSpecification.Width, m_TextureSpecification.Height, 0, TextureFormatToGLFormat(m_TextureSpecification.Format), GL_UNSIGNED_BYTE, pixelData);
	
	if(mips)
		glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(pixelData);
}

Texture2D::Texture2D(const TextureSpecification& spec, int mipLevels)
	: m_TextureSpecification(spec)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, TextureFilterToGLFilter(m_TextureSpecification.MinFilter));
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, TextureFilterToGLFilter(m_TextureSpecification.MagFilter));
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, TextureWrapToGLWrap(m_TextureSpecification.Wrap));
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, TextureWrapToGLWrap(m_TextureSpecification.Wrap));
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, TextureWrapToGLWrap(m_TextureSpecification.Wrap));

	glTexStorage2D(GL_TEXTURE_2D, mipLevels, TextureFormatToGLInternalFormat(m_TextureSpecification.Format), m_TextureSpecification.Width, m_TextureSpecification.Height);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture2D::Bind(uint32_t slot)
{
	glBindTextureUnit(slot, m_RendererID);
}

void Texture2D::Unbind(uint32_t slot)
{
	glBindTextureUnit(slot, 0);
}
