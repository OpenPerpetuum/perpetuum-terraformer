#pragma once

#include "ICamera.h"
#include "Shader.h"
#include "CMesh.h"

#include <vector>

class FBasicRenderer
{
public:
	FBasicRenderer(ICamera* camera);
	~FBasicRenderer();

	void Render(std::vector<CMesh>* meshes) const;
	void SetMode(const GLenum mode);
private:
	Shader* shader;
	ICamera* camera;
	GLenum drawingMode;

	// At the moment, the below are unnecessary because I'm using Vertex Array Objects (and the VertexAttribPointer) to inject them into the shader
	// But they're here as a useful reminder to order
	static const GLuint kVertexIndex = 0;				// This is for the x,y,z vertices
	static const GLuint kNormalIndex = 1;				// This is for the x,y,z normals
	static const GLuint kTextureIndex = 2;				// This is for the u,v texture coordinates
	static const GLuint kColourIndex = -1;				// This is for the r,g,b,a colors - no colour index. I used the shader for this	
};

