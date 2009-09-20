#pragma once

#include "windows.h"
#include "d3d9.h"
#include "exception"


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
	static D3DVERTEXELEMENT9 vertexDeclaration[] = 
	{
		{0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0},
		{0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};

	inline void CheckResult(HRESULT errorCode)
	{
		if( errorCode != D3D_OK )
			throw Error(errorCode);
	}

	class GraphicDevice
	{
	public:
		class Scene;
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
	class GraphicDevice::Scene
		:public Base
	{
	public:
		Scene(GraphicDevice& device,
				DWORD flags, D3DCOLOR color, float z, DWORD stencil,
				DWORD count = NULL, const D3DRECT* rects = NULL )
				:Base(device)
		{
			//assert( (count == NULL && rects == NULL) || (count != NULL && rects != NULL) );
			CheckResult( device->Clear( count, rects, flags, color, z, stencil ) );
			CheckResult( device->BeginScene() );
		}
		~Scene()
		{
			CheckResult( device_->EndScene() );
			CheckResult( device_->Present( NULL, NULL, NULL, NULL ) );
		}
	private:
		Scene(const Scene&);
		Scene& operator=(const Scene&);
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