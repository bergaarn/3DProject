#include "Renderer.h"

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

bool Renderer::createSamplerState()
{


	return true;
}

Renderer::Renderer():
	device(nullptr), immediateContext(nullptr), swapChain(nullptr), viewport(),
	backBufferRTV(nullptr), dsTexture(nullptr), dsView(nullptr), sampler(nullptr),
	texture(nullptr), texSRV(nullptr), window(nullptr), it(0.0f)
{
}

Renderer::~Renderer()
{
	this->immediateContext->ClearState();

	// Release memory
	//this->texSRV->Release();
	//this->texture->Release();
	//this->sampler->Release();
	this->dsView->Release();
	this->dsTexture->Release();
	this->backBufferRTV->Release();
	this->swapChain->Release();
	this->immediateContext->Release();
	this->device->Release();
}

void Renderer::init(Window& window)
{
	this->window = &window;
	this->it = 0.5f;
	createInterfaces();
	createViews();
	createDepthStencil();
	createViewport();
	
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Renderer::render(Scene& scene)
{
	if (this->it <= 0.0f)
		this->it = 0.5f;

	this->it = this->it - 0.00005f;
	float clearColor[4] = { this->it, this->it, this->it, this->it };
	immediateContext->ClearRenderTargetView(this->backBufferRTV, clearColor);

	this->swapChain->Present(1, 0);
}