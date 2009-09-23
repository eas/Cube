#pragma once
#include "graphics.h"

#define SIZE(Array) sizeof(Array)/sizeof(Array[0]);

D3D::Vertex cubeVertices[] =
{
	{ -50.0f, -50.0f, -50.0f, Red },
	{ -50.0f,  50.0f, -50.0f, Magenta },
	{  50.0f,  50.0f, -50.0f, White },
	{  50.0f, -50.0f, -50.0f, Yellow },

	{ -50.0f, -50.0f,  50.0f, Black },
	{ -50.0f,  50.0f,  50.0f, Blue },
	{  50.0f,  50.0f,  50.0f, Cyan },
	{  50.0f, -50.0f,  50.0f, Green },
};
UINT cubeVerticesCount = SIZE(cubeVertices);

UINT cubeIndices[] =
{
	3, 0, 1,	3, 1, 2, //front side
	3, 7, 4,	3, 4, 0, //bottom side
	7, 6, 5,	7, 5, 4, //back side
	2, 1, 5,	2, 5, 6, //top side
	1, 0, 4,	1, 4, 5, //left side
	3, 2, 6,	3, 6, 7, //right side
};
UINT cubeIndicesCount = SIZE(cubeIndices);
#undef SIZE