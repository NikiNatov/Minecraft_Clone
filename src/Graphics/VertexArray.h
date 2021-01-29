#pragma once

#include "Graphics\VertexBuffer.h"
#include "Graphics\IndexBuffer.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void Bind();
	void Unbind();

	void SetVertexBuffer(const Ref<VertexBuffer>& vb);
	void SetIndexBuffer(const Ref<IndexBuffer>& ib);

	inline const Ref<VertexBuffer>& GetVertexBuffer() const { return m_VertexBuffer; }
	inline const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
private:
	uint32_t m_RendererID;
	uint32_t m_AttributeIndex = 0;

	Ref<VertexBuffer> m_VertexBuffer;
	Ref<IndexBuffer> m_IndexBuffer;
};