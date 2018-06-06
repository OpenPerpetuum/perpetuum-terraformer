#pragma once
// Packed vertex to reduce the memory cost on the GPU
// It's worth noting that with all the required drivers and data points loaded
// a single 8MB map consumes ~300MB of RAM (48MB of Vertices + ~246MB of index data) which is transferred to the GFX card.

// Aligned on 4 and 32 byte boundary due to 8 * 4 bytes, if modifying, keep aligned and most importantly, RETAIN ORDER
// OpenGL will access this data based on instructions in the VertexAttribPointer
// If necessary due to memory access performance problems on the GPU, align each subset of data to 16 bytes and update the CMesh
// VertexAttribPointer stride parameter as necessary.

// Pragma is here to make sure MSVC doesn't do something weird
#pragma pack(1)
struct Vertex
{
	Vertex()
	{
		x = y = z = nx = ny = nz = u = v = 0;
	};
	Vertex(float inX, float inY, float inZ)
	{
		x = inX;
		y = inY;
		z = inZ;
		nx = ny = nz = u = v = 0;
	}
	float x, y, z;
	float nx, ny, nz;
	float u, v;
};
#pragma pack()

