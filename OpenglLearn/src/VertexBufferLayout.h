#pragma once
#include <vector>
#include <glad\glad.h>

struct VertexBufferElement
{
	unsigned int count;
	unsigned int type;
	unsigned char normalized;

	static unsigned int GetSize(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
	}
};
class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement>	m_Elements;
	unsigned int m_Stride;

public:
	VertexBufferLayout()
		:m_Stride(0)
	{

	}

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({count, GL_FLOAT, GL_FALSE});
		m_Stride += count * sizeof(GLfloat);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ count, GL_UNSIGNED_INT, GL_FALSE });
		m_Stride += count * sizeof(GLuint);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ count, GL_UNSIGNED_BYTE, GL_TRUE});
		m_Stride += count * sizeof(GLbyte);
	}

	inline unsigned int GetStride() const
	{
		return m_Stride;
	}

	inline std::vector< VertexBufferElement> GetElements() const { return m_Elements; }

};
