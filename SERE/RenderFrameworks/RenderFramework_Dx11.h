#pragma once

#include "RenderFrameWork.h"
#include <d3d11.h>


class RenderFramework_Dx11 : public RenderFramework {

	struct Buffer {
		ID3D11Buffer* buffer;
		ID3D11ShaderResourceView* view;
	};

	struct Texture {
		ID3D11Resource* resource;
		ID3D11ShaderResourceView* view;
	};

public:
	RenderFramework_Dx11();
	~RenderFramework_Dx11();

	bool ShouldMainLoopRun();
	
	bool ImGuiStartFrame();
	void ImGuiEndFrame();

	void ImGuiDeInit();

	void RuiClearFrame();

	void DrawIndexed(uint32_t count,uint32_t start,size_t* resources);
	size_t CreateShaderDataBuffer(std::vector<ShaderSizeData_t> data);

	size_t LoadTexture(fs::path& path);

	void RuiWriteIndexBuffer(std::vector<uint16_t>& data);
	void RuiWriteVertexBuffer(std::vector<Vertex_t>& data);
	void RuiWriteStyleBuffer(std::vector<StyleDescriptorShader_t>& data);


	void RuiBindPipeline();
	void RuiCreatePipeline(Vector2 size);

	void* GetTextureView(size_t id);
	void* GetRuiView();

private:
	ID3D11Device*            g_pd3dDevice = nullptr;
	ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
	IDXGISwapChain*          g_pSwapChain = nullptr;
	bool                     g_SwapChainOccluded = false;

	ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;
	WNDCLASSEXW wc;
	HWND hwnd;
	
	ID3D11SamplerState* samplerState;

	ID3D11BlendState* blendState;
	ID3D11Texture2D* targetTexture;
	ID3D11RenderTargetView* targetView;

	ID3D11Texture2D* depthTexture;
	ID3D11DepthStencilView* depthStencil;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11RasterizerState* rasterState;
	D3D11_VIEWPORT viewport;

	ID3D11Buffer* commonPerCameraBuffer;
	ID3D11Buffer* modelInstanceBuffer;

	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* styleDescBuffer;
	ID3D11ShaderResourceView* styleDescriptorResourceView;

	ID3D11VertexShader* vertexShader;
	ID3D11InputLayout *shaderLayout;
	ID3D11PixelShader* pixelShader;

	std::vector<Texture> textures;
	std::vector<Buffer> buffers;
	ID3D11ShaderResourceView* targetResourceView;

	
	bool CreateDeviceD3D();
	void CleanupDeviceD3D();
	void CreateRenderTarget();
	void CleanupRenderTarget();
	

	
};
