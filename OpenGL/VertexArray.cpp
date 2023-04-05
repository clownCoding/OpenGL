#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Mesh.h"

VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_RendererID);
	Bind();
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		if (element.type == GL_INT) {
			glVertexAttribIPointer(i, element.count, element.type, layout.GetStride(), (void*)offset);
			offset += element.count * element.GetTypeOfSize(element.type);
		}
		else {
			glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (void*)offset);
			offset += element.count * element.GetTypeOfSize(element.type);
		}
	}
	
	vb.Unbind();
	Unbind();
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);

}
