#include "Renderer.h"
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <DirectXMath.h>
using namespace DirectX;

bool Renderer::createInterfaces()
{
	UINT flags = 0;
	if (_DEBUG)
		flags = D3D11_CREATE_DEVICE_DEBUG;

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = this->window->getWidth();
	swapChainDesc.BufferDesc.Height = this->window->getHeight();
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = this->window->getWindowHandler();
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	return !(FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		flags, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &this->swapChain, &this->device,
		nullptr, &this->immediateContext)));
}

bool Renderer::createViews()
{
	ID3D11Texture2D* backBuffer = nullptr;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
	{
		std::cerr << "Failed to get back buffer." << std::endl;
		return false;
	}

	D3D11_TEXTURE2D_DESC backBufferDesc{};
	backBuffer->GetDesc(&backBufferDesc);
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.Format = backBufferDesc.Format;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

	HRESULT result = this->device->CreateUnorderedAccessView(backBuffer, &uavDesc, &this->backBufferUAV);
	if (FAILED(result))
	{
		std::cerr << "Failed to create UAV." << std::endl;
		return false;
	}

	if (FAILED(this->device->CreateRenderTargetView(backBuffer, NULL, &this->backBufferRTV)))
	{
		std::cerr << "Failed to create backbufferRTV" << std::endl;
		return false;
	}
	backBuffer->Release();

	return true;
}

bool Renderer::createDepthStencil()
{
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = this->window->getWidth();
	textureDesc.Height = this->window->getHeight();
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &dsTexture)))
	{
		std::cerr << "Failed to create depth stencil texture." << std::endl;
		return false;
	}

	if (FAILED(device->CreateDepthStencilView(dsTexture, 0, &dsView)))
	{
		std::cerr << "Failed to create depth stencil view." << std::endl;
		return false;
	}

	return true;
}

void Renderer::createViewport()
{
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(this->window->getWidth());
	viewport.Height = static_cast<float>(this->window->getHeight());
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
}

bool Renderer::createShaders() 
{
	// - Create Pixel Shader
	std::string shaderData;
	std::ifstream fileReader;

	fileReader.open("../PixelShader.cso", std::ios::binary | std::ios::ate);
	if (!fileReader.is_open())
	{
		std::cerr << "PixelShader file could not be created." << std::endl;
		return false;
	}

	fileReader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(fileReader.tellg()));
	fileReader.seekg(0, std::ios::beg);
	shaderData.assign((std::istreambuf_iterator<char>(fileReader)),
		std::istreambuf_iterator<char>());

	if (FAILED(device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &pixelShader)))
	{
		std::cerr << "Failed to create pixel shader!" << std::endl;
		return false;
	}
	shaderData.clear();
	fileReader.close();

	// - Create Compute Shader
	fileReader.open("../ComputeShader.cso", std::ios::binary | std::ios::ate);
	if (!fileReader.is_open())
	{
		std::cerr << "ComputeShader file could not be created." << std::endl;
		return false;
	}

	fileReader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(fileReader.tellg()));
	fileReader.seekg(0, std::ios::beg);
	shaderData.assign((std::istreambuf_iterator<char>(fileReader)),
		std::istreambuf_iterator<char>());

	HRESULT result = this->device->CreateComputeShader(shaderData.c_str(), shaderData.length(), nullptr, &this->computeShader);
	if (FAILED(result))
	{
		std::cerr << "Failed to create compute shader" << std::endl;
		return false;
	}
	shaderData.clear();
	fileReader.close();

	// - Create Vertex Shader
	fileReader.open("../VertexShader.cso", std::ios::binary | std::ios::ate);
	if (!fileReader.is_open())
	{
		std::cerr << "VertexShader file could not be created." << std::endl;
		return false;
	}

	fileReader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(fileReader.tellg()));
	fileReader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(fileReader)),
		std::istreambuf_iterator<char>());

	if (FAILED(device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &vertexShader)))
	{
		std::cerr << "VertexShader could not be created." << std::endl;
		return false;
	}
	D3D11_INPUT_ELEMENT_DESC inputDesc[3] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = device->CreateInputLayout(inputDesc, 3, shaderData.c_str(), shaderData.length(), &inputLayout);
	if (FAILED(hr))
	{
		std::cerr << "InputLayout could not be created." << std::endl;
		return false;
	}

	fileReader.close();

	return true;
}

