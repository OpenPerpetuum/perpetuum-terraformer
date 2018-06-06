#include "stdafx.h"
#include "FPterraCore.h"

#include <cstdio>
#include <iostream>

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cout << "Filename of the terrain should be specified, using default 0000" << std::endl;
		TempVars::TerrainFile() = "C:\\PerpetuumServer\\data\\layers\\altitude.0000.bin";
	}
	else
		TempVars::TerrainFile() = argv[1];

	std::cout << "Using terrain file: " << TempVars::TerrainFile() << std::endl;
	FPterraCore* core = new FPterraCore(SCREEN_WIDTH, SCREEN_HEIGHT);

	if (!core->init())
		return -1;

	core->start();
	core->close();

	delete core;

	return 0;
}