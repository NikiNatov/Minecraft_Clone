#pragma once

#include <glm\glm.hpp>
#include <vector>

#include "Core\Base.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec3 Normal;
	float LightLevel;
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

enum class BlockID : int8_t
{
	Air		= -1,
	Grass	=  0,
	Dirt	=  1,
	Stone	=  2,
	Bedrock	=  3,
	Water	=  4,
	Sand	=  5,
	Wood	=  6,
	Leaf	=  7,
	Plank	=  8,
	Glass	=  9,
};

class Block
{
public:
	Block(BlockID id);

	inline BlockID GetID() const { return m_ID; }
	inline const BlockFace& GetFace(BlockFaceID id) const { return m_Faces[(uint8_t)id]; }
	void SetBlockTextures(BlockID block);

	static void CreateBlockTemplates();
public:
	static std::vector<ScopedPtr<Block>> s_Blocks;
private:
	BlockID m_ID;
	BlockFace m_Faces[6];
};