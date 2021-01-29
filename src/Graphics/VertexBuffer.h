#pragma once

#include "Core\Base.h"

#include <glad\glad.h>

#include <vector>
#include <string>

enum class DataType
{
	None = 0,
	Int, Int2, Int3, Int4,
	Float, Float2, Float3, Float4,
	Bool
};

struct BufferElement
{
	std::string Name;
	DataType Type;
	bool Normalized;
	uint32_t Offset;

	BufferElement(const std::string& name, DataType dataType, bool normalized)
		: Name(name), Type(dataType), Normalized(normalized)
	{
	}

	uint32_t GetComponentCount() const
	{
		switch (Type)
		{
			case DataType::Int:		return 1;
			case DataType::Int2:	return 2;
			case DataType::Int3:	return 3;
			case DataType::Int4:	return 4;
			case DataType::Float:	return 1;
			case DataType::Float2:	return 2;
			case DataType::Float3:	return 3;
			case DataType::Float4:	return 4;
			case DataType::Bool:	return 1;
		}

		ASSERT(false, "Invalid data type!");
		return 0;
	}

	uint32_t GetElementSize() const
	{
		switch (Type)
		{
			case DataType::Int:		return 1 * 4;
			case DataType::Int2:	return 2 * 4;
			case DataType::Int3:	return 3 * 4;
			case DataType::Int4:	return 4 * 4;
			case DataType::Float:	return 1 * 4;
			case DataType::Float2:	return 2 * 4;
			case DataType::Float3:	return 3 * 4;
			case DataType::Float4:	return 4 * 4;
			case DataType::Bool:	return 1;
		}

			ASSERT(false, "Invalid data type!");
			return 0;
	}
};

class BufferLayout
{
public:
	BufferLayout() = default;

	BufferLayout(const std::initializer_list<BufferElement>& elementList)
		: m_Elements(elementList)
	{
		CalculateStrideAndOffsets();
	}

	inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
	inline uint32_t GetStride() const { return m_Stride; }

	std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
	std::vector<BufferElement>::const_iterator begin() const { return m_Elements.cbegin(); }
	std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
	std::vector<BufferElement>::const_iterator end() const { return m_Elements.cend(); }
private:
	void CalculateStrideAndOffsets()
	{
		uint32_t offset = 0;
		for (auto& element : m_Elements)
		{
			element.Offset = offset;
			m_Stride += element.GetElementSize();
			offset += element.GetElementSize();
		}
	}
private:
	std::vector<BufferElement> m_Elements;
	uint32_t m_Stride = 0;
};

class VertexBuffer
{
public:
	VertexBuffer(const void* data, uint32_t size);
	~VertexBuffer();

	void Bind();
	void Unbind();

	inline void SetLayout(const BufferLayout& layout) { m_Layout = layout; }
	inline const BufferLayout& GetLayout() const { return m_Layout; }
private:
	uint32_t m_RendererID;
	BufferLayout m_Layout;
};