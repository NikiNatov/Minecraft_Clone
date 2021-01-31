#pragma once

#include <glm\glm.hpp>

#include "Core\Base.h"
#include "Graphics\VertexArray.h"
#include "Graphics\Texture2D.h"

#include "Game\Block.h"


class Chunk
{
public:
	Chunk();
	~Chunk();

	void CreateMesh();
	void AddFace(const BlockFace& face, const glm::vec3& blockPosition);

	void OnUpdate(float dt);

	void GenerateBlocks();

	inline const Ref<VertexArray>& GetVAO() const { return m_VAO; }
public:
	static const uint8_t WIDTH = 16;
	static const uint8_t HEIGHT = 16;
	static const uint8_t DEPTH = 16;
private:
	BlockID*** m_Blocks;
	Ref<VertexArray> m_VAO;

	std::vector<Vertex> m_Vertices;
	std::vector<uint32_t> m_Indices;

	int m_IndicesCount = 0;
};