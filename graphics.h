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
		GraphicDevice( HWND hWnd, D3DPRESENT_PARAMETERS& params );
		GraphicDevice( const GraphicDevice& other );

		~GraphicDevice();
		IDirect3DDevice9* GetDevice()
		{
			return device_;
		}
		IDirect3DDevice9* operator->()
		{
			return device_;
		}
		void SetRenderState(D3DRENDERSTATETYPE state, DWORD value)
		{
			CheckResult( device_->SetRenderState(state, value) );
		}
	private:
		IDirect3D9* directX_;
		IDirect3DDevice9* device_;
	};

	class Base
	{
	public:
		Base(GraphicDevice& device)
			:device_(device)
		{
		}
	protected:
		GraphicDevice device_;
	private:
		Base();
		Base(const Base& other);
		Base& operator=(const Base& other);
	};

	class Shader
		:public Base
	{
	public:
		Shader(GraphicDevice& device, LPCTSTR fileName);
		IDirect3DVertexShader9* GetShader()
		{
			return shader_;
		}
		void SetupConstantF( GraphicDevice& device, UINT startRegister, const float* data, UINT count);
		void Use()
		{
			CheckResult( device_->SetVertexShader(shader_) );
		}
		~Shader();
	private:
		IDirect3DVertexShader9* shader_;
	};

	class VertexBuffer
		:public Base
	{
	public:
		VertexBuffer(GraphicDevice& device, UINT length);
		~VertexBuffer();
		void SetVertices(Vertex vertices[], UINT sizeToLock);
		inline IDirect3DVertexBuffer9* GetBuffer()
		{
			return vertexBuffer_;
		}
		IDirect3DVertexBuffer9* operator->()
		{
			return vertexBuffer_;
		}
		void Use(UINT streamNumber, UINT offsetInBytes)
		{
			CheckResult( device_->SetStreamSource(streamNumber, vertexBuffer_, offsetInBytes, sizeof(Vertex)) );
		}
	private:
		IDirect3DVertexBuffer9* vertexBuffer_;
	};

	class IndexBuffer
		:public Base
	{
	public:
		IndexBuffer(GraphicDevice& device, UINT length);
		~IndexBuffer();
		void SetIndices(UINT indices[], UINT sizeToLock);
		inline IDirect3DIndexBuffer9* GetBuffer()
		{
			return indexBuffer_;
		}
		IDirect3DIndexBuffer9* operator->()
		{
			return indexBuffer_;
		}
		void Use()
		{
			CheckResult( device_->SetIndices(indexBuffer_) );
		}
	private:
		IDirect3DIndexBuffer9* indexBuffer_;
	};

	class VertexDeclaration
		:public Base
	{
	public:
		VertexDeclaration(GraphicDevice& device, D3DVERTEXELEMENT9 vertexDeclaration[]);
		~VertexDeclaration();
		IDirect3DVertexDeclaration9* GetDeclaration()
		{
			return vertexDeclaration_;
		}
		void Use()
		{
			CheckResult( device_->SetVertexDeclaration(vertexDeclaration_) );
		}
	private:
		IDirect3DVertexDeclaration9* vertexDeclaration_;
	};
} // namespace D3D