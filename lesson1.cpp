// lesson1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "lesson1.h"

#include "graphics.h"
#include "internals.h"

#include "d3dx9.h"

const int maxLoadString = 100;

#define COLOR_DEFINE(name, r, g, b) \
	const DWORD name = D3DCOLOR_XRGB( (r), (g), (b) );
COLOR_DEFINE( Black,	000, 000, 000 )
COLOR_DEFINE( Red,		255, 000, 000 )
COLOR_DEFINE( Green,	000, 255, 000 )
COLOR_DEFINE( Blue,		000, 000, 255 )
COLOR_DEFINE( Yellow,	255, 255, 000 )
COLOR_DEFINE( Cyan,		000, 255, 255 )
COLOR_DEFINE( Magenta,	255, 000, 255 )
COLOR_DEFINE( White,	255, 255, 255 )

COLOR_DEFINE( Gray,		128, 128, 128 )

#define SIZE(Array) sizeof(Array)/sizeof(Array[0]);

D3D::Vertex cubeVertices[] =
{
	{ 100.0f, 200.0f, 10.0f, 1.0f, Red },
	{ 100.0f, 100.0f, 10.0f, 1.0f, Magenta },
	{ 200.0f, 100.0f, 10.0f, 1.0f, White },
	{ 200.0f, 200.0f, 10.0f, 1.0f, Yellow },

	{ 100.0f, 200.0f, 110.0f, 1.0f, Black },
	{ 100.0f, 100.0f, 110.0f, 1.0f, Blue },
	{ 200.0f, 100.0f, 110.0f, 1.0f, Cyan },
	{ 200.0f, 200.0f, 110.0f, 1.0f, Green },
};
UINT cubeVerticesCount = SIZE(cubeVertices);

UINT cubeIndices[] =
{
	3, 0, 1,	3, 1, 2, //front side
	3, 0, 4,	3, 4, 7, //bottom side
	7, 6, 5,	7, 5, 4, //back side
	2, 6, 5,	2, 5, 1, //top side
	1, 0, 4,	1, 4, 5, //left side
	3, 2, 6,	3, 6, 7, //right side
};
UINT cubeIndicesCount = SIZE(cubeIndices);

D3D::Vertex testVertices[] =
{
	{ 100.0f, 100.0f, 0.20f, 1.0f, Red },
	{ 200.0f, 150.0f, 0.30f, 1.0f, Red },
	{ 100.0f, 200.0f, 0.20f, 1.0f, Red },

	{ 200.0f, 200.0f, 0.20f, 1.0f, Blue },
	{ 100.0f, 150.0f, 0.30f, 1.0f, Blue },
	{ 200.0f, 100.0f, 0.20f, 1.0f, Blue },
};
UINT testVerticesCount = SIZE(testVertices);

UINT testIndices[] =
{
	0, 1, 2, //left triangle(Red)
	3, 4, 5, //right triangle(Blue)
};
UINT testIndicesCount = SIZE(testIndices);

#define TEST
#ifdef TEST
	D3D::Vertex* vertices = testVertices;
	UINT* indices = testIndices;
	UINT indicesCount = testIndicesCount;
	UINT verticesCount = testVerticesCount;
#else
	D3D::Vertex* vertices = cubeVertices;
	UINT* indices = cubeIndices;
	UINT indicesCount = cubeIndicesCount;
	UINT verticesCount = cubeVerticesCount;
#endif


float worldMatrix[] =
{
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};

// Global Variables:
TCHAR szTitle[maxLoadString];					// The title bar text
TCHAR szWindowClass[maxLoadString];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
HWND				CreateMainWindow(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

void Render(D3D::GraphicDevice& device, D3D::VertexBuffer& vertexBuffer, 
			D3D::IndexBuffer& indexBuffer, D3D::Shader& shader, D3D::VertexDeclaration& vertexDeclaration)
{
	D3D::GraphicDevice::Scene scene( device, D3DCLEAR_TARGET, Gray, 1.0f, 0 );

	vertexBuffer.Use(0, 0);
	indexBuffer.Use();
	shader.Use();
	vertexDeclaration.Use();

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
		//params.EnableAutoDepthStencil = TRUE;
		//params.AutoDepthStencilFormat = D3DFMT_D16;
		//params.MultiSampleType = D3DMULTISAMPLE_NONE;


	D3D::GraphicDevice graphicDevice( mainWindow, params );
	graphicDevice.SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	graphicDevice.SetRenderState( D3DRS_LIGHTING, FALSE );

	//graphicDevice.SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
	//graphicDevice.SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );

	D3D::VertexDeclaration vertexDeclaration(graphicDevice, D3D::vertexDeclaration);
	vertexDeclaration.Use();

	D3D::Shader shader(graphicDevice, L"shader.vsh");
	shader.Use();
	shader.SetupConstantF( graphicDevice, 0, worldMatrix, 4 );
	//D3D::CheckResult( graphicDevice->SetFVF(D3DFVF_CUSTOMVERTEX) );

	D3D::VertexBuffer vertexBuffer(graphicDevice, sizeof(D3D::Vertex)*verticesCount);
	vertexBuffer.SetVertices(vertices, sizeof(D3D::Vertex)*verticesCount);

	D3D::IndexBuffer indexBuffer(graphicDevice, sizeof(UINT)*indicesCount);
	indexBuffer.SetIndices(indices, sizeof(UINT)*indicesCount);

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
            Render(graphicDevice, vertexBuffer, indexBuffer, shader, vertexDeclaration);
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
	wcex.cbWndExtra		= 0;
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
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

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

