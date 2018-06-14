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

	glm::mat4 projection, model, view, mvp;
	camera->GetMatrices(projection, view, model, mvp);

	GLint modelMatrixId = shader->GetVariable("modelMatrix");
	shader->SetMatrix4(modelMatrixId, 1, false, &model[0][0]);

	GLint mvpMatrixId = shader->GetVariable("mvpMatrix");
	shader->SetMatrix4(mvpMatrixId, 1, false, &mvp[0][0]);

	GLint normalMatrixId = shader->GetVariable("normalMatrix");
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
	shader->SetMatrix3(normalMatrixId, 1, false, &normalMatrix[0][0]);

	shader->SetUniform("light.position", glm::vec3(0.f, 10000.f, 0.f));
	shader->SetUniform("light.intensity", glm::vec3(1, 1, 1)); // White light
	shader->SetUniform("light.ambientCoefficient", glm::vec3(0.3, 0.3, 0.3));
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
