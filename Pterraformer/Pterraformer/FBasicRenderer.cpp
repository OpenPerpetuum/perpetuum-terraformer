#include "stdafx.h"
#include "FBasicRenderer.h"

#include <cstdio>
#include <iostream>


FBasicRenderer::FBasicRenderer(ICamera* camera)
{
	shader = new Shader();
	shader->Initialize("./Shaders/BaseVertex.vertex", "./Shaders/BaseFragment.fragment");
	this->camera = camera;
}


FBasicRenderer::~FBasicRenderer()
{
	if (shader != nullptr)
		delete shader;
}

void FBasicRenderer::SetMode(const GLenum mode)
{
	drawingMode = mode;
	
}

void FBasicRenderer::Render(std::vector<CMesh>* meshes) const
{
	shader->TurnOn();

	glm::mat4 projection, model, view;
	camera->GetMatrices(projection, view, model);

	GLint modelMatrixId = shader->GetVariable("modelMatrix");
	GLint viewMatrixId = shader->GetVariable("viewMatrix");
	GLint projectionMatrixId = shader->GetVariable("projectionMatrix");

	shader->SetMatrix4(modelMatrixId, 1, false, &model[0][0]);
	shader->SetMatrix4(viewMatrixId, 1, false, &view[0][0]);
	shader->SetMatrix4(projectionMatrixId, 1, false, &projection[0][0]);
	// If we get GPU performance problems, enable culling here. There are so few tris it shouldn't be a problem though

	for (std::vector<CMesh>::iterator mesh = meshes->begin(); mesh != meshes->end(); mesh++)
	{
		glBindVertexArray(mesh->vertexArrayObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->elementBufferObject);
		glPointSize(3.f);
		glDrawElements(drawingMode, (GLsizei)mesh->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	shader->TurnOff();
}
