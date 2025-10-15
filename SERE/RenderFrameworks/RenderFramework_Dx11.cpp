#include "RenderFrameworks/RenderFramework_Dx11.h"
#include "ThirdParty/DDSTextureLoader11.h"
#include "Imgui/imgui_impl_dx11.h"
#include "Imgui/imgui_impl_win32.h"

#include <fstream>

#include <d3dcompiler.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")


UINT g_WinResizeWidth = 0, g_WinResizeHeight = 0;


// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
			return 0;
		g_WinResizeWidth = (UINT)LOWORD(lParam); // Queue resize
		g_WinResizeHeight = (UINT)HIWORD(lParam);
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}


RenderFramework_Dx11::RenderFramework_Dx11() {
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr),::LoadIcon(nullptr,MAKEINTRESOURCE(1)) , nullptr, nullptr, nullptr, L"SERE", ::LoadIcon(nullptr,MAKEINTRESOURCE(1))};
    ::RegisterClassExW(&wc);
    hwnd = ::CreateWindowW(wc.lpszClassName, L"SERE", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D())
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWMAXIMIZED);
    ::UpdateWindow(hwnd);

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);


}

RenderFramework_Dx11::~RenderFramework_Dx11() {

	CleanupDeviceD3D();
	::DestroyWindow(hwnd);
	::UnregisterClassW(wc.lpszClassName, wc.hInstance);
}

void RenderFramework_Dx11::ImGuiDeInit() {
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
}

bool RenderFramework_Dx11::ShouldMainLoopRun() {
	MSG msg;
	while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
			return false;
	}
	return true;
}

bool RenderFramework_Dx11::CreateDeviceD3D()
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}


void RenderFramework_Dx11::CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void RenderFramework_Dx11::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void RenderFramework_Dx11::CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}



bool RenderFramework_Dx11::ImGuiStartFrame() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();

    if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
    {
        ::Sleep(10);
        return false;
    }
    g_SwapChainOccluded = false;

    // Handle window resize (we don't resize directly in the WM_SIZE handler)
    if (g_WinResizeWidth != 0 && g_WinResizeHeight != 0)
    {
        CleanupRenderTarget();
        g_pSwapChain->ResizeBuffers(0, g_WinResizeWidth, g_WinResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
        g_WinResizeWidth = 0;
		g_WinResizeHeight = 0;
        CreateRenderTarget();
    }
    return true;
}

void RenderFramework_Dx11::ImGuiEndFrame() {
    const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
    g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Present
    //HRESULT hr = g_pSwapChain->Present(1, 0);   // Present with vsync
    HRESULT hr = g_pSwapChain->Present(0, 0); // Present without vsync
    g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
}



size_t RenderFramework_Dx11::LoadTexture(fs::path& path) {
    Texture tex;
    DirectX::CreateDDSTextureFromFile(g_pd3dDevice,path.wstring().c_str(), &tex.resource, &tex.view);
    size_t res = textures.size();
    textures.push_back(tex);
    return res;
}


size_t RenderFramework_Dx11::CreateShaderDataBuffer(std::vector<ShaderSizeData_t> data){
    D3D11_SUBRESOURCE_DATA boundSubresoureDesc;
    D3D11_SHADER_RESOURCE_VIEW_DESC boundsShaderResourceViewDesc;
    D3D11_BUFFER_DESC boundsBufferDesc;

    Buffer buf;

    boundsBufferDesc.ByteWidth = sizeof(ShaderSizeData_t) * data.size();
    boundsBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    boundsBufferDesc.MiscFlags = 64;
    boundsBufferDesc.StructureByteStride = sizeof(ShaderSizeData_t);
    boundsBufferDesc.BindFlags =  8;
    boundsBufferDesc.CPUAccessFlags = 0;

    boundSubresoureDesc.pSysMem = data.data();
    boundSubresoureDesc.SysMemPitch = 0;
    boundSubresoureDesc.SysMemSlicePitch = 0;

    if (HRESULT res = g_pd3dDevice->CreateBuffer(&boundsBufferDesc, &boundSubresoureDesc, &buf.buffer); res || (buf.buffer ==NULL)) {
        printf("D3D11Decive::CreateBuffer returned 0x%x\n",(uint32_t)res);
    };
    boundsShaderResourceViewDesc.Format = DXGI_FORMAT_UNKNOWN;
    boundsShaderResourceViewDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
    boundsShaderResourceViewDesc.Buffer.FirstElement = 0;
    boundsShaderResourceViewDesc.Buffer.NumElements = data.size();
    if (HRESULT res = g_pd3dDevice->CreateShaderResourceView(buf.buffer, &boundsShaderResourceViewDesc, &buf.view); res) {
        printf("D3D11Decive::CreateShaderResourceView returned 0x%x\n",(uint32_t)res);
    }
    size_t res = buffers.size();
    buffers.push_back(buf);
    return res;

}

