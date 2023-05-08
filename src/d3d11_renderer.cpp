#include "d3d11_renderer.h"

#define HRCheck(hr, error_message, error_type) \
	do { \
		HRESULT result = (hr); \
		if (FAILED(result)) { \
			MessageBox(0, (error_message), (error_type), 0);  \
			return S_FALSE; \
		} \
	} while(0)

#define HRCheckSoft(hr, error_message, error_type) \
	do { \
		HRESULT result = (hr); \
		if (FAILED(result)) { \
			MessageBox(0, (error_message), (error_type), 0);  \
		} \
	} while(0)

ID3D11Device* device;
ID3D11DeviceContext* deviceContext;

ID3D11Texture2D* depthStencilBuffer;
ID3D11DepthStencilView* depthStencilView;

ID3D11RenderTargetView* renderTargetView;

IDXGISwapChain* swapChain;

ID3D11InputLayout* inputLayout;

ID3D11RasterizerState* rasterizerState;

D3D11_INPUT_ELEMENT_DESC VertexDesc[] = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20,
		D3D11_INPUT_PER_VERTEX_DATA, 0}
};

bool mEnable4xMsaa = true;

ID3D11Device* GetDevice() { return device; }
ID3D11DeviceContext* GetDeviceContext() { return deviceContext; }

HRESULT Initialize(HWND window, int width, int height)
{
	UINT createDeviceFlags = 0;

#ifdef DEBUG
	createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif // DEBUG

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		createDeviceFlags,
		0,
		0,
		D3D11_SDK_VERSION,
		&device,
		&featureLevel,
		&deviceContext);

	HRCheck(hr, L"Failed to Create Direct3D Device (D3D11CreateDevice)", L"Error");

	if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
		MessageBox(0, L"Direct3D Feature Level 11 not supported", 0, 0);
		return S_FALSE;
	}

	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	SwapChainDesc.BufferDesc.Width = width;
	SwapChainDesc.BufferDesc.Height = height;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	UINT m4xMsaaQuality;
	device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);
	assert(m4xMsaaQuality > 0);

	if (mEnable4xMsaa) {
		SwapChainDesc.SampleDesc.Count = 4;

		SwapChainDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	else {
		SwapChainDesc.SampleDesc.Count = 1;
		SwapChainDesc.SampleDesc.Quality = 0;
	}
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.OutputWindow = window;
	SwapChainDesc.Windowed = true;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.Flags = 0;

	IDXGIDevice* dxgiDevice = 0;
	hr = device->QueryInterface(__uuidof(IDXGIDevice),
		(void**)&dxgiDevice);

	IDXGIAdapter* dxgiAdapter = 0;
	hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter),
		(void**)&dxgiAdapter);

	IDXGIFactory* dxgiFactory = 0;
	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory),
		(void**)&dxgiFactory);

	HRCheck(dxgiFactory->CreateSwapChain(device, &SwapChainDesc, &swapChain),
		L"Failed to Create Direct3D Swap Chain (CreateSwapChain)", L"Error");

	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	ID3D11Texture2D* backBuffer;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	device->CreateRenderTargetView(backBuffer, 0, &renderTargetView);
	backBuffer->Release();

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (mEnable4xMsaa) {
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	else {
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HRCheck(device->CreateTexture2D(&depthStencilDesc,
		0,
		&depthStencilBuffer),
		L"Failed to Create Direct3D Depth Stencil Buffer (CreateTexture2D)", L"Error");

	HRCheck(device->CreateDepthStencilView(depthStencilBuffer,
		0,
		&depthStencilView), L"Failed to Create Direct3D Depth Stencil View (CreateDepthStencilView)", L"Error");

	deviceContext->OMSetRenderTargets(1,
		&renderTargetView, depthStencilView);

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	deviceContext->RSSetViewports(1, &viewport);

	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.FrontCounterClockwise = false;
	rsDesc.DepthClipEnable = true;
	HRCheck(device->CreateRasterizerState(&rsDesc, &rasterizerState), L"Failed to Create Direct3D Rasterizer State (CreateRasterizerState)", L"Error");

	return S_OK;
}

void DrawScene() {
	float clearColor[] = { 0.01f, .05f, 0.01f, 1 };
	deviceContext->ClearRenderTargetView(renderTargetView, clearColor);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	deviceContext->RSSetState(rasterizerState);
}

void EndFrame() {
	swapChain->Present(0, 0);
}

void WindowResizeBuffers(int newWindowWidth, int newWindowHeight){

}

HRESULT VertexBuffer::Create() {
	this->stride = (UINT)sizeof(Vertex);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = size;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = data;

	HRESULT hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &this->buffer);

	deviceContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
	return hr;
}

HRESULT Buffer::Create() {
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = size;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData;
	indexBufferData.pSysMem = data;
	HRESULT hr = device->CreateBuffer(&indexBufferDesc, &indexBufferData, &buffer);
	deviceContext->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, 0);
	return hr;
}

void ConstantBuffer::Create() {
	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.ByteWidth = size + 0xf & 0xfffffff0;
	desc.StructureByteStride = 0;

	device->CreateBuffer(&desc, 0, &buffer);
}
void ConstantBuffer::Update() {
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	deviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	CopyMemory(mappedResource.pData, data, size);
	deviceContext->Unmap(buffer, 0);
}

void ConstantBuffer::Destroy() {
	buffer->Release();
}

HRESULT Shader::create(LPCWSTR srcFile, LPCSTR entryPoint, LPCSTR target) {
	if (!srcFile || !entryPoint || !target)
		return E_INVALIDARG;

	ID3DBlob* shaderBlob = nullptr;
	ID3D11ShaderReflection* shaderReflection = nullptr;

	HRCheck(D3DCompileFromFile(srcFile, 0, 0, entryPoint,
		target, 0, 0, &shaderBlob, 0), L"Failed to Compile Shader (D3DCompileFromFile)", L"Error");

	char* shaderCode = reinterpret_cast<char*>(shaderBlob->GetBufferPointer());
	size_t shaderCodeSize = shaderBlob->GetBufferSize();

	D3DReflect(shaderCode, shaderCodeSize, IID_ID3D11ShaderReflection, (void**)&shaderReflection);

	D3D11_SHADER_DESC shaderDesc;
	shaderReflection->GetDesc(&shaderDesc);
	
	if (shaderDesc.OutputParameters == 4)
	{
		//Vertex Shader
		device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &vertexShader);
		deviceContext->VSSetShader(vertexShader, 0, 0);

		device->CreateInputLayout(VertexDesc, 3, shaderCode, shaderCodeSize, &inputLayout);

		deviceContext->IASetInputLayout(inputLayout);
		deviceContext->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	else if (shaderDesc.OutputParameters == 1)
	{
		//Pixel Shader
		device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &pixelShader);
		deviceContext->PSSetShader(pixelShader, 0, 0);
	}
	else
	{
		MessageBox(0, L"Input .HLSL Shader is not a valid Shader", L"Error", 0);
	}

	shaderBlob->Release();
	shaderReflection->Release();
}