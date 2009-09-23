// lesson1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "lesson1.h"

#include "graphics.h"
#include "internals.h"

#include <d3dx9.h>
#include "matrices.h"
#include "colors.h"

#include "cube.h"

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
	void IncTheta() { if(theta_+=deltaTheta > thetaMax) theta_= thetaMax; }
	void DecTheta() { if(theta_-=deltaTheta < thetaMin) theta_= thetaMin; }
	void IncFi() { fi_ += deltaFi; }
	void DecFi() { fi_ -= deltaFi; }
	void IncR() { r_ += deltaR; }
	void DecR() { if(r_-=deltaR < rMin) r_ = rMin; }


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
const float SpectatorCoords::deltaFi = D3DX_PI / 8;
const float SpectatorCoords::deltaTheta = D3DX_PI / 8;
const float SpectatorCoords::deltaR = 10.0f;
const float SpectatorCoords::thetaMin = 1e-12f;
const float SpectatorCoords::thetaMax = D3DX_PI - thetaMin;
const float SpectatorCoords::rMin = 0.0f;

const int maxLoadString = 100;
const int WindowHeight = 500;
const int WindowWidth = WindowHeight;
const int WindowPostionX = 350;
const int WindowPostionY = 120;



D3D::Vertex* vertices = cubeVertices;
UINT* indices = cubeIndices;
UINT indicesCount = cubeIndicesCount;
UINT verticesCount = cubeVerticesCount;



// Global Variables:
TCHAR szTitle[maxLoadString];					// The title bar text
TCHAR szWindowClass[maxLoadString];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
HWND				CreateMainWindow(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

void Render(D3D::GraphicDevice& device, D3D::VertexBuffer& vb, 
			D3D::IndexBuffer& ib, D3D::Shader& shader, D3D::VertexDeclaration& vd, SpectatorCoords& spCoords)
{
	//shader.SetShaderMatrix();
	//static int i=1;
	//if( ++i % 100 == 0 )
	//{
	//	spCoords.IncFi();
	//	shader.SetViewMatrix(ViewMatrix( spCoords.GetCartesianCoords(),
	//									 D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	//									 D3DXVECTOR3(0.0f, 1.0f, 0.0f) ));
	//	//shader.Use();
	//	//vb.Use(0,0);
	//	//ib.Use();
	//	//vd.Use();
	//}
	D3D::GraphicDevice::DC dc( device, D3DCLEAR_TARGET, Gray, 1.0f, 0 );
	device.DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, verticesCount, 0, indicesCount/3 );
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE ,
                     LPTSTR    ,
                     int       nCmdShow)
{

 	// TODO: Place code here.

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, maxLoadString);
	LoadString(hInstance, IDC_LESSON1, szWindowClass, maxLoadString);
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
		//params.MultiSampleType = D3DMULTISAMPLE_NONE;


	D3D::GraphicDevice graphicDevice( mainWindow, params );
	graphicDevice.SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	graphicDevice.SetRenderState( D3DRS_LIGHTING, FALSE );

	graphicDevice.SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
	graphicDevice.SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );

	D3D::VertexDeclaration vertexDeclaration(graphicDevice);
	vertexDeclaration.Use();

	D3D::Shader shader(graphicDevice, L"shader.vsh");
	shader.Use();

#define TEST
#ifdef TEST
	shader.SetWorldMatrix( TranslationMatrix( 0.0f, 0.0f, 0.0f )*
		RotateXMatrix(2*D3DX_PI / 8)*RotateYMatrix(0*D3DX_PI / 8)*ScaleMatrix( 1/170.0f, 1/170.0f, 1/170.0f) );
#else
	shader.SetWorldMatrix( RotateXMatrix(0*D3DX_PI / 8)*RotateYMatrix(0*D3DX_PI / 8) );
#endif
	//shader.SetViewMatrix(ViewMatrix(D3DXVECTOR3(0.0f, 0.0f, -1.0f),
	//								D3DXVECTOR3(0.0f, 0.0f, 0.0f),
	//								D3DXVECTOR3(0.0f, 1.0f, 0.0f)));

	shader.SetProjectiveMatrix( ProjectiveMatrix(0.5f, 10000.0f) );

	D3D::VertexBuffer vertexBuffer(graphicDevice, sizeof(D3D::Vertex)*verticesCount);
	vertexBuffer.SetVertices(vertices, sizeof(D3D::Vertex)*verticesCount);
	vertexBuffer.Use(0,0);

	D3D::IndexBuffer indexBuffer(graphicDevice, sizeof(UINT)*indicesCount);
	indexBuffer.SetIndices(indices, sizeof(UINT)*indicesCount);
	indexBuffer.Use();
#ifdef TEST
	SpectatorCoords spectatorCoords( 1.0f, D3DX_PI / 2, -D3DX_PI / 2 + 2*SpectatorCoords::deltaFi);
#else
	SpectatorCoords spectatorCoords( 100.0f, D3DX_PI / 2, -D3DX_PI / 2 + 2*SpectatorCoords::deltaFi);
#endif
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
            Render(graphicDevice, vertexBuffer, indexBuffer, shader, vertexDeclaration, spectatorCoords);
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LESSON1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_LESSON1);
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
	int wmId, wmEvent;

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
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			pShader->SetViewMatrix(ViewMatrix(	pSpectatorCoords->GetCartesianCoords(),
												D3DXVECTOR3(0.0f, 0.0f, 0.0f),
												D3DXVECTOR3(0.0f, 1.0f, 0.0f) ));
			break;
		}
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

