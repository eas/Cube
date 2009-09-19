#pragma once

#include "windows.h"
#include "d3d9.h"
#include "exception"

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)




namespace D3D
{
	class Error
		:public std::exception
	{
	public:
		Error(HRESULT errorCode);

		HRESULT errorCode_;
		char* error_;
	};

	struct Vertex
	{
		float x, y, z, rhw;
		DWORD color;
	};

	inline void CheckResult(HRESULT errorCode)
	{
		if( errorCode != D3D_OK )
			throw Error(errorCode);
	}

	class GraphicDevice
	{
	public:
		GraphicDevice( HWND hWnd );
		GraphicDevice( HWND hWnd, D3DPRESENT_PARAMETERS& params );
		~GraphicDevice();
		IDirect3DDevice9* GetDevice()
		{
			return device_;
		}
		IDirect3DDevice9* operator->()
		{
			return device_;
		}
	private:
		IDirect3D9* directX_;
		IDirect3DDevice9* device_;
	};

	class Shader
	{
	public:
		Shader(GraphicDevice& device);
		Shader::Shader(GraphicDevice& device, LPCTSTR fileName, D3DVERTEXELEMENT9 vertexDeclaration[]);
		~Shader();
	private:
		IDirect3DVertexDeclaration9* vertexDeclaration_;
		IDirect3DVertexShader9* shader_;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(GraphicDevice& device, UINT length);
		~VertexBuffer();
		void Set(Vertex vertices[], UINT sizeToLock);
		inline IDirect3DVertexBuffer9* GetBuffer()
		{
			return vertexBuffer_;
		}
		IDirect3DVertexBuffer9* operator->()
		{
			return vertexBuffer_;
		}
	private:
		IDirect3DVertexBuffer9* vertexBuffer_;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer(GraphicDevice& device, UINT length);
		~IndexBuffer();
		void Set(UINT indices[], UINT sizeToLock);
		inline IDirect3DIndexBuffer9* GetBuffer()
		{
			return indexBuffer_;
		}
		IDirect3DIndexBuffer9* operator->()
		{
			return indexBuffer_;
		}
	private:
		IDirect3DIndexBuffer9* indexBuffer_;
	};
} // namespace D3D