#include "stdafx.h"
#define D3D_DEBUG_INFO 1

#include "graphics.h"

#include "d3dx9.h"




namespace D3D
{
	Error::Error(HRESULT errorCode)
		: errorCode_(errorCode)
	{
		switch(errorCode)
		{
		case D3DERR_DEVICELOST:			error_ = "D3DERR_DEVICELOST"; break;
		case D3DERR_INVALIDCALL:		error_ = "D3DERR_INVALIDCALL"; break;
		case D3DERR_NOTAVAILABLE:		error_ = "D3DERR_NOTAVAILABLE"; break;
		case D3DERR_OUTOFVIDEOMEMORY:	error_ = "D3DERR_OUTOFVIDEOMEMORY"; break;
		case E_FAIL:					error_ = "E_FAIL"; break;
		case D3DXERR_INVALIDDATA:		error_ = "D3DXERR_INVALIDDATA"; break;
		case E_OUTOFMEMORY:				error_ = "E_OUTOFMEMORY"; break;
		default:
			error_ = "Unknown error";
		}
	}

	GraphicDevice::GraphicDevice(HWND hWnd, D3DPRESENT_PARAMETERS &params)
		:directX_(NULL),
		device_(NULL)
	{
		if( NULL == ( directX_ = Direct3DCreate9( D3D_SDK_VERSION ) ) )
			throw Error(E_FAIL);
		CheckResult( directX_->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
										  D3DCREATE_HARDWARE_VERTEXPROCESSING,
										  &params, &device_ ) );
	}

	GraphicDevice::GraphicDevice( const GraphicDevice& other )
		:directX_(other.directX_),
		device_(other.device_)
	{
		directX_->AddRef();
		device_->AddRef();
	}

	GraphicDevice::~GraphicDevice()
	{

		if( device_ != NULL )
			device_->Release();

		if( directX_ != NULL )
			directX_->Release();
	}

	Shader::Shader(GraphicDevice& device, LPCTSTR fileName)
		:Base(device), shader_(NULL)
	{
		ID3DXBuffer* shaderCode = NULL;

		CheckResult( D3DXAssembleShaderFromFile(fileName, NULL, NULL, NULL, &shaderCode, NULL) );
		DWORD* buf = static_cast<DWORD*>(shaderCode->GetBufferPointer());
		buf;
		CheckResult( device->CreateVertexShader(static_cast<DWORD*>(shaderCode->GetBufferPointer()), &shader_) );

		shaderCode->Release();
	}

	Shader::~Shader()
	{
		if( shader_ != NULL )
			shader_->Release();
	}

	void Shader::SetupConstantF( GraphicDevice& device, UINT startRegister, const float* data, UINT count)
	{
		CheckResult( device->SetVertexShaderConstantF(startRegister, data, count) );

	}

	VertexBuffer::VertexBuffer(GraphicDevice& device, UINT length)
		:Base(device), vertexBuffer_(NULL)
	{
		CheckResult(device->CreateVertexBuffer( length,
							  0, 0,
							  D3DPOOL_DEFAULT, &vertexBuffer_, NULL ));
	}

	VertexBuffer::~VertexBuffer()
	{
		if(vertexBuffer_ != NULL)
			vertexBuffer_->Release();
	}

	void VertexBuffer::SetVertices(Vertex vertices[], UINT sizeToLock)
	{
		void* buffer = NULL;

		CheckResult( vertexBuffer_->Lock(0, sizeToLock, &buffer, 0) );
		memcpy( buffer, vertices, sizeToLock);
		CheckResult( vertexBuffer_->Unlock() );
	}

	IndexBuffer::IndexBuffer(GraphicDevice& device, UINT length)
		:Base(device), indexBuffer_(NULL)
	{
		CheckResult(device->CreateIndexBuffer( length,
							  0, D3DFMT_INDEX32,
							  D3DPOOL_DEFAULT, &indexBuffer_, NULL ));
	}

	IndexBuffer::~IndexBuffer()
	{
		if(indexBuffer_ != NULL)
			indexBuffer_->Release();
	}

	void IndexBuffer::SetIndices(UINT indices[], UINT sizeToLock)
	{
		void* buffer = NULL;

		CheckResult( indexBuffer_->Lock(0, sizeToLock, &buffer, 0) );
		memcpy( buffer, indices, sizeToLock);
		CheckResult( indexBuffer_->Unlock() );
	}

	VertexDeclaration::VertexDeclaration(GraphicDevice& device, D3DVERTEXELEMENT9 vertexDeclaration[])
		:Base(device), vertexDeclaration_(NULL)
	{
		CheckResult( device->CreateVertexDeclaration(vertexDeclaration, &vertexDeclaration_) );
	}

	VertexDeclaration::~VertexDeclaration()
	{
		if(vertexDeclaration_ != NULL)
			vertexDeclaration_->Release();
	}

} // namespace D3D