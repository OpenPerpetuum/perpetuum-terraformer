#include "stdafx.h"
#include "FHeightmap.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <math.h>

int clampSamples(int upper, int value)
{
	return std::min(upper, std::max(value, 0));
}

FHeightmap::FHeightmap(const std::string &Filename, int NumberOfSamples)
{
	std::ifstream file(Filename, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	
	int64_t indexLength = size;
	file.seekg(0, std::ios::beg);
	
	int64_t pixelSize = indexLength / 2;
	long mapSize = std::lround(sqrt(pixelSize));

	// Perpetuum maps are always square (for now)
	width = mapSize;
	height = mapSize;

	char* buffer = new char[size];
	if (!file.read(buffer, size))
		throw std::runtime_error("Unable to read specified file");
	
	file.close();
	// Pixel height data is given in 16 bit format (ushort)
	// Interestingly enough, the actual height, according to Perpetuum, is point >> 5 (max value 2047)
	// instead of using the whole 16 bits (65,535)
	heightData = reinterpret_cast<uint16_t*>(buffer);

	// For now number of samples doesn't do anything and the heightmap will always be 1:1
	// Leave this in though as it may be useful for creating "quick and dirty" heightmap runs
	numberOfSamples = clampSamples(height, NumberOfSamples);
}

FHeightmap::FHeightmap(uint16_t* heightmapData, int NumberOfSamples)
{
	heightData = heightmapData;
	width = std::lround(sqrt(sizeof(heightData)));
	height = width;

	numberOfSamples = clampSamples(height, NumberOfSamples);
}


FHeightmap::~FHeightmap()
{
	if (heightData != nullptr)
	{
		delete[] heightData;
		heightData = nullptr;
	}
}

uint16_t FHeightmap::GetHeightAt(int x, int y) const
{
	if (x >= width || y >= height)
		throw std::invalid_argument("X and Y value must be within the bounds of the map data");

	return heightData[y * width + x];
}