void RenderFramework_Dx11::RuiCreatePipeline(Vector2 size) {
	D3D11_BUFFER_DESC bufferInit{};
	bufferInit.ByteWidth = 576;
	bufferInit.Usage = D3D11_USAGE_DEFAULT;
	bufferInit.BindFlags = 4;
	bufferInit.CPUAccessFlags = 0;
	bufferInit.MiscFlags = 0;
	bufferInit.StructureByteStride = 0;
	g_pd3dDevice->CreateBuffer(&bufferInit, 0i64, &commonPerCameraBuffer);
	bufferInit.ByteWidth = 208;
	bufferInit.Usage = D3D11_USAGE_DYNAMIC;
	bufferInit.BindFlags = 4;
	bufferInit.CPUAccessFlags = 0x10000;
	bufferInit.MiscFlags = 0;
	bufferInit.StructureByteStride = 0;
	g_pd3dDevice->CreateBuffer(&bufferInit, 0i64, &modelInstanceBuffer);



	CBufCommonPerCamera cam{};

	cam.c_cameraRelativeToClip.a.x = 0.0010416672f;
	cam.c_cameraRelativeToClip.a.w = -1.0f;
	cam.c_cameraRelativeToClip.b.y = -0.00185185182f;
	cam.c_cameraRelativeToClip.b.w = 1.0f;
	cam.c_cameraRelativeToClip.c.z = -0.5f;
	cam.c_cameraRelativeToClip.c.w = 0.5f;
	cam.c_cameraRelativeToClip.d.w = 1.0f;
	cam.c_cameraRelativeToClipPrevFrame.a.x = 1.0f;
	cam.c_cameraRelativeToClipPrevFrame.b.y = 1.0f;
	cam.c_cameraRelativeToClipPrevFrame.c.z = 1.0f;
	cam.c_cameraRelativeToClipPrevFrame.d.w = 1.0f;
	cam.c_envMapLightScale = 1.0f;
	cam.c_renderTargetSize.x = 1920.0f;
	cam.c_renderTargetSize.y = 1080.0f;
	cam.c_rcpRenderTargetSize.x = 0.000520833360f;
	cam.c_rcpRenderTargetSize.y = 0.000925925910f;
	cam.c_numCoverageSamples = 1.0f;
	cam.c_rcpNumCoverageSamples = 1.0f;
	cam.c_cloudRelConst.x = 0.5f;
	cam.c_cloudRelConst.y = 0.5f;
	cam.c_useRealTimeLighting = 1.0f;
	cam.c_maxLightingValue = 5.0f;
	cam.c_viewportMaxZ = 1.0f;
	cam.c_viewportScale.x = 1.0f;
	cam.c_viewportScale.y = 1.0f;
	cam.c_rcpViewportScale.x = 1.0f;
	cam.c_rcpViewportScale.y = 1.0f;
	cam.c_framebufferViewportScale.x = 1.0f;
	cam.c_framebufferViewportScale.y = 1.0f;
	cam.c_rcpFramebufferViewportScale.x = 1.0f;
	cam.c_rcpFramebufferViewportScale.y = 1.0f;
	g_pd3dDeviceContext->UpdateSubresource(commonPerCameraBuffer, 0, 0, &cam, sizeof(cam), sizeof(cam));
	D3D11_MAPPED_SUBRESOURCE map;
	g_pd3dDeviceContext->Map(modelInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	ModelInstance* inst = (ModelInstance*)map.pData;
	memset(inst, 0, sizeof(ModelInstance));
	inst->objectToCameraRelative.a.x = 1.0f;
	inst->objectToCameraRelative.b.y = 1.0f;
	inst->objectToCameraRelative.c.z = 1.0f;
	inst->objectToCameraRelativePrevFrame.a.x = 1.0f;
	inst->objectToCameraRelativePrevFrame.b.y = 1.0f;
	inst->objectToCameraRelativePrevFrame.c.z = 1.0f;
	inst->diffuseModulation.x = 0.999999940f;
	inst->diffuseModulation.y = 0.999999940f;
	inst->diffuseModulation.z = 0.999999940f;
	inst->diffuseModulation.w = 1.0f;
	g_pd3dDeviceContext->Unmap(modelInstanceBuffer, 0);


	bufferInit.ByteWidth = sizeof(uint16_t) * 0x6000;
	bufferInit.Usage = D3D11_USAGE_DYNAMIC;
	bufferInit.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferInit.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferInit.MiscFlags = 0;
	bufferInit.StructureByteStride = 0;
	g_pd3dDevice->CreateBuffer(&bufferInit, 0, &indexBuffer);
	bufferInit.ByteWidth = sizeof(Vertex_t) * 0x4000;
	bufferInit.Usage = D3D11_USAGE_DYNAMIC;
	bufferInit.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferInit.MiscFlags = 0;
	bufferInit.StructureByteStride = 0;
	g_pd3dDevice->CreateBuffer(&bufferInit, 0, &vertexBuffer);
	bufferInit.ByteWidth = sizeof(StyleDescriptorShader_t) * 0x200;
	bufferInit.Usage = D3D11_USAGE_DYNAMIC;
	bufferInit.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bufferInit.MiscFlags = 0x40;
	bufferInit.StructureByteStride = sizeof(StyleDescriptorShader_t);
	g_pd3dDevice->CreateBuffer(&bufferInit, 0, &styleDescBuffer);
	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewInit{};
	resourceViewInit.Format = DXGI_FORMAT_UNKNOWN;
	resourceViewInit.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
	resourceViewInit.Buffer.FirstElement = 0;
	resourceViewInit.Buffer.NumElements = 0x200;
	g_pd3dDevice->CreateShaderResourceView(styleDescBuffer, &resourceViewInit, &styleDescriptorResourceView);
	D3D11_SAMPLER_DESC samplerInit{};
	samplerInit.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerInit.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerInit.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerInit.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerInit.MipLODBias = 0.0f;
	samplerInit.MaxAnisotropy = 0;
	samplerInit.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerInit.BorderColor[0] = 0.0f;
	samplerInit.BorderColor[1] = 0.0f;
	samplerInit.BorderColor[2] = 0.0f;
	samplerInit.BorderColor[3] = 0.0f;
	samplerInit.MinLOD = 0.0f;
	samplerInit.MaxLOD = 3.40282347e+38f;
	g_pd3dDevice->CreateSamplerState(&samplerInit, &samplerState);

	D3D11_BLEND_DESC blendDesc;
	memset(&blendDesc,0,sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	g_pd3dDevice->CreateBlendState(&blendDesc,&blendState);


	std::vector<char> vertexByteCode;

	std::ifstream vertexFile{"./Assets/Shader/ui_vs.fxc",std::ios::binary};
	vertexFile.seekg(0,std::ios::end);
	vertexByteCode.resize(vertexFile.tellg());
	vertexFile.seekg(0);
	vertexFile.read(vertexByteCode.data(),vertexByteCode.size());
	vertexFile.close();



	HRESULT res = g_pd3dDevice->CreateVertexShader(vertexByteCode.data(), vertexByteCode.size(), 0, &vertexShader);
	if (res) {
		printf("Errror creating Vertex Shader\n");
		return;
	}
	ID3DBlob *inputSignatureBlob;
	D3DGetBlobPart(vertexByteCode.data(), vertexByteCode.size(), D3D_BLOB_INPUT_SIGNATURE_BLOB, 0, &inputSignatureBlob);

	D3D11_INPUT_ELEMENT_DESC elementDescriptors[4];
	memset(elementDescriptors,0,sizeof(elementDescriptors));

	elementDescriptors[0].SemanticName = "POSITION";
	elementDescriptors[0].SemanticIndex = 0;
	elementDescriptors[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elementDescriptors[0].InputSlot = 0;
	elementDescriptors[0].AlignedByteOffset = 0;
	elementDescriptors[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elementDescriptors[0].InstanceDataStepRate = 0;

	elementDescriptors[1].SemanticName = "TEXCOORD";
	elementDescriptors[1].SemanticIndex = 1;
	elementDescriptors[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elementDescriptors[1].InputSlot = 0;
	elementDescriptors[1].AlignedByteOffset = 0x18;
	elementDescriptors[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elementDescriptors[1].InstanceDataStepRate = 0;

	elementDescriptors[2].SemanticName = "TEXCOORD";
	elementDescriptors[2].SemanticIndex = 2;
	elementDescriptors[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	elementDescriptors[2].InputSlot = 0;
	elementDescriptors[2].AlignedByteOffset = 0x28;
	elementDescriptors[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elementDescriptors[2].InstanceDataStepRate = 0;

	elementDescriptors[3].SemanticName = "TEXCOORD";
	elementDescriptors[3].SemanticIndex = 3;
	elementDescriptors[3].Format = DXGI_FORMAT_R16G16B16A16_SINT;
	elementDescriptors[3].InputSlot = 0;
	elementDescriptors[3].AlignedByteOffset = 0x30;
	elementDescriptors[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elementDescriptors[3].InstanceDataStepRate = 0;


	g_pd3dDevice->CreateInputLayout( elementDescriptors, 4, inputSignatureBlob->GetBufferPointer(), inputSignatureBlob->GetBufferSize(), &shaderLayout);

	//pixel shader

	std::vector<char> pixelByteCode;
	std::ifstream pixelFile{"./Assets/Shader/ui_ps.fxc",std::ios::binary};
	pixelFile.seekg(0,std::ios::end);
	pixelByteCode.resize(pixelFile.tellg());
	pixelFile.seekg(0);
	pixelFile.read(pixelByteCode.data(),pixelByteCode.size());
	pixelFile.close();

	res = g_pd3dDevice->CreatePixelShader(pixelByteCode.data(), pixelByteCode.size(),0,&pixelShader);
	if (res) {
		printf("Error creating Pixel shader\n");
		return;
	}

	D3D11_TEXTURE2D_DESC texture_desc = {};
	texture_desc.Width = 1920;
	texture_desc.Height = 1080;
	texture_desc.MipLevels = 1;
	texture_desc.ArraySize = 1;
	texture_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texture_desc.SampleDesc.Count = 1;
	texture_desc.SampleDesc.Quality = 0;
	texture_desc.Usage = D3D11_USAGE_DEFAULT;
	texture_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texture_desc.CPUAccessFlags = 0;
	texture_desc.MiscFlags = 0;

	g_pd3dDevice->CreateTexture2D(&texture_desc, 0, &targetTexture);

	D3D11_RENDER_TARGET_VIEW_DESC target_view_desc = {};
	target_view_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	target_view_desc.Texture2D.MipSlice = 0;

	g_pd3dDevice->CreateRenderTargetView(targetTexture, &target_view_desc, &targetView);

	// Depth stencil
	D3D11_TEXTURE2D_DESC depth_texture_desc = {};
	depth_texture_desc.Width = 1920;
	depth_texture_desc.Height = 1080;
	depth_texture_desc.MipLevels = 1;
	depth_texture_desc.ArraySize = 1;
	depth_texture_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_texture_desc.SampleDesc.Count = 1;
	depth_texture_desc.SampleDesc.Quality = 0;
	depth_texture_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;


	g_pd3dDevice->CreateTexture2D(&depth_texture_desc, nullptr, &depthTexture);
	g_pd3dDevice->CreateDepthStencilView(depthTexture, nullptr, &depthStencil);

	D3D11_SHADER_RESOURCE_VIEW_DESC view_desc = {};
	view_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	view_desc.Texture2D.MostDetailedMip = 0;
	view_desc.Texture2D.MipLevels = 1;

	g_pd3dDevice->CreateShaderResourceView(targetTexture, &view_desc, &targetResourceView);

	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = false;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	g_pd3dDevice->CreateRasterizerState(&rasterDesc, &rasterState);

	viewport.Width = size.x;
	viewport.Height = size.y;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xff;
	depthStencilDesc.StencilWriteMask = 0xff;

	// stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	g_pd3dDevice->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);

}

void RenderFramework_Dx11::RuiBindPipeline() {
	g_pd3dDeviceContext->OMSetDepthStencilState(depthStencilState, 1);
	g_pd3dDeviceContext->RSSetState(rasterState);
	g_pd3dDeviceContext->RSSetViewports(1,&viewport);

	g_pd3dDeviceContext->OMSetRenderTargets(1,&targetView,depthStencil);
	g_pd3dDeviceContext->OMSetBlendState(blendState,NULL,0xFFFFFFFF);

	g_pd3dDeviceContext->VSSetConstantBuffers(2i64, 1i64, &commonPerCameraBuffer);
	g_pd3dDeviceContext->PSSetConstantBuffers(2i64, 1i64, &commonPerCameraBuffer);
	
	g_pd3dDeviceContext->VSSetConstantBuffers(3i64, 1i64, &modelInstanceBuffer);
	g_pd3dDeviceContext->PSSetConstantBuffers(3i64, 1i64, &modelInstanceBuffer);

	g_pd3dDeviceContext->IASetInputLayout(shaderLayout);
	g_pd3dDeviceContext->VSSetShader(vertexShader,0,0);
	g_pd3dDeviceContext->PSSetShader(pixelShader,0,0);
	g_pd3dDeviceContext->PSSetSamplers(0,1,&samplerState);
	g_pd3dDeviceContext->IASetIndexBuffer(indexBuffer,DXGI_FORMAT_R16_UINT,0);
	UINT strides = 56;
	UINT offsets = 0;
	g_pd3dDeviceContext->IASetVertexBuffers(0,1,&vertexBuffer,&strides,&offsets);
	g_pd3dDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void RenderFramework_Dx11::RuiClearFrame() {
	float color[] = {.1f,.1f,.1f,1.f};
	g_pd3dDeviceContext->ClearRenderTargetView(targetView,color);
	g_pd3dDeviceContext->ClearDepthStencilView(depthStencil,D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

}



void RenderFramework_Dx11::RuiWriteIndexBuffer(std::vector<uint16_t>& data) {
	D3D11_MAPPED_SUBRESOURCE map;
	g_pd3dDeviceContext->Map(indexBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&map);
	memcpy(map.pData,data.data(),data.size()*sizeof(uint16_t));
	g_pd3dDeviceContext->Unmap(indexBuffer,0);
}
void RenderFramework_Dx11::RuiWriteVertexBuffer(std::vector<Vertex_t>& data) {
	D3D11_MAPPED_SUBRESOURCE map;
	g_pd3dDeviceContext->Map(vertexBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&map);
	memcpy(map.pData,data.data(),data.size()*sizeof(Vertex_t));
	g_pd3dDeviceContext->Unmap(vertexBuffer,0);
}
void RenderFramework_Dx11::RuiWriteStyleBuffer(std::vector<StyleDescriptorShader_t>& data) {
	D3D11_MAPPED_SUBRESOURCE map;
	g_pd3dDeviceContext->Map(styleDescBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&map);
	memcpy(map.pData,data.data(),data.size()*sizeof(StyleDescriptorShader_t));
	g_pd3dDeviceContext->Unmap(styleDescBuffer,0);
}

void RenderFramework_Dx11::DrawIndexed(uint32_t count,uint32_t start,size_t* resources) {
	ID3D11ShaderResourceView* resourceViews[5];
	memset(resourceViews,0,sizeof(resourceViews));
	if (resources[0] != ~0) {
		resourceViews[0] = textures[resources[0]].view;
	}
	if (resources[1] != ~0) {
		resourceViews[1] = textures[resources[1]].view;
	}
	if (resources[2] != ~0) {
		resourceViews[2] = buffers[resources[2]].view;
	}
	if (resources[3] != ~0) {
		resourceViews[3] = buffers[resources[3]].view;
	}
	resourceViews[4] = styleDescriptorResourceView;
	g_pd3dDeviceContext->PSSetShaderResources(0,5,resourceViews);
	g_pd3dDeviceContext->DrawIndexed(count,start,0);
}

void* RenderFramework_Dx11::GetTextureView(size_t id) {
	return textures[id].view;
}

void* RenderFramework_Dx11::GetRuiView() {
	return targetResourceView;
}