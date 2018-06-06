#include "stdafx.h"
#include "CTerrain.h"



CTerrain::CTerrain(FHeightmap* Heightmap)
{
	heightmap = Heightmap;
	Mesh = new CMesh();
	Mesh->Reset(); // Allocate memory now and get it out of the way
	initialised = false;
	dirty = true;
	UpdateMesh();
}


CTerrain::~CTerrain()
{
	if (Mesh)
	{
		delete Mesh;
		Mesh = nullptr;
	}
}

void CTerrain::UpdateMesh()
{
	if (!dirty) return;

	int width = heightmap->Width();
	int height = heightmap->Height();

	// Permance Improvement A: Only update the vertices where the heightmap has changed
	// -- Without quad raytrace collision detection and sectional updating, this is not possible within this methof
	// Performance Improvement B: Update each row in a Thread. (Similar to C# ThreadQueue)
	// -- Option A is by far the best solution in terms of elegence and performance,
	// but Option B is easy to implement and requires no underlying framework changes.
	// Performance Improvement C: Implement the entire thing by passing the heightmap into the shader.
	// -- After looking into something else, this option came up. http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.161.8979&rep=rep1&type=pdf
	// and may be the single most performant way to implement terrain manipulation. The problem is that the heightmap is not a natural "texture"
	// and may not yield expected results.

	// Heightmap is 'point-sampled'. There is no vertex. Get the heightmap value and build a
	// vertex buffer. 2048 * 2048 = 4,194,304 * 2 (unsigned short == 2 bytes) = 8,388,608 === 8MB. This is the file size we all know and love :)
	// Float is 4 bytes and we need three for X, Y, and Z: ((2048 * 2048) * 4) * 3 = 50,331,684 (48MB)
	// For lighting, each vertex requires a normal. Which is the same calculation again: ((2048 * 2048) * 4) * 3 = 50,331,684 (48MB)
	// So for vertex data *alone* that would require uploaded 96MB of data to the GPU whilst also getting the CPU to do all this
	// floating point math that it's, subjectively, not very good at.
	// For this reason, I'm tempted NOT to pre-generate the normals, but instead to use the vertex shader and compute them
	// every frame. It should be fairly fast given todays processing power on the GPU.

	Mesh->vertices.resize(width * height);
	Mesh->indices.resize((width - 1) * (width - 1) * 6);
	
	int vertIndex = 0;
	int indicesIndex = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			float newY = (float)heightmap->GetHeightAt(x, y);
			float newX = x * 100.f;
			float newZ = y * 100.f;
			Mesh->vertices[vertIndex++] = Vertex(newX, newY, newZ);
		}
	}
	
	// Build the element index - only do this once per mesh as it is not destructable
	// Why do this? Because when the mesh gets updated, you're changing an 48MB dataset, not a 300MB dataset :)
	if (!initialised)
	{
		// Stop wrap-around and over-flow on tris
		int maxRow = width - 2;
		int maxCol = height - 2;
		for (int c = 0; c <= maxCol; c++)
		{
			for (int r = 0; r <= maxRow; r++)
			{

				// Build the element index
				// Each iteration consists of a quad (two triangles)
				// that must be built at the same time - A B C B D B
				unsigned int start = c * width + r;
				Mesh->indices[indicesIndex++] = start;
				Mesh->indices[indicesIndex++] = start + width;
				Mesh->indices[indicesIndex++] = start + 1;
				Mesh->indices[indicesIndex++] = start + width;
				Mesh->indices[indicesIndex++] = start + width + 1;
				Mesh->indices[indicesIndex++] = start + 1;
			}
		}
		initialised = true;
		Mesh->Bind();
	}
	else
		Mesh->Update();
	
	dirty = false;
}
