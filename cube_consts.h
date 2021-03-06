#pragma once
#include "graphics.h"
#include "colors.h"

#define SIZE(Array) sizeof(Array)/sizeof(Array[0]);


const D3D::Vertex cubeVertices[] =
{
	D3D::Vertex( -50.0f, -50.0f, -50.0f, Colors::Red ),
	D3D::Vertex( -50.0f,  50.0f, -50.0f, Colors::Magenta ),
	D3D::Vertex(  50.0f,  50.0f, -50.0f, Colors::White ),
	D3D::Vertex(  50.0f, -50.0f, -50.0f, Colors::Yellow ),

	D3D::Vertex( -50.0f, -50.0f,  50.0f, Colors::Black ),
	D3D::Vertex( -50.0f,  50.0f,  50.0f, Colors::Blue ),
	D3D::Vertex(  50.0f,  50.0f,  50.0f, Colors::Cyan ),
	D3D::Vertex(  50.0f, -50.0f,  50.0f, Colors::Green ),
};
const unsigned cubeVerticesCount = SIZE(cubeVertices);

const D3D::Index cubeIndices[] =
{
	3, 0, 1,	3, 1, 2, //front side
	3, 7, 4,	3, 4, 0, //bottom side
	7, 6, 5,	7, 5, 4, //back side
	2, 1, 5,	2, 5, 6, //top side
	1, 0, 4,	1, 4, 5, //left side
	3, 2, 6,	3, 6, 7, //right side
};
const unsigned cubeIndicesCount = SIZE(cubeIndices);
#undef SIZE