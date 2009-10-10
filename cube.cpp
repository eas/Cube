// lesson1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "cube.h"

#include "graphics.h"
#include "internals.h"

#include <d3dx9.h>
#include "matrices.h"
#include "colors.h"

#include "cube_consts.h"
#include <tchar.h>



const LPCTSTR ShaderFileName = _T("shader.vsh");


const D3D::Vertex* const vertices = cubeVertices;
const D3D::Index* const indices = cubeIndices;
const unsigned IndicesCount = cubeIndicesCount;
const unsigned VerticesCount = cubeVerticesCount;

const float FrontClippingPlane = 0.5f;
const float BackClippingPlane = 1.0e13f;
const int VerticesInTriangle = 3;

const float InitialR = 150.0f;
const float InitialTheta = D3DX_PI / 2;
const float InitialFi = -D3DX_PI / 2;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void Render(D3D::GraphicDevice& device)
{
	D3D::GraphicDevice::DC dc( device, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, Colors::Gray, 1.0f, 0 );
	dc.DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, VerticesCount, 0, IndicesCount/VerticesInTriangle );
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE ,
                     LPTSTR    ,
                     int       nCmdShow)
{

	Window mainWindow(hInstance, nCmdShow, &WndProc);

	D3DPRESENT_PARAMETERS params;
		ZeroMemory( &params, sizeof( params ) );

		params.Windowed = TRUE;
		params.SwapEffect = D3DSWAPEFFECT_DISCARD;
		params.BackBufferFormat = D3DFMT_UNKNOWN;
		params.EnableAutoDepthStencil = TRUE;
		params.AutoDepthStencilFormat = D3DFMT_D16;
		params.MultiSampleType = D3DMULTISAMPLE_NONE;

	//const float pointSize = 1.3098e-316f;
	D3D::GraphicDevice graphicDevice( mainWindow.GetHWND(), params );
	//graphicDevice.SetRenderState(D3DRS_POINTSIZE_MAX, *((DWORD*)&pointSize));



	D3D::VertexDeclaration vertexDeclaration(graphicDevice);
	vertexDeclaration.Use();

	D3D::Shader shader(graphicDevice, ShaderFileName);
	shader.Use();

	D3D::VertexBuffer vertexBuffer(graphicDevice, VerticesCount);
	vertexBuffer.SetVertices(vertices, VerticesCount);
	vertexBuffer.Use(0,0);

	D3D::IndexBuffer indexBuffer(graphicDevice, IndicesCount);
	indexBuffer.SetIndices(indices, IndicesCount);
	indexBuffer.Use();

	SpectatorCoords spectatorCoords( InitialR, InitialTheta, InitialFi );

	shader.SetWorldMatrix( UnityMatrix() );
	shader.SetProjectiveMatrix( ProjectiveMatrix(FrontClippingPlane, BackClippingPlane) );
	shader.SetViewMatrix(ViewMatrix(	spectatorCoords.GetCartesianCoords(),
										D3DXVECTOR3(0.0f, 0.0f, 0.0f),
										D3DXVECTOR3(0.0f, 1.0f, 0.0f) ));


	SetWindowLong(mainWindow.GetHWND(), 0, reinterpret_cast<LONG>(&spectatorCoords));
	SetWindowLong(mainWindow.GetHWND(), sizeof(LONG), reinterpret_cast<LONG>(&shader));

	MSG msg;
	
	ZeroMemory(&msg, sizeof(msg));
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
            Render(graphicDevice);
    }
	return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		{
			D3D::Shader* pShader = NULL;
			SpectatorCoords* pSpectatorCoords = NULL;
			pSpectatorCoords = reinterpret_cast<SpectatorCoords*>(GetWindowLong(hWnd, 0));
			pShader = reinterpret_cast<D3D::Shader*>(GetWindowLong(hWnd, sizeof(LONG)));

			switch(wParam)
			{
			case VK_UP:
				pSpectatorCoords->DecTheta();
				break;
			case VK_DOWN:
				pSpectatorCoords->IncTheta();
				break;
			case VK_RIGHT:
				pSpectatorCoords->IncPhi();
				break;
			case VK_LEFT:
				pSpectatorCoords->DecPhi();
				break;
			case VK_NEXT:
			case 'S':
				pSpectatorCoords->IncR();
				break;
			case VK_PRIOR:
			case 'W':
				pSpectatorCoords->DecR();
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			pShader->SetViewMatrix(ViewMatrix(	pSpectatorCoords->GetCartesianCoords(),
												D3DXVECTOR3(0.0f, 0.0f, 0.0f),
												D3DXVECTOR3(0.0f, 1.0f, 0.0f) ));
			break;
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

