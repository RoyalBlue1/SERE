#pragma once

#include "RenderFrameWork.h"
#include <d3d11.h>


static const DXGI_FORMAT s_PakToDxgiFormat[] =
{
	DXGI_FORMAT_BC1_UNORM,
	DXGI_FORMAT_BC1_UNORM_SRGB,
	DXGI_FORMAT_BC2_UNORM,
	DXGI_FORMAT_BC2_UNORM_SRGB,
	DXGI_FORMAT_BC3_UNORM,
	DXGI_FORMAT_BC3_UNORM_SRGB,
	DXGI_FORMAT_BC4_UNORM,
	DXGI_FORMAT_BC4_SNORM,
	DXGI_FORMAT_BC5_UNORM,
	DXGI_FORMAT_BC5_SNORM,
	DXGI_FORMAT_BC6H_UF16,
	DXGI_FORMAT_BC6H_SF16,
	DXGI_FORMAT_BC7_UNORM,
	DXGI_FORMAT_BC7_UNORM_SRGB,
	DXGI_FORMAT_R32G32B32A32_FLOAT,
	DXGI_FORMAT_R32G32B32A32_UINT,
	DXGI_FORMAT_R32G32B32A32_SINT,
	DXGI_FORMAT_R32G32B32_FLOAT,
	DXGI_FORMAT_R32G32B32_UINT,
	DXGI_FORMAT_R32G32B32_SINT,
	DXGI_FORMAT_R16G16B16A16_FLOAT,
	DXGI_FORMAT_R16G16B16A16_UNORM,
	DXGI_FORMAT_R16G16B16A16_UINT,
	DXGI_FORMAT_R16G16B16A16_SNORM,
	DXGI_FORMAT_R16G16B16A16_SINT,
	DXGI_FORMAT_R32G32_FLOAT,
	DXGI_FORMAT_R32G32_UINT,
	DXGI_FORMAT_R32G32_SINT,
	DXGI_FORMAT_R10G10B10A2_UNORM,
	DXGI_FORMAT_R10G10B10A2_UINT,
	DXGI_FORMAT_R11G11B10_FLOAT,
	DXGI_FORMAT_R8G8B8A8_UNORM,
	DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
	DXGI_FORMAT_R8G8B8A8_UINT,
	DXGI_FORMAT_R8G8B8A8_SNORM,
	DXGI_FORMAT_R8G8B8A8_SINT,
	DXGI_FORMAT_R16G16_FLOAT,
	DXGI_FORMAT_R16G16_UNORM,
	DXGI_FORMAT_R16G16_UINT,
	DXGI_FORMAT_R16G16_SNORM,
	DXGI_FORMAT_R16G16_SINT,
	DXGI_FORMAT_R32_FLOAT,
	DXGI_FORMAT_R32_UINT,
	DXGI_FORMAT_R32_SINT,
	DXGI_FORMAT_R8G8_UNORM,
	DXGI_FORMAT_R8G8_UINT,
	DXGI_FORMAT_R8G8_SNORM,
	DXGI_FORMAT_R8G8_SINT,
	DXGI_FORMAT_R16_FLOAT,
	DXGI_FORMAT_R16_UNORM,
	DXGI_FORMAT_R16_UINT,
	DXGI_FORMAT_R16_SNORM,
	DXGI_FORMAT_R16_SINT,
	DXGI_FORMAT_R8_UNORM,
	DXGI_FORMAT_R8_UINT,
	DXGI_FORMAT_R8_SNORM,
	DXGI_FORMAT_R8_SINT,
	DXGI_FORMAT_A8_UNORM,
	DXGI_FORMAT_R9G9B9E5_SHAREDEXP,
	DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM,
	DXGI_FORMAT_D32_FLOAT,
	DXGI_FORMAT_D16_UNORM,
};

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
	size_t CreateTextureFromData(void* data,uint32_t width,uint32_t height,uint16_t format,uint32_t pitch,uint32_t slicePitch);
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
