#include "RenderFramework_OGL3.h"
#include <Imgui/imgui_impl_opengl3.h>
#include <Imgui/imgui_impl_sdl3.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

uint32_t WinResizeWidth = 0, WinResizeHeight = 0;

#define MSAA_SAMPLES 16


static bool IsMultisampleCountSupported(GLenum internalFormat, GLsizei samples)
{
    GLint sampleCountCount = 0;
    glGetInternalformativ(GL_TEXTURE_2D_MULTISAMPLE, internalFormat, GL_NUM_SAMPLE_COUNTS, 1, &sampleCountCount);
    if (sampleCountCount <= 0) {
        return false;
    }

    std::vector<GLint> sampleCounts(sampleCountCount);
    glGetInternalformativ(
        GL_TEXTURE_2D_MULTISAMPLE,
        internalFormat,
        GL_SAMPLES,
        sampleCountCount,
        sampleCounts.data()
    );

    for (GLint sampleCount : sampleCounts) {
        if (sampleCount == samples) {
            return true;
        }
    }

    return false;
}

static GLsizei GetSupportedRuiMsaaSamples()
{
    for (GLsizei samples = MSAA_SAMPLES; samples > 1; --samples) {
        if (IsMultisampleCountSupported(GL_RGBA32F, samples) &&
            IsMultisampleCountSupported(GL_DEPTH24_STENCIL8, samples)) {
            return samples;
        }
    }

    return 1;
}

static void ThrowSdlError(const char* message)
{
    const char* error = SDL_GetError();
    std::string fullMessage = std::string(message) + ": " + (error && *error ? error : "unknown SDL error");
    SDL_Log("%s", fullMessage.c_str());
    throw std::runtime_error(fullMessage);
}


RenderFramework_OGL3::RenderFramework_OGL3()
{

    window = nullptr;
    glContext = nullptr;
    fbo = 0;
    depthStencilTexture = 0;
    colorTexture = 0;
    msaaFbo = 0;
    msaaColorTexture = 0;
    msaaDepthStencilTexture = 0;
    msaaSamples = 1;
    shouldUpdateMsaa = false;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        ThrowSdlError("Unable to initialize SDL");
    }
    SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_OPENGL;
	if(!SDL_GL_LoadLibrary(nullptr)) { // Load the default OpenGL library
        ThrowSdlError("Failed to load OpenGL library");
    }
	const char* glsl_version = "#version 450 core";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);

	window = SDL_CreateWindow("SERE", 1280, 800, window_flags);
    if (window == nullptr) {
        ThrowSdlError("Failed to create SDL window");
    }

	glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr) {
		ThrowSdlError("Failed to create OpenGL context");
	}
    if (!SDL_GL_MakeCurrent(window, glContext)) {
        ThrowSdlError("Failed to make OpenGL context current");
    }

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        const char* error = reinterpret_cast<const char*>(glewGetErrorString(glewError));
        std::string message = std::string("Error initializing GLEW: ") + (error ? error : "unknown GLEW error");
        SDL_Log("%s", message.c_str());
        throw std::runtime_error(message);
    }
	SDL_PropertiesID props = SDL_GetWindowProperties(window);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);
    SDL_StartTextInput(window);
    SDL_MaximizeWindow(window);
    SDL_SetHint(SDL_HINT_WINDOWS_ENABLE_MENU_MNEMONICS, "0"); // Disable ALT key mnemonics to avoid interfering with ImGui input handling
    
    ImGui_ImplSDL3_InitForOpenGL(window, glContext);
	if (!ImGui_ImplOpenGL3_Init(glsl_version)) {
		SDL_Log("Failed to initialize ImGui OpenGL3 backend");
	}

	
}

RenderFramework_OGL3::~RenderFramework_OGL3()
{
    if (glContext) {
        SDL_GL_DestroyContext(glContext);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
}

bool RenderFramework_OGL3::ShouldMainLoopRun()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_KEY_DOWN)
		{
			if (event.key.key == SDLK_LALT || event.key.key == SDLK_RALT)
				break;
		}
		if (event.type == SDL_EVENT_QUIT)
			return false;
		if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
			return false;
		if (event.type == SDL_EVENT_WINDOW_RESIZED && event.window.windowID == SDL_GetWindowID(window))
		{
			WinResizeWidth = event.window.data1;
			WinResizeHeight = event.window.data2;
		}
		ImGui_ImplSDL3_ProcessEvent(&event);
	}
	return true;
}

