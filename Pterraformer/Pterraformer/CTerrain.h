#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "CMesh.h"
#include "FHeightmap.h"

class CTerrain
{
public:
	CTerrain(FHeightmap* Heightmap);
	~CTerrain();
	void UpdateMesh();
	
public:
	CMesh* Mesh;
private:
	bool initialised;
	bool dirty;
	FHeightmap* heightmap;
};

