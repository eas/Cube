#pragma once
#include "d3dx9.h"

namespace D3D
{

	inline const D3DXMATRIX ScaleMatrix()
	{
		return D3DXMATRIX( scale.x,		0.0f,		0.0f,		0.0f,
						   0.0f,		scale.y,	0.0f,		0.0f,
						   0.0f,		0.0f,		scale.z,	0.0f,
						   0.0f,		0.0f,		0.0f,		1.0f );
	}

	inline const D3DXMATRIX TranslationMatrix
	{
		return D3DXMATRIX( 1.0f,			0.0f,			0.0f,			0.0f,
						   0.0f,			1.0f,			0.0f,			0.0f,
						   0.0f,			0.0f,			1.0f,			0.0f,
						   translation.x,	translation.y,	translation.z,	1.0f );
	}

	inline const D3DXMATRIX RotateZMatrix
	{
		return D3DXMATRIX(  cosf(angle),	sinf(angle),	0.0f,		0.0f,
						   -sinf(angle),	cosf(angle),	0.0f,		0.0f,
							0.0f,			0.0f,			1.0f,		0.0f,
							0.0f,			0.0f,			0.0f,		1.0f );
	}

	inline const D3DXMATRIX RotateYMatrix
	{
		return D3DXMATRIX(  cosf(angle),	0.0f,		-sinf(angle),	0.0f,
							0.0f,			1.0f,		0.0f,			0.0f,
							sinf(angle),	0.0f,		cosf(angle),	0.0f,
							0.0f,			0.0f,		0.0f,			1.0f );
	}

	inline const D3DXMATRIX RotateXMatrix
	{
		return D3DXMATRIX(  1.0f,		0.0f,			0.0f,			0.0f,
							0.0f,		cosf(angle),	sinf(angle),	0.0f,
							0.0f,		-sinf(angle),	cosf(angle),	0.0f,
							0.0f,		0.0f,			0.0f,			1.0f );

