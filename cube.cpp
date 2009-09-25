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

class SpectatorCoords
{
public:
	SpectatorCoords(float r, float theta, float fi)
		:r_(r), theta_(theta), fi_(fi)
	{
	}
	const D3DXVECTOR3 GetCartesianCoords() const
	{
		return D3DXVECTOR3(	r_*sinf(theta_)*cosf(fi_),
							r_*cosf(theta_),
							r_*sinf(theta_)*sinf(fi_) );
	}
	void IncTheta() { if((theta_+=deltaTheta) > thetaMax) theta_= thetaMax; }
	void DecTheta() { if((theta_-=deltaTheta) < thetaMin) theta_= thetaMin; }
	void IncFi() { fi_ += deltaFi; }
	void DecFi() { fi_ -= deltaFi; }
	void IncR() { r_ += deltaR; }
	void DecR() { if((r_-=deltaR) < rMin) r_ = rMin; }


private:
	float r_, theta_, fi_;

public:
	static const float deltaFi;
	static const float deltaTheta;
	static const float deltaR;
	static const float thetaMin;
	static const float thetaMax;
	static const float rMin;
};
const float SpectatorCoords::deltaFi = D3DX_PI / 24;
const float SpectatorCoords::deltaTheta = D3DX_PI / 24;
const float SpectatorCoords::deltaR = 10.0f;
const float SpectatorCoords::thetaMin = 1e-3f;
const float SpectatorCoords::thetaMax = D3DX_PI - thetaMin;
const float SpectatorCoords::rMin = 0.1f;

const int MaxLoadString = 100;
const int WindowHeight = 500;
const int WindowWidth = WindowHeight;
const int WindowPostionX = 350;
const int WindowPostionY = 120;

const LPCTSTR ShaderFileName = L"shader.vsh";


const D3D::Vertex* const vertices = cubeVertices;
const UINT* const indices = cubeIndices;
const UINT indicesCount = cubeIndicesCount;
const UINT verticesCount = cubeVerticesCount;



// Global Variables:
TCHAR szTitle[MaxLoadString];					// The title bar text
TCHAR szWindowClass[MaxLoadString];				// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
HWND				CreateMainWindow(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

void Render(D3D::GraphicDevice& device)
{
	D3D::GraphicDevice::DC dc( device, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, Gray, 1.0f, 0 );
	dc.DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, verticesCount, 0, indicesCount/3 );
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE ,
                     LPTSTR    ,
                     int       nCmdShow)
{
 	// TODO: Place code here.

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MaxLoadString);
	LoadString(hInstance, IDC_CUBE, szWindowClass, MaxLoadString);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	HWND mainWindow = CreateMainWindow(hInstance, nCmdShow);

	D3DPRESENT_PARAMETERS params;
		ZeroMemory( &params, sizeof( params ) );

		params.Windowed = TRUE;
		params.SwapEffect = D3DSWAPEFFECT_DISCARD;
		params.BackBufferFormat = D3DFMT_UNKNOWN;
		params.EnableAutoDepthStencil = TRUE;
		params.AutoDepthStencilFormat = D3DFMT_D16;
		params.MultiSampleType = D3DMULTISAMPLE_NONE;

	//const float pointSize = 1.3098e-316f;
	D3D::GraphicDevice graphicDevice( mainWindow, params );
	//graphicDevice.SetRenderState(D3DRS_POINTSIZE_MAX, *((DWORD*)&pointSize));



	D3D::VertexDeclaration vertexDeclaration(graphicDevice);
	vertexDeclaration.Use();

	D3D::Shader shader(graphicDevice, ShaderFileName);
	shader.Use();

	D3D::VertexBuffer vertexBuffer(graphicDevice, verticesCount);
	vertexBuffer.SetVertices(vertices, verticesCount);
	vertexBuffer.Use(0,0);

	D3D::IndexBuffer indexBuffer(graphicDevice, indicesCount);
	indexBuffer.SetIndices(indices, indicesCount);
	indexBuffer.Use();

	SpectatorCoords spectatorCoords( 150.0f, D3DX_PI / 2, -D3DX_PI / 2 );

	shader.SetWorldMatrix( UnityMatrix() );
	shader.SetProjectiveMatrix( ProjectiveMatrix(0.5f, 1.0e+15f) );
	shader.SetViewMatrix(ViewMatrix(	spectatorCoords.GetCartesianCoords(),
										D3DXVECTOR3(0.0f, 0.0f, 0.0f),
										D3DXVECTOR3(0.0f, 1.0f, 0.0f) ));


	SetWindowLong(mainWindow, 0, reinterpret_cast<LONG>(&spectatorCoords));
	SetWindowLong(mainWindow, sizeof(LONG), reinterpret_cast<LONG>(&shader));

	MSG msg;
	
	ZeroMemory(&msg, sizeof(msg));
    while( msg.message != WM_QUIT )
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



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= sizeof(LONG) * 2;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CUBE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;/*MAKEINTRESOURCE(IDC_CUBE);*/
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}


HWND CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   WindowPostionX, WindowPostionY, WindowWidth, WindowHeight, NULL, NULL, hInstance, NULL);

   if (!hWnd)
	   throw WinApiError( GetLastError() );

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return hWnd;
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
				pSpectatorCoords->IncFi();
				break;
			case VK_LEFT:
				pSpectatorCoords->DecFi();
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