bool RenderFramework_OGL3::ImGuiStartFrame()
{
	if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
	{
		SDL_Delay(10);
		return false;
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	
	return true;
}

void RenderFramework_OGL3::ImGuiEndFrame()
{
	const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
	glClearColor(clear_color_with_alpha[0], clear_color_with_alpha[1], clear_color_with_alpha[2], clear_color_with_alpha[3]);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(window);
}

void RenderFramework_OGL3::ImGuiDeInit()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
}

void RenderFramework_OGL3::RuiClearFrame()
{
    glBindFramebuffer(GL_FRAMEBUFFER, GetRuiRenderFramebuffer());

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClearDepth(1.0);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    shouldUpdateMsaa = msaaFbo != 0;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, colorTexture);

}


void RenderFramework_OGL3::DrawIndexed(uint32_t count, uint32_t start, size_t * resources)
{
    GLuint resourceViews[5];
    memset(resourceViews, 0, sizeof(resourceViews));
    if (resources[0] != ~0) {
        resourceViews[0] = textures[resources[0]].id;
    }
    if (resources[1] != ~0) {
        resourceViews[1] = textures[resources[1]].id;
    }
    if (resources[2] != ~0) {
        resourceViews[2] = buffers[resources[2]].id;
    } 
    if (resources[3] != ~0) {
        resourceViews[3] = buffers[resources[3]].id;
    }

	// bind and acivate the textures 
    for (int i = 0; i < 2; i++) {
        if (resources[i] != ~0) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, resourceViews[i]);
        }
	} 

	for (int i = 2; i < 4; i++) { 
        if (resources[i] != ~0) {
			// font and image bounds are bound to slots 5 and 6 in the shader, so we bind them to 5 and 6 here
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i + 3, resourceViews[i]);
        }
    }
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, styleDescSSBO);
    glBindFramebuffer(GL_FRAMEBUFFER, GetRuiRenderFramebuffer());
    if(glGetError() != GL_NO_ERROR)
        printf("OpenGL error occurred\n");

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer); 

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
        sizeof(Vertex_t), (void*)0x00);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
        sizeof(Vertex_t), (void*)0x18);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex_t), (void*)0x28);
     
    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 4, GL_SHORT,
        sizeof(Vertex_t), (void*)0x30); 
    glUseProgram(shaderProgram);
	glDrawElements(GL_TRIANGLES, (GLsizei)count, GL_UNSIGNED_SHORT, (void*)(start * sizeof(uint16_t)));
    glBindVertexArray(0);
    shouldUpdateMsaa = msaaFbo != 0;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
}

size_t RenderFramework_OGL3::CreateShaderDataBuffer(std::vector<ShaderSizeData_t> data)
{
	GLuint buffer;
	glGenBuffers(1, &buffer); 
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(ShaderSizeData_t), data.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	auto ret = buffers.size();
	buffers.push_back({ buffer });
    return ret;
}

size_t RenderFramework_OGL3::CreateTextureFromData(void* data, uint32_t width, uint32_t height, uint16_t format, uint32_t pitch, uint32_t slicePitch)
{
    const GLTextureFormat& fmt = s_PakToGLFormat[format];
    GLuint texture;

    glCreateTextures(GL_TEXTURE_2D, 1, &texture);

    // Immutable storage — equivalent to D3D11_USAGE_IMMUTABLE with 1 mip level
    glTextureStorage2D(texture, 1, fmt.internalFormat, width, height);

    if (fmt.compressed) {
        // For BCn/DXT formats — slicePitch is the total compressed data size
        glCompressedTextureSubImage2D(
            texture, 0,
            0, 0, width, height,
            fmt.internalFormat,
            static_cast<GLsizei>(slicePitch),
            data
        );
  
    }
    else {
        // GL_UNPACK_ROW_LENGTH is in pixels, not bytes
        const uint32_t rowLengthPixels = pitch / fmt.blockSize;

        glTextureSubImage2D(
            texture, 0,
            0, 0, width, height,
            fmt.format, fmt.type,
            data
        );

    }
    auto error = glGetError();
    if (error != GL_NO_ERROR) {
        glDeleteTextures(1, &texture);
        return ~0ULL;
    }

    size_t ret = textures.size();
    textures.emplace_back(texture);
	return ret;
}