bool Renderer::createVertexBuffer()
{
	Vertex triangle[] =
	{
		{ {-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f}, {0, 0, -1} },
		{ {0.5f, -0.5f, 0.0f}, {1.0f, 1.0f}, {0, 0, -1} },
		{ {-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f}, {0, 0, -1} },
		
		{ {-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f}, {0, 0, -1} },
		{ {0.5f, 0.5f, 0.0f}, {1.0f, 0.0f}, {0, 0, -1} },
		{ {0.5f, -0.5f, 0.0f}, {1.0f, 1.0f}, {0, 0, -1} },
		
	};

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(triangle);
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;			// Unchangable after creation
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA triangleData;
	triangleData.pSysMem = &triangle;					// Data always necessary when flag IMMUTABLE
	triangleData.SysMemPitch = 0;
	triangleData.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateBuffer(&bufferDesc, &triangleData, &vertexBuffer);
	return !FAILED(hr);
}

bool Renderer::createMatrixBuffer()
{
	// Camera Creation
	XMVECTOR camera = XMVectorSet(0.0f, 0.0f, -3.0f, 1.0f);
	XMVECTOR direction = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

	XMMATRIX worldMatrix = { XMMatrixTranslation(0.0f, 0.0f, -1.0f) * XMMatrixRotationY(0.0f) };
	XMMATRIX viewMatrix = { XMMatrixLookAtLH(camera, direction, up) };
	XMMATRIX perspectiveMatrix = { XMMatrixPerspectiveFovLH(XM_PI / 3.0f, (float)this->window->getWidth() / (float)this->window->getHeight(), 0.1f, 10.0f)};
	
	XMMATRIX viewPerspectiveMatrix = XMMatrixMultiply(viewMatrix, perspectiveMatrix);
	
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewPerspectiveMatrix = XMMatrixTranspose(viewPerspectiveMatrix);
	
	XMFLOAT4X4 worldMatrixTransposed;
	XMFLOAT4X4 viewPerspectiveMatrixTransposed;
	XMStoreFloat4x4(&worldMatrixTransposed, worldMatrix);
	XMStoreFloat4x4(&viewPerspectiveMatrixTransposed, viewPerspectiveMatrix);
	XMFLOAT4X4 transposedMatrices[2] = { worldMatrixTransposed, viewPerspectiveMatrixTransposed };

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.ByteWidth = sizeof(transposedMatrices);
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA matrixBufferData;
	matrixBufferData.pSysMem = &transposedMatrices;
	matrixBufferData.SysMemPitch = 0;
	matrixBufferData.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateBuffer(&matrixBufferDesc, &matrixBufferData, &this->matrixBuffer);
	return !FAILED(hr);
	return true;
}

void Renderer::updateMatrices(const float &angle)
{
	XMVECTOR camera = XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);
	XMVECTOR direction = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX worldMatrix = { XMMatrixTranslation(0.0f, 0.0f, 0.0f) * XMMatrixRotationY(angle) };
	XMMATRIX viewMatrix = { XMMatrixLookAtLH(camera, direction, up) };
	XMMATRIX perspectiveMatrix = { XMMatrixPerspectiveFovLH(XM_PI / 3.0f, (float)this->window->getWidth() / (float)this->window->getHeight(), 0.1f, 10.0f) };

	XMMATRIX viewPerspectiveMatrix = XMMatrixMultiply(viewMatrix, perspectiveMatrix);

	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewPerspectiveMatrix = XMMatrixTranspose(viewPerspectiveMatrix);

	XMFLOAT4X4 worldMatrixTransposed;
	XMFLOAT4X4 viewPerspectiveMatrixTransposed;
	XMStoreFloat4x4(&worldMatrixTransposed, worldMatrix);
	XMStoreFloat4x4(&viewPerspectiveMatrixTransposed, viewPerspectiveMatrix);
	XMFLOAT4X4 transposedMatrices[2] = { worldMatrixTransposed, viewPerspectiveMatrixTransposed };

	D3D11_MAPPED_SUBRESOURCE mResource;
	ZeroMemory(&mResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	this->immediateContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mResource);
	memcpy(mResource.pData, transposedMatrices, sizeof(transposedMatrices));
	this->immediateContext->Unmap(matrixBuffer, 0);
}

