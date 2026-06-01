#include "RenderFramework.h"
#include <gl/glew.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <Windows.h>
#include <gl/GL.h>
struct GLTextureFormat {
    GLenum internalFormat;
    GLenum format;
    GLenum type;
    bool compressed;
    uint32_t blockSize; // bytes per block (compressed) or bytes per pixel
};

static const GLTextureFormat s_PakToGLFormat[] = {
    // BC1–BC7 compressed
    { GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,        GL_NONE, GL_NONE, true,  8  }, // BC1_UNORM
    { GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT,  GL_NONE, GL_NONE, true,  8  }, // BC1_UNORM_SRGB
    { GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,        GL_NONE, GL_NONE, true,  16 }, // BC2_UNORM
    { GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT,  GL_NONE, GL_NONE, true,  16 }, // BC2_UNORM_SRGB
    { GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,        GL_NONE, GL_NONE, true,  16 }, // BC3_UNORM
    { GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT,  GL_NONE, GL_NONE, true,  16 }, // BC3_UNORM_SRGB
    { GL_COMPRESSED_RED_RGTC1,                 GL_NONE, GL_NONE, true,  8  }, // BC4_UNORM
    { GL_COMPRESSED_SIGNED_RED_RGTC1,          GL_NONE, GL_NONE, true,  8  }, // BC4_SNORM
    { GL_COMPRESSED_RG_RGTC2,                  GL_NONE, GL_NONE, true,  16 }, // BC5_UNORM
    { GL_COMPRESSED_SIGNED_RG_RGTC2,           GL_NONE, GL_NONE, true,  16 }, // BC5_SNORM
    { GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT,   GL_NONE, GL_NONE, true,  16 }, // BC6H_UF16
    { GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT,     GL_NONE, GL_NONE, true,  16 }, // BC6H_SF16
    { GL_COMPRESSED_RGBA_BPTC_UNORM,           GL_NONE, GL_NONE, true,  16 }, // BC7_UNORM
    { GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM,     GL_NONE, GL_NONE, true,  16 }, // BC7_UNORM_SRGB

    // 128-bit
    { GL_RGBA32F,  GL_RGBA,         GL_FLOAT,        false, 16 }, // R32G32B32A32_FLOAT
    { GL_RGBA32UI, GL_RGBA_INTEGER, GL_UNSIGNED_INT,  false, 16 }, // R32G32B32A32_UINT
    { GL_RGBA32I,  GL_RGBA_INTEGER, GL_INT,           false, 16 }, // R32G32B32A32_SINT

    // 96-bit
    { GL_RGB32F,   GL_RGB,          GL_FLOAT,         false, 12 }, // R32G32B32_FLOAT
    { GL_RGB32UI,  GL_RGB_INTEGER,  GL_UNSIGNED_INT,  false, 12 }, // R32G32B32_UINT
    { GL_RGB32I,   GL_RGB_INTEGER,  GL_INT,           false, 12 }, // R32G32B32_SINT

    // 64-bit
    { GL_RGBA16F,     GL_RGBA,         GL_HALF_FLOAT,    false, 8 }, // R16G16B16A16_FLOAT
    { GL_RGBA16,      GL_RGBA,         GL_UNSIGNED_SHORT, false, 8 }, // R16G16B16A16_UNORM
    { GL_RGBA16UI,    GL_RGBA_INTEGER, GL_UNSIGNED_SHORT, false, 8 }, // R16G16B16A16_UINT
    { GL_RGBA16_SNORM,GL_RGBA,         GL_SHORT,          false, 8 }, // R16G16B16A16_SNORM
    { GL_RGBA16I,     GL_RGBA_INTEGER, GL_SHORT,          false, 8 }, // R16G16B16A16_SINT
    { GL_RG32F,       GL_RG,           GL_FLOAT,          false, 8 }, // R32G32_FLOAT
    { GL_RG32UI,      GL_RG_INTEGER,   GL_UNSIGNED_INT,   false, 8 }, // R32G32_UINT
    { GL_RG32I,       GL_RG_INTEGER,   GL_INT,            false, 8 }, // R32G32_SINT

    // 32-bit
    { GL_RGB10_A2,    GL_RGBA,         GL_UNSIGNED_INT_2_10_10_10_REV, false, 4 }, // R10G10B10A2_UNORM
    { GL_RGB10_A2UI,  GL_RGBA_INTEGER, GL_UNSIGNED_INT_2_10_10_10_REV, false, 4 }, // R10G10B10A2_UINT
    { GL_R11F_G11F_B10F, GL_RGB,       GL_UNSIGNED_INT_10F_11F_11F_REV, false, 4 }, // R11G11B10_FLOAT
    { GL_RGBA8,       GL_RGBA,         GL_UNSIGNED_BYTE,  false, 4 }, // R8G8B8A8_UNORM
    { GL_SRGB8_ALPHA8,GL_RGBA,         GL_UNSIGNED_BYTE,  false, 4 }, // R8G8B8A8_UNORM_SRGB
    { GL_RGBA8UI,     GL_RGBA_INTEGER, GL_UNSIGNED_BYTE,  false, 4 }, // R8G8B8A8_UINT
    { GL_RGBA8_SNORM, GL_RGBA,         GL_BYTE,           false, 4 }, // R8G8B8A8_SNORM
    { GL_RGBA8I,      GL_RGBA_INTEGER, GL_BYTE,           false, 4 }, // R8G8B8A8_SINT
    { GL_RG16F,       GL_RG,           GL_HALF_FLOAT,     false, 4 }, // R16G16_FLOAT
    { GL_RG16,        GL_RG,           GL_UNSIGNED_SHORT, false, 4 }, // R16G16_UNORM
    { GL_RG16UI,      GL_RG_INTEGER,   GL_UNSIGNED_SHORT, false, 4 }, // R16G16_UINT
    { GL_RG16_SNORM,  GL_RG,           GL_SHORT,          false, 4 }, // R16G16_SNORM
    { GL_RG16I,       GL_RG_INTEGER,   GL_SHORT,          false, 4 }, // R16G16_SINT
    { GL_R32F,        GL_RED,          GL_FLOAT,          false, 4 }, // R32_FLOAT
    { GL_R32UI,       GL_RED_INTEGER,  GL_UNSIGNED_INT,   false, 4 }, // R32_UINT
    { GL_R32I,        GL_RED_INTEGER,  GL_INT,            false, 4 }, // R32_SINT

    // 16-bit
    { GL_RG8,         GL_RG,           GL_UNSIGNED_BYTE,  false, 2 }, // R8G8_UNORM
    { GL_RG8UI,       GL_RG_INTEGER,   GL_UNSIGNED_BYTE,  false, 2 }, // R8G8_UINT
    { GL_RG8_SNORM,   GL_RG,           GL_BYTE,           false, 2 }, // R8G8_SNORM
    { GL_RG8I,        GL_RG_INTEGER,   GL_BYTE,           false, 2 }, // R8G8_SINT
    { GL_R16F,        GL_RED,          GL_HALF_FLOAT,     false, 2 }, // R16_FLOAT
    { GL_R16,         GL_RED,          GL_UNSIGNED_SHORT, false, 2 }, // R16_UNORM
    { GL_R16UI,       GL_RED_INTEGER,  GL_UNSIGNED_SHORT, false, 2 }, // R16_UINT
    { GL_R16_SNORM,   GL_RED,          GL_SHORT,          false, 2 }, // R16_SNORM
    { GL_R16I,        GL_RED_INTEGER,  GL_SHORT,          false, 2 }, // R16_SINT

    // 8-bit
    { GL_R8,          GL_RED,          GL_UNSIGNED_BYTE,  false, 1 }, // R8_UNORM
    { GL_R8UI,        GL_RED_INTEGER,  GL_UNSIGNED_BYTE,  false, 1 }, // R8_UINT
    { GL_R8_SNORM,    GL_RED,          GL_BYTE,           false, 1 }, // R8_SNORM
    { GL_R8I,         GL_RED_INTEGER,  GL_BYTE,           false, 1 }, // R8_SINT

    // (*) A8_UNORM — no native GL equivalent; stored as R8, needs swizzle mask
    { GL_R8,          GL_RED,          GL_UNSIGNED_BYTE,  false, 1 }, // A8_UNORM (*)

    // Misc
    { GL_RGB9_E5,        GL_RGB,           GL_UNSIGNED_INT_5_9_9_9_REV, false, 4 }, // R9G9B9E5_SHAREDEXP
    { GL_NONE,           GL_NONE,          GL_NONE,                      false, 0 }, // R10G10B10_XR_BIAS_A2_UNORM (**)

    // Depth
    { GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT,          false, 4 }, // D32_FLOAT
    { GL_DEPTH_COMPONENT16,  GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT,  false, 2 }, // D16_UNORM
};

