#include "RenderFrameworks/RenderFramework.h"
#include "RenderFrameworks/RenderFramework_Dx11.h"
std::unique_ptr<RenderFramework> g_renderFramework;

void CreateRenderFramework(char** argv, int argc) {
	g_renderFramework = std::make_unique<RenderFramework_Dx11>();
}