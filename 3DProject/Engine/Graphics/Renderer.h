#pragma once

#include <d3d11.h>
#include <Windows.h>
#include <iostream>

#include "../Application/Window.h"
#include "../Application/Scene.h"

class Renderer
{
private:
	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;
	IDXGISwapChain* swapChain;

	// Shaders

	D3D11_VIEWPORT viewport;
	ID3D11RenderTargetView* backBufferRTV;

	ID3D11Texture2D* dsTexture;				
	ID3D11DepthStencilView* dsView;
	ID3D11SamplerState* sampler;
	ID3D11Texture2D* texture;				
	ID3D11ShaderResourceView* texSRV;		
	
	// Unordered Access View

	// Buffers

	Window* window;

	float it;

	bool createInterfaces();
	bool createViews();
	bool createDepthStencil();
	void createViewport();
	bool createSamplerState();
	
public:
	Renderer();
	~Renderer();

	void init(Window& window);
	void render(Scene& scene);

};