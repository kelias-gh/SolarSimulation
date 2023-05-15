#pragma once
#include <assert.h>
#include <fstream>
#include <vector>

#ifdef SYS_WIN

#include <d3dcompiler.h>
#include <d3d11.h>
#include <d3d11shader.h>

struct Shader {
	HRESULT create(LPCWSTR srcFile, LPCSTR entryPoint, LPCSTR target);
	union
	{
		ID3D11PixelShader* pixelShader;
		ID3D11VertexShader* vertexShader;
	};
};

ID3D11Device* GetDevice();
ID3D11DeviceContext* GetDeviceContext();

//----------------------Constant Buffer--------------------------//

struct ConstantBuffer {
	ID3D11Buffer* buffer;

	size_t size;
	void* data;
};

HRESULT __d3d11_CreateConstantBuffer(ConstantBuffer* constantBuffer, size_t size*, void* data);

HRESULT __d3d11_UpdateConstantBuffer(ConstantBuffer* constantBuffer);

HRESULT __d3d11_ReleaseConstantBuffer(ConstantBuffer* constantBuffer);

//----------------------Vertex Buffer--------------------------//

struct VertexBuffer {
	ID3D11Buffer* buffer;
	size_t verticesAmount = 0;
	size_t stride = 0;
	size_t offset = 0;
	size_t size = 0;

	void* data;
};

HRESULT __d3d11_CreateVertexBuffer(VertexBuffer* constantBuffer, size_t* size, size_t* stride, size_t* offset, size_t* verticesAmount, void* data);

HRESULT __d3d11_UpdateVertexBuffer(VertexBuffer* constantBuffer);

HRESULT __d3d11_ReleaseVertexBuffer(VertexBuffer* constantBuffer);

//----------------------Index Buffer--------------------------//

struct Buffer {
	ID3D11Buffer* buffer;
	size_t indicesAmount;
	size_t size;

	void* data;
};

HRESULT __d3d11_CreateIndextBuffer(Buffer* constantBuffer, size_t* size, size_t* indicesAmount, void* data);

HRESULT __d3d11_UpdateIndexBuffer(Buffer* constantBuffer);

HRESULT __d3d11_ReleaseIndexBuffer(Buffer* constantBuffer);

//----------------------------------------------------------------//

//Initialize DirectX11
HRESULT Initialize(HWND hwnd, int width, int height);

void DrawScene();
void EndFrame();

// Resizes Swap Chain, Rasterizer etc. 
//to match current window size
void WindowResizeBuffers(int newWindowWidth, int newWindowHeight);

#endif // SYS_WIN