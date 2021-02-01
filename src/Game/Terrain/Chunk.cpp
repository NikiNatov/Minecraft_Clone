#include "pch.h"
#include "Chunk.h"

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
			m_Blocks[x][y] = new BlockID[DEPTH];
		}
	}

	GenerateBlocks();
	CreateMesh();
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
}

void Chunk::OnUpdate(float dt)
{
}

void Chunk::CreateMesh()
{
	for (uint8_t x = 0; x < WIDTH; x++)
	{
		for (uint8_t y = 0; y < HEIGHT; y++)
		{
			for (uint8_t z = 0; z < DEPTH; z++)
			{
				if (m_Blocks[x][y][z] != BlockID::Air)
				{
					const Block* block = nullptr;

					switch (m_Blocks[x][y][z])
					{
						case BlockID::Grass:	block = Block::s_GrassBlock.get(); break;
						case BlockID::Dirt:		block = Block::s_DirtBlock.get(); break;
						case BlockID::Stone:	block = Block::s_StoneBlock.get(); break;
						case BlockID::Bedrock:	block = Block::s_BedrockBlock.get(); break;
						case BlockID::Water:	block = Block::s_WaterBlock.get(); break;
						case BlockID::Sand:		block = Block::s_SandBlock.get(); break;
					}

					if (x == 0 || (x - 1 >= 0 && m_Blocks[x - 1][y][z] == BlockID::Air))
						AddFace(block->GetFace(BlockFaceID::Left), {x , y, z});
					if (x == WIDTH - 1 || (x + 1 < WIDTH && m_Blocks[x + 1][y][z] == BlockID::Air))
						AddFace(block->GetFace(BlockFaceID::Right), { x , y, z });
					if (y == 0 || (y - 1 >= 0 && m_Blocks[x][y - 1][z] == BlockID::Air))
						AddFace(block->GetFace(BlockFaceID::Down), { x , y, z });
					if (y == HEIGHT - 1 || (y + 1 < HEIGHT && m_Blocks[x][y + 1][z] == BlockID::Air))
						AddFace(block->GetFace(BlockFaceID::Up), { x , y, z });
					if (z == 0 || (z - 1 >= 0 && m_Blocks[x][y][z - 1] == BlockID::Air))
						AddFace(block->GetFace(BlockFaceID::Back), { x , y, z });
					if (z == DEPTH - 1 || (z + 1 < DEPTH && m_Blocks[x][y][z + 1] == BlockID::Air))
						AddFace(block->GetFace(BlockFaceID::Front), { x , y, z });
					
				}
			}
		}
	}

	Ref<VertexBuffer> vb = CreateRef<VertexBuffer>(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
	vb->SetLayout(
	{
			{"a_Position", DataType::Float3, false},
			{"a_TexCoords", DataType::Float2, false},
			{"a_Normal", DataType::Float3, false},
	});

	Ref<IndexBuffer> ib = CreateRef<IndexBuffer>(m_Indices.data(), m_Indices.size());

	m_VAO = CreateRef<VertexArray>();
	m_VAO->Bind();
	m_VAO->SetVertexBuffer(vb);
	m_VAO->SetIndexBuffer(ib);
	m_VAO->Unbind();
}

void Chunk::AddFace(const BlockFace& face, const glm::vec3& blockPosition)
{
	for (int i = 0; i < 4; i++)
	{
		Vertex v = face.Vertices[i];
		v.Position += blockPosition;
		m_Vertices.push_back(v);
	}

	m_Indices.push_back(m_IndicesCount);
	m_Indices.push_back(m_IndicesCount + 1);
	m_Indices.push_back(m_IndicesCount + 2);
	m_Indices.push_back(m_IndicesCount + 2);
	m_Indices.push_back(m_IndicesCount + 3);
	m_Indices.push_back(m_IndicesCount);
	m_IndicesCount += 4;
}

void Chunk::GenerateBlocks()
{
	uint8_t heights[WIDTH][DEPTH];

	Noise noise1(1231, 6, 110, 205, 0.38, 18);
	Noise noise2(1231, 4, 30, 200, 0.15, 0);

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

	for (uint8_t x = 0; x < WIDTH; x++)
	{
		for (uint8_t z = 0; z < DEPTH; z++)
		{
			for (uint8_t y = 0; y < HEIGHT; y++)
			{	
				if(y > heights[x][z] && y < WATER_LEVEL)
					m_Blocks[x][y][z] = BlockID::Water;
				else if (y == heights[x][z] && y < WATER_LEVEL + 2)
					m_Blocks[x][y][z] = BlockID::Sand;
				else if (y == heights[x][z])
					m_Blocks[x][y][z] = BlockID::Grass;
				else if (y < heights[x][z] && y >= heights[x][z] - 5)
					m_Blocks[x][y][z] = BlockID::Dirt;
				else if (y >= 3 && y < heights[x][z] - 5)
					m_Blocks[x][y][z] = BlockID::Stone;
				else if (y < 3)
					m_Blocks[x][y][z] = BlockID::Bedrock;
				else 
					m_Blocks[x][y][z] = BlockID::Air;
			}
		}
	}
}
