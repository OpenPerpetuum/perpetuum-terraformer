#include "CMesh.h"
#include "stdafx.h"


CMesh::CMesh()
{
	Reset();
	glGenVertexArrays(1, &vertexArrayObject);
	glGenBuffers(1, &vertexBufferObject);
	glGenBuffers(1, &elementBufferObject);
}


CMesh::~CMesh()
{
	glDeleteBuffers(1, &vertexBufferObject);
	glDeleteBuffers(1, &elementBufferObject);
}

void CMesh::Reset()
{
	vertices.clear();
	indices.clear();
}

void CMesh::Bind()
{
	const GLsizei vecSize = sizeof(Vertex);
	const GLsizei indexSize = sizeof(unsigned int);

	glBindVertexArray(vertexArrayObject);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * vecSize, vertices.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * indexSize, indices.data(), GL_STATIC_DRAW);

	// Vertex Position Data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vecSize, BUFFER_OFFSET(0));
	
	// Vertex Normal Data
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, vecSize, BUFFER_OFFSET(3 * sizeof(float)));

	// Texture Coord Data
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vecSize, BUFFER_OFFSET(6 * sizeof(float)));

	glBindVertexArray(0);

}
// Performance improvement - Only upload the data that's changed. Although with the speed of PCIE this shouldn't be as important
// as the CPU bound task of mesh generation
void CMesh::Update()
{
	const GLsizei vecSize = sizeof(Vertex);

	glBindVertexArray(vertexArrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * vecSize, vertices.data());

	// Vertex Position Data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vecSize, BUFFER_OFFSET(0));

	// Vertex Normal Data
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, vecSize, BUFFER_OFFSET(3 * sizeof(float)));

	// Texture Coord Data
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vecSize, BUFFER_OFFSET(9 * sizeof(float)));

	glBindVertexArray(0);
}