size_t RenderFramework_OGL3::LoadTexture(fs::path& path)
{
	return size_t();
}

void RenderFramework_OGL3::RuiWriteIndexBuffer(std::vector<uint16_t>& data)
{
    //indexBuffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(uint16_t), data.data(), GL_STATIC_DRAW);
}

void RenderFramework_OGL3::RuiWriteVertexBuffer(std::vector<Vertex_t>&data)
{
    glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(Vertex_t), data.data(), GL_STATIC_DRAW);
}

void RenderFramework_OGL3::RuiWriteStyleBuffer(std::vector<StyleDescriptorShader_t>&data)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, styleDescSSBO);
    glBufferSubData(
        GL_SHADER_STORAGE_BUFFER,
        0,
        data.size() * sizeof(StyleDescriptorShader_t),
        data.data()
	);
}

void RenderFramework_OGL3::RuiBindPipeline()
{
    // OMSetDepthStencilState (stencil ref = 1)
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_ALWAYS);
    glEnable(GL_STENCIL_TEST);
    glStencilMaskSeparate(GL_FRONT_AND_BACK, 0xFF);
    glStencilFuncSeparate(GL_FRONT, GL_ALWAYS, 1, 0xFF); // ref=1
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_INCR, GL_KEEP);
    glStencilFuncSeparate(GL_BACK, GL_ALWAYS, 1, 0xFF);  // ref=1
    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_DECR, GL_KEEP);

    // RSSetState
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_SCISSOR_TEST);

    // RSSetViewports
    glViewport(
        (GLint)viewport.x,
        (GLint)viewport.y,
        (GLsizei)viewport.width,
        (GLsizei)viewport.height
    );
    glDepthRange(viewport.MinDepth, viewport.MaxDepth);

    // OMSetBlendState
    glEnable(GL_BLEND);
    glBlendFuncSeparate(
        GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
        GL_SRC_ALPHA, GL_DST_ALPHA
    );
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // VSSetConstantBuffers(2) + PSSetConstantBuffers(2)
    // In GL, UBO bindings are shared across all shader stages
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, commonPerCameraUBO);

    // VSSetConstantBuffers(3) + PSSetConstantBuffers(3)
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, modelInstanceUBO);

    // IASetInputLayout + IASetVertexBuffers + IASetIndexBuffer
    // All stored in the VAO from init
    glBindVertexArray(vao);

    // VSSetShader + PSSetShader
    glUseProgram(shaderProgram);

    // PSSetSamplers(0)
    glBindSampler(0, samplerState);
	glBindSampler(1, samplerState);

    
}