class RenderFramework_OGL3 : public RenderFramework {
    struct Buffer {
        GLuint id;
    };
    struct Texture {
        GLuint id;
    };
    struct Viewport {
        int x;
        int y;
        int width;
		int height;
        float MinDepth;
        float MaxDepth;
    };
public:
    RenderFramework_OGL3();
    ~RenderFramework_OGL3();

    bool ShouldMainLoopRun();

    bool ImGuiStartFrame();
    void ImGuiEndFrame();

    void ImGuiDeInit();

    void RuiClearFrame();

    void DrawIndexed(uint32_t count, uint32_t start, size_t* resources);
    size_t CreateShaderDataBuffer(std::vector<ShaderSizeData_t> data);
    size_t CreateTextureFromData(void* data, uint32_t width, uint32_t height, uint16_t format, uint32_t pitch, uint32_t slicePitch);
    size_t LoadTexture(fs::path& path);

    void RuiWriteIndexBuffer(std::vector<uint16_t>& data);
    void RuiWriteVertexBuffer(std::vector<Vertex_t>& data);
    void RuiWriteStyleBuffer(std::vector<StyleDescriptorShader_t>& data);


    void RuiBindPipeline();
    void RuiLoad(int width, int height);
    void RuiReCreatePipeline(int width, int height);

    void* GetTextureView(size_t id);
    void* GetRuiView();
private:
    std::vector<Buffer> buffers;
    std::vector<Texture> textures;
    const char* vertexSource;
    const char* fragmentSource;
    uint32_t ruiFBO;
    uint32_t ruiTexture;
    uint32_t ruiDepthStencil;
    SDL_Window* window;
    SDL_GLContext glContext;
    GLuint* ruiImageView;
    GLuint fbo;
    GLuint colorTexture;
    GLuint depthStencilTexture;
    GLuint vao;
    GLuint shaderProgram;
    GLuint vertexBuffer;
    GLuint indexBuffer;
    GLuint styleDescBuffer;
    GLuint samplerState;
    GLuint styleDescSSBO;
	GLuint modelInstanceUBO;
	GLuint commonPerCameraUBO;
	Viewport viewport;
};