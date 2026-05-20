#include "RenderFrameworks/RenderFramework.h"
#include "RenderFrameworks/RenderFramework_Dx11.h"
#include "RenderFrameworks/RenderFramework_OGL3.h"
std::unique_ptr<RenderFramework> g_renderFramework;

void CreateRenderFramework(char** argv, int argc) {
	if (argc > 1) {
		std::string api = argv[1];
		if (api == "-dx11") {
			g_renderFramework = std::make_unique<RenderFramework_Dx11>();
			return;
		}
		else if (api == "-ogl") {
			g_renderFramework = std::make_unique< RenderFramework_OGL3>();
			return;
		}
	}
	// else auto detect default to dx11 on windows and ogl on other platforms
#ifdef _WIN32
	g_renderFramework = std::make_unique<RenderFramework_Dx11>();
#else
	g_renderFramework = std::make_unique< RenderFramework_OGL3>();
#endif
}