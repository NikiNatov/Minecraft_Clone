#include "pch.h"
#include "VertexArray.h"

static GLenum DataTypeToGLType(DataType type)
{
	switch (type)
	{
		case DataType::Int:		return GL_INT;
		case DataType::Int2:	return GL_INT;
		case DataType::Int3:	return GL_INT;
		case DataType::Int4:	return GL_INT;
		case DataType::Float:	return GL_FLOAT;
		case DataType::Float2:	return GL_FLOAT;
		case DataType::Float3:	return GL_FLOAT;
		case DataType::Float4:	return GL_FLOAT;
		case DataType::Bool:	return GL_BOOL;
	}

	ASSERT(false, "Invalid data type!");
	return 0;
}

VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind()
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}

void VertexArray::SetVertexBuffer(const Ref<VertexBuffer>& vb)
{
	m_VertexBuffer = vb;
	const BufferLayout& layout = m_VertexBuffer->GetLayout();

	glBindVertexArray(m_RendererID);
	vb->Bind();

	for (auto& element : layout)
	{
		glEnableVertexAttribArray(m_AttributeIndex);
		switch (element.Type)
		{
			case DataType::Int:
			case DataType::Int2:
			case DataType::Int3:
			case DataType::Int4:
			case DataType::Bool:
				glVertexAttribIPointer(m_AttributeIndex, element.GetComponentCount(), DataTypeToGLType(element.Type), layout.GetStride(), (void*)element.Offset);
				break;
			default:
				glVertexAttribPointer(m_AttributeIndex, element.GetComponentCount(), DataTypeToGLType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (void*)element.Offset);
				break;
		}

		m_AttributeIndex++;
	}
}

void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& ib)
{
	glBindVertexArray(m_RendererID);
	ib->Bind();
	m_IndexBuffer = ib;
}
