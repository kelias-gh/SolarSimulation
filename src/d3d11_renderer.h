#pragma once
#include <assert.h>
#include <d3d11.h>
#include <fstream>
#include <vector>
#include <d3dcompiler.h>
#include <d3d11shader.h>

//Initialize DirectX11
HRESULT Initialize(HWND hwnd, int width, int height);

void DrawScene();
void EndFrame();

// Resizes Swap Chain, Rasterizer etc. 
//to match current window size
void WindowResizeBuffers(int newWindowWidth, int newWindowHeight);

struct Shader {
	HRESULT create(LPCWSTR srcFile, LPCSTR entryPoint, LPCSTR target);
	union
	{
		ID3D11PixelShader* pixelShader;
		ID3D11VertexShader* vertexShader;
	};
};

struct VertexBuffer {
	ID3D11Buffer* buffer;
	uint32_t verticesAmount;
	UINT stride;
	UINT offset = 0;

	UINT size;
	void* data;

	HRESULT Create();

	void Destroy();
};

struct Buffer {
	ID3D11Buffer* buffer;
	uint32_t indicesAmount;

	UINT size;
	void* data;

	HRESULT Create();

	void Destroy();
};

struct ConstantBuffer {
	ID3D11Buffer* buffer;

	uint32_t size;
	void* data = reinterpret_cast<void*>(data);

	void Create();
	void Update();
	void Destroy();
};

ID3D11Device* GetDevice();
ID3D11DeviceContext* GetDeviceContext();