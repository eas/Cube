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

	GraphicDevice::GraphicDevice( HWND hWnd )
		: directX_(NULL),
		device_(NULL)
	{
		// Create the D3D object.
		if( NULL == ( directX_ = Direct3DCreate9( D3D_SDK_VERSION ) ) )
			throw Error(E_FAIL);

		// Set up the structure used to create the D3Ddevice_
		D3DPRESENT_PARAMETERS params;
		ZeroMemory( &params, sizeof( params ) );

		//params.BackBufferFormat				= D3DFMT_A8R8G8B8;
		//params.BackBufferCount				= 1;
		//params.Windowed						= TRUE;
		//params.SwapEffect					= D3DSWAPEFFECT_FLIP;
		////params.EnableAutoDepthStencil		= TRUE;
		////params.AutoDepthStencilFormat		= D3DFMT_D32;
		//params.MultiSampleType				= D3DMULTISAMPLE_NONE;
		//params.hDeviceWindow				= hWnd;

		params.Windowed = TRUE;
		params.SwapEffect = D3DSWAPEFFECT_DISCARD;
		params.BackBufferFormat = D3DFMT_UNKNOWN;

		// Create the D3Ddevice_
		CheckResult( directX_->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
										  D3DCREATE_HARDWARE_VERTEXPROCESSING,
										  &params, &device_ ) );
	  

		// Turn off culling, so we see the front and back of the triangle
		device_->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

		// Turn off D3D lighting, since we are providing our own vertex colors
		device_->SetRenderState( D3DRS_LIGHTING, FALSE );

		device_->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

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

	GraphicDevice::~GraphicDevice()
	{

		if( device_ != NULL )
			device_->Release();

		if( directX_ != NULL )
			directX_->Release();
	}

	Shader::Shader(GraphicDevice& device)
		:shader_(NULL), vertexDeclaration_(NULL)
	{
		D3DVERTEXELEMENT9 vertexDeclaration[] = 
		{
			{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
			{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
			D3DDECL_END()

		};

		CheckResult( device->CreateVertexDeclaration(vertexDeclaration, &vertexDeclaration_) );

		ID3DXBuffer* shaderCode = NULL;

		CheckResult( D3DXAssembleShaderFromFile(L"shader.vsh", NULL, NULL, NULL, &shaderCode, NULL) );
		CheckResult( device->CreateVertexShader(static_cast<DWORD*>(shaderCode->GetBufferPointer()), &shader_) );
		shaderCode->Release();

		float matrix[] =
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};

		CheckResult( device->SetVertexShaderConstantF(0, matrix, 4) );

		CheckResult( device->SetVertexShader(shader_) );
	}

	Shader::Shader(GraphicDevice& device, LPCTSTR fileName, D3DVERTEXELEMENT9 vertexDeclaration[])
		:shader_(NULL), vertexDeclaration_(NULL)
	{
		CheckResult( device->CreateVertexDeclaration(vertexDeclaration, &vertexDeclaration_) );

		ID3DXBuffer* shaderCode = NULL;

		CheckResult( D3DXAssembleShaderFromFile(fileName, NULL, NULL, NULL, &shaderCode, NULL) );
		CheckResult( device->CreateVertexShader(static_cast<DWORD*>(shaderCode->GetBufferPointer()), &shader_) );

		shaderCode->Release();

		CheckResult( device->SetVertexShader(shader_) );
		
	}
	Shader::~Shader()
	{
		if( vertexDeclaration_ != NULL )
			vertexDeclaration_->Release();
		if( shader_ != NULL )
			shader_->Release();
	}

	VertexBuffer::VertexBuffer(GraphicDevice& device, UINT length)
		:vertexBuffer_(NULL)
	{
		CheckResult(device->CreateVertexBuffer( length,
							  0, D3DFVF_CUSTOMVERTEX,
							  D3DPOOL_DEFAULT, &vertexBuffer_, NULL ));
	}
	VertexBuffer::~VertexBuffer()
	{
		if(vertexBuffer_ != NULL)
			vertexBuffer_->Release();
	}

	void VertexBuffer::Set(Vertex vertices[], UINT sizeToLock)
	{
		void* buffer = NULL;

		CheckResult( vertexBuffer_->Lock(0, sizeToLock, &buffer, 0) );
		memcpy( buffer, vertices, sizeToLock);
		CheckResult( vertexBuffer_->Unlock() );
	}

	IndexBuffer::IndexBuffer(GraphicDevice& device, UINT length)
		:indexBuffer_(NULL)
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

	void IndexBuffer::Set(UINT indices[], UINT sizeToLock)
	{
		void* buffer = NULL;

		CheckResult( indexBuffer_->Lock(0, sizeToLock, &buffer, 0) );
		memcpy( buffer, indices, sizeToLock);
		CheckResult( indexBuffer_->Unlock() );
	}

} // namespace D3D