void RenderFramework_OGL3::RuiLoad(int width, int height)
{

  
    CBufCommonPerCamera cam{};

    cam.c_cameraRelativeToClip.a.x = 2.f;
    cam.c_cameraRelativeToClip.a.w = -1.0f;
    cam.c_cameraRelativeToClip.b.y = -2.f;
    cam.c_cameraRelativeToClip.b.w = 1.0f;
    cam.c_cameraRelativeToClip.c.z = 1.f;
    //cam.c_cameraRelativeToClip.c.w = 0.5f;
    cam.c_cameraRelativeToClip.d.w = 1.0f;
    cam.c_cameraRelativeToClipPrevFrame.a.x = 1.0f;
    cam.c_cameraRelativeToClipPrevFrame.b.y = 1.0f;
    cam.c_cameraRelativeToClipPrevFrame.c.z = 1.0f;
    cam.c_cameraRelativeToClipPrevFrame.d.w = 1.0f;
    cam.c_envMapLightScale = 1.0f;
    cam.c_renderTargetSize.x = width;
    cam.c_renderTargetSize.y = height;
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

    glGenBuffers(1, &commonPerCameraUBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, commonPerCameraUBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(cam), &cam, GL_STATIC_DRAW);

    ModelInstance inst{};
    //memset(inst, 0, sizeof(ModelInstance));
    inst.objectToCameraRelative.a.x = 1.0f;
    inst.objectToCameraRelative.b.y = 1.0f;
    inst.objectToCameraRelative.c.z = 1.0f;
    inst.objectToCameraRelativePrevFrame.a.x = 1.0f;
    inst.objectToCameraRelativePrevFrame.b.y = 1.0f;
    inst.objectToCameraRelativePrevFrame.c.z = 1.0f;
    inst.diffuseModulation.x = 0.999999940f;
    inst.diffuseModulation.y = 0.999999940f;
    inst.diffuseModulation.z = 0.999999940f;
    inst.diffuseModulation.w = 1.0f;


    glGenBuffers(1, &modelInstanceUBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, modelInstanceUBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(inst), &inst, GL_STATIC_DRAW);

    
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(uint16_t) * 0x6000,
        nullptr, GL_DYNAMIC_DRAW
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(Vertex_t) * 0x4000,
        nullptr, GL_DYNAMIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

   

    glGenBuffers(1, &styleDescSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, styleDescSSBO);
    glBufferData(
        GL_SHADER_STORAGE_BUFFER,
        sizeof(StyleDescriptorShader_t) * 0x200,
        nullptr, GL_DYNAMIC_DRAW
    );
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

 
    glGenSamplers(1, &samplerState);
    glSamplerParameteri(samplerState, GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_NEAREST);
    glSamplerParameteri(samplerState, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(samplerState, GL_TEXTURE_WRAP_S, GL_REPEAT);        // U = WRAP
    glSamplerParameteri(samplerState, GL_TEXTURE_WRAP_T, GL_REPEAT);        // V = WRAP
    glSamplerParameteri(samplerState, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // W = CLAMP
    glSamplerParameterf(samplerState, GL_TEXTURE_LOD_BIAS, 0.0f);
    glSamplerParameterf(samplerState, GL_TEXTURE_MIN_LOD, 0.0f);
    glSamplerParameterf(samplerState, GL_TEXTURE_MAX_LOD, FLT_MAX);

    glEnable(GL_BLEND);
    glBlendFuncSeparate(
        GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
        GL_SRC_ALPHA, GL_DST_ALPHA
    );
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);


    auto compileShader = [](GLenum type, const char* src) -> GLuint {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1,(const GLchar**)&src, nullptr);

        glCompileShader(shader);
        GLint ok;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
        if (!ok) {
            char log[4096];
            glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
            printf("Shader compile error: %s\n", log);
            return 0;
        }
        char log[512];
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        printf("Shader compiled %s\n", log);
        return shader;
        };

    std::ifstream vertexFile{ "./Assets/Shader/ui.vs" };
    std::vector<char> vertexBufferData; 
    vertexFile.seekg(0, std::ios::end);
    vertexBufferData.resize(vertexFile.tellg());
    vertexFile.seekg(0);
    vertexFile.read(vertexBufferData.data(), vertexBufferData.size());
    vertexFile.close();

    vertexBufferData.push_back('\0'); // Null-terminate the shader source

    vertexSource = vertexBufferData.data();

    std::ifstream fragFile{ "./Assets/Shader/ui.ps" };
    std::vector<char> fragBufferData;
    fragFile.seekg(0, std::ios::end);
    fragBufferData.resize(fragFile.tellg());
    fragFile.seekg(0);
    fragFile.read(fragBufferData.data(), fragBufferData.size());
    fragFile.close();

    fragBufferData.push_back('\0'); // Null-terminate the shader source

    fragmentSource = fragBufferData.data(); 

    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    shaderProgram = glCreateProgram();  
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    glDeleteShader(vs);
    glDeleteShader(fs);

    GLint linkOk;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkOk);
    if (!linkOk) {
        char log[512];
        glGetProgramInfoLog(shaderProgram, sizeof(log), nullptr, log);
        printf("Program link error: %s\n", log);
        return;
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex_t), (void*)0x00);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
        sizeof(Vertex_t), (void*)0x18);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
        sizeof(Vertex_t), (void*)0x28);

    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 4, GL_SHORT,
        sizeof(Vertex_t), (void*)0x30);

    glBindVertexArray(0);

    RuiReCreatePipeline(width, height);
}

GLuint RenderFramework_OGL3::GetRuiRenderFramebuffer() const
{
    return msaaFbo ? msaaFbo : fbo;
}