bool Renderer::createSamplerState()
{
	return true;
}

bool Renderer::createGBuffers()
{
	D3D11_TEXTURE2D_DESC gBuffDesc;
	gBuffDesc.Width = this->window->getWidth();
	gBuffDesc.Height = this->window->getHeight();
	gBuffDesc.MipLevels = 1;
	gBuffDesc.ArraySize = 1;
	gBuffDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	gBuffDesc.SampleDesc.Count = 1;
	gBuffDesc.SampleDesc.Quality = 0;
	gBuffDesc.Usage = D3D11_USAGE_DEFAULT;
	gBuffDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	gBuffDesc.CPUAccessFlags = 0;
	gBuffDesc.MiscFlags = 0;

	HRESULT hr = device->CreateTexture2D(&gBuffDesc, nullptr, &gBuffTex0);
	if (FAILED(hr))
	{
		return false;
	}

	hr = device->CreateShaderResourceView(gBuffTex0, nullptr, &gBuffSRV0);
	if (FAILED(hr))
	{
		return false;
	}

	hr = device->CreateRenderTargetView(gBuffTex0, nullptr, &gBuffRTV0);
	if (FAILED(hr))
	{
		return false;
	}

	hr = device->CreateTexture2D(&gBuffDesc, nullptr, &gBuffTex1);
	if (FAILED(hr))
	{
		return false;
	}

	hr = device->CreateShaderResourceView(gBuffTex1, nullptr, &gBuffSRV1);
	if (FAILED(hr))
	{
		return false;
	}

	hr = device->CreateRenderTargetView(gBuffTex1, nullptr, &gBuffRTV1);
	if (FAILED(hr))
	{
		return false;
	}

	hr = device->CreateTexture2D(&gBuffDesc, nullptr, &gBuffTex2);
	if (FAILED(hr))
	{
		return false;
	}

	hr = device->CreateShaderResourceView(gBuffTex2, nullptr, &gBuffSRV2);
	if (FAILED(hr))
	{
		return false;
	}

	hr = device->CreateRenderTargetView(gBuffTex2, nullptr, &gBuffRTV2);
	if (FAILED(hr))
	{
		return false;
	}

	hr = device->CreateTexture2D(&gBuffDesc, nullptr, &gBuffTex3);
	if (FAILED(hr))
	{
		return false;
	}

	hr = device->CreateShaderResourceView(gBuffTex3, nullptr, &gBuffSRV3);
	if (FAILED(hr))
	{
		return false;
	}

	hr = device->CreateRenderTargetView(gBuffTex3, nullptr, &gBuffRTV3);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool Renderer::createMaterialBuffer()
{
	Material material
	{
		{0.0f, 1.0f, 1.0f, 0.0f},	// Ambient
		{0.0f, 0.0f, 1.0f, 0.0f},	// Diffuse
		{0.0f, 1.0f, 0.0f, 0.0f},	// Specular
		75.0f						// Shininess
	};

	D3D11_BUFFER_DESC materialBufferDesc;
	materialBufferDesc.ByteWidth = sizeof(material);
	materialBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	materialBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	materialBufferDesc.CPUAccessFlags = 0;
	materialBufferDesc.MiscFlags = 0;
	materialBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA materialBufferData;
	materialBufferData.pSysMem = &material;
	materialBufferData.SysMemPitch = 0;
	materialBufferData.SysMemSlicePitch = 0;

	HRESULT hr = this->device->CreateBuffer(&materialBufferDesc, &materialBufferData, &materialBuffer);
	return !FAILED(hr);
}

Renderer::Renderer() :
	device(nullptr), immediateContext(nullptr), swapChain(nullptr), window(nullptr),
	vertexShader(nullptr), inputLayout(nullptr), pixelShader(nullptr), computeShader(nullptr),
	viewport(), backBufferRTV(nullptr), backBufferUAV(nullptr), dsTexture(nullptr), dsView(nullptr),
	sampler(nullptr), texture(nullptr), texSRV(nullptr),
	vertexBuffer(nullptr), matrixBuffer(nullptr), materialBuffer(nullptr),
	it(0.0f), numVertices(0), numGBuffers(0), gBuffersRTV(), gBuffersSRV(),
	gBuffTex0(nullptr), gBuffSRV0(nullptr), gBuffRTV0(nullptr),
	gBuffTex1(nullptr), gBuffSRV1(nullptr), gBuffRTV1(nullptr),
	gBuffTex2(nullptr), gBuffSRV2(nullptr), gBuffRTV2(nullptr),
	gBuffTex3(nullptr), gBuffSRV3(nullptr), gBuffRTV3(nullptr)
{
}

Renderer::~Renderer()
{
	this->immediateContext->ClearState();
	//this->texSRV->Release();
	//this->texture->Release();
	//this->sampler->Release();

	this->gBuffTex3->Release();
	this->gBuffSRV3->Release();
	this->gBuffRTV3->Release();

	this->gBuffTex2->Release();
	this->gBuffSRV2->Release();
	this->gBuffRTV2->Release();

	this->gBuffTex1->Release();
	this->gBuffSRV1->Release();
	this->gBuffRTV1->Release();

	this->gBuffTex0->Release();
	this->gBuffSRV0->Release();
	this->gBuffRTV0->Release();
	
	this->materialBuffer->Release();
	this->matrixBuffer->Release();
	this->vertexBuffer->Release();
	this->inputLayout->Release();
	this->vertexShader->Release();
	this->pixelShader->Release();
	this->computeShader->Release();
	this->dsView->Release();
	this->dsTexture->Release();
	this->backBufferUAV->Release();
	this->backBufferRTV->Release();
	this->swapChain->Release();
	this->immediateContext->Release();
	this->device->Release();
}

void Renderer::init(Window& window)
{
	this->window = &window;
	this->it = 0.5f;
	this->numGBuffers = 4;
	this->numVertices = 6;
	createInterfaces();
	createViews();
	createDepthStencil();
	createViewport();
	createShaders();
	createVertexBuffer();
	createMatrixBuffer();
	createMaterialBuffer();
	createGBuffers();

	gBuffersRTV[0] = gBuffRTV0;
	gBuffersRTV[1] = gBuffRTV1;
	gBuffersRTV[2] = gBuffRTV2;
	gBuffersRTV[3] = gBuffRTV3;
	gBuffersRTV[4] = nullptr;
	gBuffersRTV[5] = nullptr;
	gBuffersRTV[6] = nullptr;
	gBuffersRTV[7] = nullptr;

	gBuffersSRV[0] = gBuffSRV0;
	gBuffersSRV[1] = gBuffSRV1;
	gBuffersSRV[2] = gBuffSRV2;
	gBuffersSRV[3] = gBuffSRV3;
	gBuffersSRV[4] = nullptr;
	gBuffersSRV[5] = nullptr;
	gBuffersSRV[6] = nullptr;
	gBuffersSRV[7] = nullptr;

	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Renderer::render(Scene& scene)
{
	if (this->it >= 1.0f)
		this->it = 0.5f;

	this->it+= 0.0005f;
	float clearColor[4] = { this->it/4, this->it/2, this->it/4, this->it };
	immediateContext->ClearRenderTargetView(this->backBufferRTV, clearColor);
	immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	immediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	immediateContext->IASetInputLayout(inputLayout);
	immediateContext->VSSetConstantBuffers(0, 1, &matrixBuffer);
	immediateContext->VSSetShader(vertexShader, nullptr, 0);
	immediateContext->RSSetViewports(1, &viewport);
	immediateContext->PSSetShader(pixelShader, nullptr, 0);
	immediateContext->PSSetConstantBuffers(0, 1, &materialBuffer);
	immediateContext->OMSetRenderTargets(4, gBuffersRTV, dsView);
	immediateContext->Draw(this->numVertices, 0);
	immediateContext->OMSetRenderTargets(4, gBuffersRTV+4, nullptr);
	
	immediateContext->CSSetShaderResources(0, 4, gBuffersSRV);
	immediateContext->CSSetUnorderedAccessViews(0, 1, &backBufferUAV, nullptr);
	immediateContext->CSSetShader(this->computeShader, NULL, 0);
	immediateContext->Dispatch(160, 90, 1);
	immediateContext->CSSetShaderResources(0, 4, gBuffersSRV+4);

	this->swapChain->Present(0, 0);
}