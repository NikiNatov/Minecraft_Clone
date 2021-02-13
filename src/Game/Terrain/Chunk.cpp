#include "pch.h"
#include "Chunk.h"

#include <random>
#include <glm\gtc\noise.hpp>

#include "Game\Terrain\Noise.h"

Chunk::Chunk(const glm::vec3& gridPosition)
	: m_GridPosition(gridPosition)
{
	m_Blocks = new BlockID **[WIDTH];
	
	for (uint8_t x = 0; x < WIDTH; x++)
	{
		m_Blocks[x] = new BlockID *[HEIGHT];
		for (uint8_t y = 0; y < HEIGHT; y++)
		{
			m_Blocks[x][y] = new BlockID[DEPTH]{ BlockID::Air };
		}
	}

	GenerateBlocks();
	CreateMeshData();
}

Chunk::~Chunk()
{
	for (uint8_t x = 0; x < WIDTH; x++)
	{
		for (uint8_t y = 0; y < HEIGHT; y++)
		{
			delete[] m_Blocks[x][y];
		}
		delete[] m_Blocks[x];
	}

	delete[] m_Blocks;

	m_Blocks = nullptr;
}

void Chunk::Recreate()
{
	CreateMeshData();
	InitializeMeshesVAOs();
}

void Chunk::InitializeMeshesVAOs()
{
	m_SolidMesh->InitializeVAO();
	m_WaterMesh->InitializeVAO();
	m_TransparentMesh->InitializeVAO();
}

void Chunk::CreateMeshData()
{
	if (!m_SolidMesh)
		m_SolidMesh = CreateRef<ChunkMesh>();

	if (!m_WaterMesh)
		m_WaterMesh = CreateRef<ChunkMesh>();

	if (!m_TransparentMesh)
		m_TransparentMesh = CreateRef<ChunkMesh>();

	m_SolidMesh->Clear();
	m_WaterMesh->Clear();
	m_TransparentMesh->Clear();

	for (uint8_t x = 0; x < WIDTH; x++)
	{
		for (uint8_t y = 0; y < HEIGHT; y++)
		{
			for (uint8_t z = 0; z < DEPTH; z++)
			{
				BlockID blockType = m_Blocks[x][y][z];
				if (blockType != BlockID::Air)
				{
					const Block* block = Block::s_Blocks[(int8_t)m_Blocks[x][y][z]].get();

					if (blockType == BlockID::Water)
					{
						m_WaterMesh->AddFace(block->GetFace(BlockFaceID::Up), { x , y - 0.2f, z });
					}
					if (x == 0 || (x - 1 >= 0 && (m_Blocks[x - 1][y][z] == BlockID::Air || m_Blocks[x - 1][y][z] == BlockID::Water || m_Blocks[x - 1][y][z] == BlockID::Glass)))
					{
						if (blockType != BlockID::Water)
						{
							if(blockType == BlockID::Glass)
								m_TransparentMesh->AddFace(block->GetFace(BlockFaceID::Left), { x , y, z });
							else
								m_SolidMesh->AddFace(block->GetFace(BlockFaceID::Left), { x , y, z });
						}
					}
					if (x == WIDTH - 1 || (x + 1 < WIDTH && (m_Blocks[x + 1][y][z] == BlockID::Air || m_Blocks[x + 1][y][z] == BlockID::Water || m_Blocks[x + 1][y][z] == BlockID::Glass)))
					{
						if (blockType != BlockID::Water)
						{
							if (blockType == BlockID::Glass)
								m_TransparentMesh->AddFace(block->GetFace(BlockFaceID::Right), { x , y, z });
							else
								m_SolidMesh->AddFace(block->GetFace(BlockFaceID::Right), { x , y, z });
						}
					}
					if (y == 0 || (y - 1 >= 0 && (m_Blocks[x][y - 1][z] == BlockID::Air || m_Blocks[x][y - 1][z] == BlockID::Water || m_Blocks[x][y - 1][z] == BlockID::Glass)))
					{
						if (blockType != BlockID::Water)
						{
							if (blockType == BlockID::Glass)
								m_TransparentMesh->AddFace(block->GetFace(BlockFaceID::Down), { x , y, z });
							else
								m_SolidMesh->AddFace(block->GetFace(BlockFaceID::Down), { x , y, z });
						}
					}
					if (y == HEIGHT - 1 || (y + 1 < HEIGHT && (m_Blocks[x][y + 1][z] == BlockID::Air || m_Blocks[x][y + 1][z] == BlockID::Water || m_Blocks[x][y + 1][z] == BlockID::Glass)))
					{
						if (blockType != BlockID::Water)
						{
							if (blockType == BlockID::Glass)
								m_TransparentMesh->AddFace(block->GetFace(BlockFaceID::Up), { x , y, z });
							else
								m_SolidMesh->AddFace(block->GetFace(BlockFaceID::Up), { x , y, z });
						}	
					}
					if (z == 0 || (z - 1 >= 0 && (m_Blocks[x][y][z - 1] == BlockID::Air || m_Blocks[x][y][z - 1] == BlockID::Water || m_Blocks[x][y][z - 1] == BlockID::Glass)))
					{
						if (blockType != BlockID::Water)
						{
							if (blockType != BlockID::Water)
							{
								if (blockType == BlockID::Glass)
									m_TransparentMesh->AddFace(block->GetFace(BlockFaceID::Back), { x , y, z });
								else
									m_SolidMesh->AddFace(block->GetFace(BlockFaceID::Back), { x , y, z });
							}
						}
					}
					if (z == DEPTH - 1 || (z + 1 < DEPTH && (m_Blocks[x][y][z + 1] == BlockID::Air || m_Blocks[x][y][z + 1] == BlockID::Water || m_Blocks[x][y][z + 1] == BlockID::Glass)))
					{
						if (blockType != BlockID::Water)
						{
							if (blockType != BlockID::Water)
							{
								if (blockType == BlockID::Glass)
									m_TransparentMesh->AddFace(block->GetFace(BlockFaceID::Front), { x , y, z });
								else
									m_SolidMesh->AddFace(block->GetFace(BlockFaceID::Front), { x , y, z });
							}
						}
					}
					
				}
			}
		}
	}
}

