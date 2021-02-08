#pragma once

#include <glm\glm.hpp>

#include "Core\Base.h"
#include "Graphics\VertexArray.h"
#include "Graphics\Texture2D.h"

#include "Game\Terrain\Block.h"

class Chunk
{
public:
	Chunk(const glm::vec3& gridPosition);
	~Chunk();

	void Recreate();
	void InitializeVAO();

	inline const Ref<VertexArray>& GetVAO() const { return m_VAO; }
	inline const glm::vec3& GetGridPosition() const { return m_GridPosition; }
	inline BlockID GetBlock(int x, int y, int z) const { return m_Blocks[x][y][z]; }
	inline void SetBlock(int x, int y, int z, BlockID id) const { m_Blocks[x][y][z] = id; }
private:
	void CreateMeshData();
	void GenerateBlocks();
	void AddFace(const BlockFace& face, const glm::vec3& blockPosition);
public:
	static const uint8_t WIDTH = 16;
	static const uint8_t HEIGHT = 255;
	static const uint8_t DEPTH = 16;

	static const uint8_t VOID_LEVEL = 0;
	static const uint8_t WATER_LEVEL = 40;
	static const uint8_t CLOUD_LEVEL = 120;
private:
	BlockID*** m_Blocks;
	glm::vec3 m_GridPosition;

	Ref<VertexArray> m_VAO;
	std::vector<Vertex> m_Vertices;
	std::vector<uint32_t> m_Indices;
	int m_IndicesCount = 0;
};