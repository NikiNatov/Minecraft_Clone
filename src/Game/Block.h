#pragma once

#include <glm\glm.hpp>
#include <vector>

#include "Core\Base.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec3 Normal;
};

enum class BlockFaceID : uint8_t
{
	Front	= 0,
	Back	= 1,
	Left	= 2,
	Right	= 3,
	Up		= 4,
	Down	= 5
};

struct BlockFace
{
	Vertex Vertices[4];
	uint32_t Indices[6];
};

enum class BlockID : uint8_t
{
	Air		= 0,
	Grass	= 1,
	Dirt	= 2,
	Stone	= 3
};

class Block
{
public:
	Block(BlockID id);

	inline BlockID GetID() const { return m_ID; }
	inline const BlockFace& GetFrontFace() const { return m_Faces[(uint8_t)BlockFaceID::Front]; }
	inline const BlockFace& GetBackFace() const { return m_Faces[(uint8_t)BlockFaceID::Back]; }
	inline const BlockFace& GetLeftFace() const { return m_Faces[(uint8_t)BlockFaceID::Left]; }
	inline const BlockFace& GetRightFace() const { return m_Faces[(uint8_t)BlockFaceID::Right]; }
	inline const BlockFace& GetUpFace() const { return m_Faces[(uint8_t)BlockFaceID::Up]; }
	inline const BlockFace& GetDownFace() const { return m_Faces[(uint8_t)BlockFaceID::Down]; }
public:
	static ScopedPtr<Block> s_GrassBlock;
	static ScopedPtr<Block> s_DirtBlock ;
	static ScopedPtr<Block> s_StoneBlock;
private:
	BlockID m_ID;
	BlockFace m_Faces[6];
};