void Chunk::GenerateBlocks()
{
	uint8_t heights[WIDTH][DEPTH];

	Noise noise1(111, 6, 110, 205, 0.38, 18);
	Noise noise2(111, 4, 30, 200, 0.15, 0);

	for (uint8_t x = 0; x < WIDTH; x++)
	{
		for (uint8_t z = 0; z < DEPTH; z++)
		{
			float blockGlobalX = x + m_GridPosition.x * WIDTH;
			float blockGlobalZ = z + m_GridPosition.z * DEPTH;

			float value1 = noise1.GetNoise(blockGlobalX, blockGlobalZ);
			float value2 = noise2.GetNoise(blockGlobalX, blockGlobalZ);

			float finalResult = value1 * value2;
			heights[x][z] = finalResult * noise1.GetAmplitude() + noise1.GetOffset();
		}
	}

	std::random_device device;
	std::mt19937 rng(device());
	std::uniform_int_distribution<int> treeDist(1, 100);
	std::uniform_int_distribution<int> treeHightDist(4, 6);

	
	for (uint8_t x = 0; x < WIDTH; x++)
	{
		for (uint8_t z = 0; z < DEPTH; z++)
		{
			for (uint8_t y = 0; y < HEIGHT; y++)
			{	
				if (y > heights[x][z] && y > WATER_LEVEL && m_Blocks[x][y][z] != BlockID::Wood && m_Blocks[x][y][z] != BlockID::Leaf)
					m_Blocks[x][y][z] = BlockID::Air;
				else if(y == WATER_LEVEL && y > heights[x][z])
					m_Blocks[x][y][z] = BlockID::Water;
				else if (y > heights[x][z] && y < WATER_LEVEL)
					m_Blocks[x][y][z] = BlockID::Air;
				else if (y == heights[x][z] && y < WATER_LEVEL + 2)
					m_Blocks[x][y][z] = BlockID::Sand;
				else if (y == heights[x][z])
				{
					m_Blocks[x][y][z] = BlockID::Grass;

					if (x >= 3 && x <= WIDTH - 3 && z >= 3 && z <= DEPTH - 3)
					{
						int randNum = treeDist(rng);

						if (randNum > 90)
						{
							int treeHeight = treeHightDist(rng);

							// Tree trunk
							for (int i = 1; i <= treeHeight; i++)
								m_Blocks[x][y + i][z] = BlockID::Wood;

							// Tree crown
							for (int i = x - 2; i <= x + 2; i++)
								for (int j = z - 2; j <= z + 2; j++)
								{
									m_Blocks[i][y + treeHeight + 1][j] = BlockID::Leaf;
									m_Blocks[i][y + treeHeight + 2][j] = BlockID::Leaf;
								}

							for (int i = x - 1; i <= x + 1; i++)
								for (int j = z - 1; j <= z + 1; j++)
									m_Blocks[i][y + treeHeight + 3][j] = BlockID::Leaf;

							for (int i = x - 1; i <= x + 1; i++)
								m_Blocks[i][y + treeHeight + 4][z] = BlockID::Leaf;

							for (int i = z - 1; i <= z + 1; i++)
								m_Blocks[x][y + treeHeight + 4][i] = BlockID::Leaf;
						}
					}
				}
				else if (y < heights[x][z] && y >= heights[x][z] - 5)
					m_Blocks[x][y][z] = BlockID::Dirt;
				else if (y >= 3 && y < heights[x][z] - 5)
					m_Blocks[x][y][z] = BlockID::Stone;
				else if (y < 3)
					m_Blocks[x][y][z] = BlockID::Bedrock;
			}
		}
	}
}
