#pragma once

#include "Graphics\VertexArray.h"
#include "Game\Terrain\Block.h"

class ChunkMesh
{
public:
	ChunkMesh();

	void AddFace(const BlockFace& face, const glm::vec3& blockPosition);
	void InitializeVAO();
	void Clear();

	inline const Ref<VertexArray>& GetVAO() const { return m_VAO; }
private:
	Ref<VertexArray> m_VAO;
	std::vector<Vertex> m_Vertices;
	std::vector<uint32_t> m_Indices;
	int m_IndicesCount = 0;
};