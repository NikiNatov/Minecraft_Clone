#include "pch.h"
#include "Chunk.h"

#include <glm\gtc\noise.hpp>

Chunk::Chunk()
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

	// Temp
	for (uint8_t x = 0; x < WIDTH; x++)
	{
		for (uint8_t y = 0; y < HEIGHT; y++)
		{
			for (uint8_t z = 0; z < DEPTH; z++)
			{
				m_Blocks[x][y][z] = BlockID::Air;
			}
		}
	}

	GenerateBlocks();
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
					}

					if (x == 0 || (x - 1 >= 0 && m_Blocks[x - 1][y][z] == BlockID::Air))
						AddFace(block->GetLeftFace(), {x , y, z});
					if (x == WIDTH - 1 || (x + 1 < WIDTH && m_Blocks[x + 1][y][z] == BlockID::Air))
						AddFace(block->GetRightFace(), { x , y, z });
					if (y == 0 || (y - 1 >= 0 && m_Blocks[x][y - 1][z] == BlockID::Air))
						AddFace(block->GetDownFace(), { x , y, z });
					if (y == HEIGHT - 1 || (y + 1 < HEIGHT && m_Blocks[x][y + 1][z] == BlockID::Air))
						AddFace(block->GetUpFace(), { x , y, z });
					if (z == 0 || (z - 1 >= 0 && m_Blocks[x][y][z - 1] == BlockID::Air))
						AddFace(block->GetBackFace(), { x , y, z });
					if (z == DEPTH - 1 || (z + 1 < DEPTH && m_Blocks[x][y][z + 1] == BlockID::Air))
						AddFace(block->GetFrontFace(), { x , y, z });
					
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
	m_VAO->SetVertexBuffer(vb);
	m_VAO->SetIndexBuffer(ib);
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

void Chunk::OnUpdate(float dt)
{
}

void Chunk::GenerateBlocks()
{
	uint8_t heights[WIDTH][DEPTH];

	for (uint8_t x = 0; x < WIDTH; x++)
	{
		for (uint8_t z = 0; z < DEPTH; z++)
		{
			float center = glm::perlin(glm::vec2(x / 8.0, z / 8.0));
			heights[x][z] = (1.0f - center) * 0.5f * HEIGHT;
		}
	}

	for (uint8_t x = 0; x < WIDTH; x++)
	{
		for (uint8_t z = 0; z < DEPTH; z++)
		{
			for (uint8_t y = 0; y < HEIGHT; y++)
			{
				
				if (y == heights[x][z])
					m_Blocks[x][y][z] = BlockID::Grass;
				else if (y < heights[x][z] && y >= heights[x][z] - 5)
					m_Blocks[x][y][z] = BlockID::Dirt;
				else if( y < heights[x][z] - 5)
					m_Blocks[x][y][z] = BlockID::Stone;
			}
		}
	}
}