void RenderFramework_OGL3::ResolveRuiFramebuffer()
{
    if (!shouldUpdateMsaa || !msaaFbo || !fbo) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return;
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFbo);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glBlitFramebuffer(
        0, 0, viewport.width, viewport.height,
        0, 0, viewport.width, viewport.height,
        GL_COLOR_BUFFER_BIT,
        GL_NEAREST
    );
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    shouldUpdateMsaa = false;
}

void RenderFramework_OGL3::RuiReCreatePipeline(int width, int height)
{
    msaaSamples = GetSupportedRuiMsaaSamples();
    shouldUpdateMsaa = false;

    if (msaaFbo) {
        glDeleteFramebuffers(1, &msaaFbo);
        msaaFbo = 0;
    }
    if (msaaColorTexture) {
        glDeleteTextures(1, &msaaColorTexture);
        msaaColorTexture = 0;
    }
    if (msaaDepthStencilTexture) {
        glDeleteTextures(1, &msaaDepthStencilTexture);
        msaaDepthStencilTexture = 0;
    }
    if (fbo) {
        glDeleteFramebuffers(1, &fbo);
        fbo = 0;
    }
    if (colorTexture) {
        glDeleteTextures(1, &colorTexture);
        colorTexture = 0;
    }
    if (depthStencilTexture) {
        glDeleteTextures(1, &depthStencilTexture);
        depthStencilTexture = 0;
    }

   
    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA32F,
        width, height, 0,
        GL_RGBA, GL_FLOAT, nullptr
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    
    glGenTextures(1, &depthStencilTexture);
    glBindTexture(GL_TEXTURE_2D, depthStencilTexture);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8,
        width, height, 0,
        GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

   
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D, colorTexture, 0
    );
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
        GL_TEXTURE_2D, depthStencilTexture, 0
    );
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (msaaSamples > 1) {
        glGenTextures(1, &msaaColorTexture);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaaColorTexture);
        glTexImage2DMultisample(
            GL_TEXTURE_2D_MULTISAMPLE,
            msaaSamples,
            GL_RGBA32F,
            width,
            height,
            GL_TRUE
        );
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

        glGenTextures(1, &msaaDepthStencilTexture);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msaaDepthStencilTexture);
        glTexImage2DMultisample(
            GL_TEXTURE_2D_MULTISAMPLE,
            msaaSamples,
            GL_DEPTH24_STENCIL8,
            width,
            height,
            GL_TRUE
        );
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

        glGenFramebuffers(1, &msaaFbo);
        glBindFramebuffer(GL_FRAMEBUFFER, msaaFbo);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D_MULTISAMPLE, msaaColorTexture, 0
        );
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
            GL_TEXTURE_2D_MULTISAMPLE, msaaDepthStencilTexture, 0
        );
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        glReadBuffer(GL_COLOR_ATTACHMENT0);

        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

	viewport.x = 0;
	viewport.y = 0;
	viewport.width = width;
	viewport.height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
    
    glDisable(GL_CULL_FACE);                   
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  
    glDisable(GL_SCISSOR_TEST);                 
    glEnable(GL_MULTISAMPLE);
    glDisable(GL_LINE_SMOOTH);                  
    glEnable(GL_DEPTH_CLAMP);                   
    glDisable(GL_POLYGON_OFFSET_FILL);

    glViewport(
        (GLint)viewport.x,
        (GLint)viewport.y,
        (GLsizei)viewport.width,
        (GLsizei)viewport.height
	);
	glDepthRange(viewport.MinDepth, viewport.MaxDepth);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_ALWAYS);

    glEnable(GL_STENCIL_TEST);
    glStencilMaskSeparate(GL_FRONT_AND_BACK, 0xFF); 

    glStencilFuncSeparate(GL_FRONT, GL_ALWAYS, 0, 0xFF);
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_INCR, GL_KEEP);

    glStencilFuncSeparate(GL_BACK, GL_ALWAYS, 0, 0xFF);
    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_DECR, GL_KEEP);

}

void* RenderFramework_OGL3::GetTextureView(size_t id)
{
	return reinterpret_cast<void*>(static_cast<std::uintptr_t>(textures[id].id));
}

void* RenderFramework_OGL3::GetRuiView()
{
    ResolveRuiFramebuffer();
	return reinterpret_cast<void*>(static_cast<std::uintptr_t>(colorTexture));
}

void* RenderFramework_OGL3::GetWindow()
{
    return window;
}
