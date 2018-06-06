#pragma once
#include <cstring>
#include <string>
class FHeightmap
{
public:
	FHeightmap(const std::string &Filepath, int NumberOfSamples = 256);
	FHeightmap(uint16_t* PixelData, int NumberOfSamples = 256);
	~FHeightmap();
public:
	inline int Width() const { return width; }
	inline int Height() const { return height; }
	uint16_t GetHeightAt(int x, int y) const;
	// TODO: Need to implement a height update method
	// TODO: Need to implement a file write method
private:
	uint16_t * heightData;
	long width, height;
	long numberOfSamples;
};

