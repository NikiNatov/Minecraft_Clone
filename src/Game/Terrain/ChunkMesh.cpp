#include "pch.h"
#include "ChunkMesh.h"

ChunkMesh::ChunkMesh()
{
}

void ChunkMesh::AddFace(const BlockFace& face, const glm::vec3& blockPosition)
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

void ChunkMesh::InitializeVAO()
{
	Ref<VertexBuffer> vb = CreateRef<VertexBuffer>(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
	vb->SetLayout({
			{"a_Position", DataType::Float3, false},
			{"a_TexCoords", DataType::Float2, false},
			{"a_Normal", DataType::Float3, false},
			{"a_LightLevel", DataType::Float, false}
		});

	Ref<IndexBuffer> ib = CreateRef<IndexBuffer>(m_Indices.data(), m_Indices.size());

	m_VAO = CreateRef<VertexArray>();
	m_VAO->Bind();
	m_VAO->SetVertexBuffer(vb);
	m_VAO->SetIndexBuffer(ib);
	m_VAO->Unbind();
}

void ChunkMesh::Clear()
{
	m_Vertices.clear();
	m_Indices.clear();
	m_IndicesCount = 0;
	m_VAO.reset();
}
