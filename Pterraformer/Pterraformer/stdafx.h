#pragma once
// Why isn't this a precompiled header? Because glad.c throws a hissy fit if you include it.
// Remember to include this file in all applicable sources because there will be no compiler hints.

//#include <vld.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <cstdarg>
#include <cstdio>

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#define GLM_FORCE_RADIANS 1
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define DEG_TO_RAD(deg) ((float)(deg * M_PI) / 180)
#define RAD_TO_DEG(rad) ((float)(rad * 180) / M_PI)

#define NEAR_PLANE 0.01f
#define FOV 45.f

#define BUFFER_OFFSET(i) ((void*)(i))

#include <cstring>
#include <string>
// Placeholder until the UX is in
class TempVars {
public:
	static std::string& TerrainFile() { static std::string terrainFile; return terrainFile; }
};