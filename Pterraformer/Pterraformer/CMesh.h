#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Vertex.hpp"

class CMesh
{
public:
	CMesh();
	~CMesh();

	void Reset();
	void Bind();
	void Update();
public:
	unsigned int vertexBufferObject;
	unsigned int vertexArrayObject;
	unsigned int elementBufferObject;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};

