#pragma once
#include <d3d11.h>
#include <Windows.h>
#include <iostream>

#include "../Application/Window.h"
#include "../Application/Scene.h"
#include "Vertex.h"
#include "Material.h"

class Renderer
{
private:
	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;
	IDXGISwapChain* swapChain;

	// Shaders
	ID3D11VertexShader* vertexShader;
	ID3D11InputLayout* inputLayout;
	ID3D11PixelShader* pixelShader;
	ID3D11ComputeShader* computeShader;

	D3D11_VIEWPORT viewport;
	ID3D11RenderTargetView* backBufferRTV;
	ID3D11UnorderedAccessView* backBufferUAV;
	ID3D11DepthStencilView* dsView;
	
	// Buffers
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* materialBuffer;

	ID3D11RenderTargetView* gBuffersRTV[8];
	ID3D11ShaderResourceView* gBuffersSRV[8];

	ID3D11Texture2D* gBuffTex0;
	ID3D11ShaderResourceView* gBuffSRV0;
	ID3D11RenderTargetView* gBuffRTV0;

	ID3D11Texture2D* gBuffTex1;
	ID3D11ShaderResourceView* gBuffSRV1;
	ID3D11RenderTargetView* gBuffRTV1;

	ID3D11Texture2D* gBuffTex2;
	ID3D11ShaderResourceView* gBuffSRV2;
	ID3D11RenderTargetView* gBuffRTV2;

	ID3D11Texture2D* gBuffTex3;
	ID3D11ShaderResourceView* gBuffSRV3;
	ID3D11RenderTargetView* gBuffRTV3;

	// Unordered Access View

	ID3D11Texture2D* dsTexture;
	ID3D11SamplerState* sampler;
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* texSRV;

	Window* window;
	float it;
	unsigned int numVertices;
	unsigned int numGBuffers;

	bool createInterfaces();
	bool createViews();
	bool createDepthStencil();
	void createViewport();
	bool createSamplerState();
	bool createShaders();
	bool createVertexBuffer();
	bool createMatrixBuffer();
	bool createGBuffers();
	bool createMaterialBuffer();
	
public:
	Renderer();
	~Renderer();

	void init(Window& window);
	void render(Scene& scene);
	void updateMatrices(const float &deltaTime);
};