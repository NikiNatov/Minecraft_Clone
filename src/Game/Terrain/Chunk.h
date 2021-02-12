#pragma once

#include <glm\glm.hpp>

#include "Core\Base.h"
#include "Graphics\VertexArray.h"
#include "Graphics\Texture2D.h"

#include "Game\Terrain\Block.h"
#include "Game\Terrain\ChunkMesh.h"

class Chunk
{
public:
	Chunk(const glm::vec3& gridPosition);
	~Chunk();

	void Recreate();
	void InitializeMeshesVAOs();

	inline const Ref<ChunkMesh> GetSolidMesh() const { return m_SolidMesh; }
	inline const Ref<ChunkMesh> GetFluidMesh() const { return m_FluidMesh; }

	inline const glm::vec3& GetGridPosition() const { return m_GridPosition; }
	inline BlockID GetBlock(int x, int y, int z) const { return m_Blocks[x][y][z]; }
	inline void SetBlock(int x, int y, int z, BlockID id) const { m_Blocks[x][y][z] = id; }
private:
	void CreateMeshData();
	void GenerateBlocks();
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

	Ref<ChunkMesh> m_SolidMesh;
	Ref<ChunkMesh> m_FluidMesh;
};