#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	// Generate VAO 
	GLCall(glGenVertexArrays(1, &m_RendereID));
   // Bind it so that rest of vao operations affect this vao 
	
}
VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendereID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	// Bind vertex array
	Bind();
	// Bind vertex buffer
	vb.Bind();
	//Set the layout
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];

		glEnableVertexAttribArray(i);
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));

		offset += element.count * VertexBufferElement::GetSize(element.type);
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendereID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}