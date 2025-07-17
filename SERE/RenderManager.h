#pragma once
#include "intTypes.h"
#include "FontAtlas.h"
#include "ImageAtlas.h"
#include "ShaderStructs.h"

#include <d3d11_1.h>
#include <fstream>
#include <map>
#include <string>
#pragma comment(lib,"d3d11.lib")
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include "imgui/imgui.h"


struct DrawInfoUnknown3
{
    float screenWidth;
    float float_4;
    float float_8;
    float float_C;
    float float_10;
    float screenHeight;
    float float_18;
    float float_1C;
    float float_20;
    float float_24;
    float float_28;
    float float_2C;
};


struct DrawInfo
{
    float float_10;
    float float_14;
    float float_18;
    float float_1C;
    DrawInfoUnknown3 ruiUnk3[2];
};

struct Vertex_t
{
    float position[3];
    float float_C[3];
    float float_18[4];

    float float_28[2];
    WORD assetIndex;
    WORD assetIndex2;
    WORD word_34;
    WORD word_36;
};

struct IndexSegment_t {
    FontAtlas_t* fontAtlas;
    ImageAtlas* imageAtlas;
    size_t indexStart;
};

struct TransformResult {
    int index;
    __m128 directionVector;
    __m128 position;
    __m128 inputSize;
};

struct RenderQuad
{
    __m128 xUvVector;
    __m128 yUvVector;
    __m128 xUvBase;
    __m128 m128_30;
    __m128 m128_40;
    __m128 m128_50;
    WORD assetIndex;
    WORD assetIndex2;
    WORD styleDescriptorIndex;
    WORD flags;
    float vert[4][2];
};

struct StyleDescriptorShader_t
{
    float color0_red;
    float color0_green;
    float color0_blue;
    float color0_alpha;
    float color1_red;
    float color1_green;
    float color1_blue;
    float color1_alpha;
    float color2_red;
    float color2_green;
    float color2_blue;
    float color2_alpha;
    float float_30;
    float float_34;
    float float_38;
    float float_3C;
    float float_40;
    float float_44;
    float float_48;
    float float_4C;
    float float_50;
    BYTE gap_54[12];
};

struct ArgVal {

    std::string stringVal;
    float floatVal[4];
    int intVal;
};

struct Argument_t {
    int type;
    ArgVal defaultVal;
};

struct TransformSize {
    __m128 size;
};

struct Globals {
    float currentTime = 0;
};

class RenderInstance
{


private:
    ID3D11Device* device;
    ID3D11DeviceContext *deviceContext;
    ID3D11SamplerState* samplerState;

    ID3D11BlendState* blendState;
    ID3D11Texture2D* targetTexture;
    ID3D11RenderTargetView* targetView;
    
    ID3D11Texture2D* depthTexture;
    ID3D11DepthStencilView* depthStencil;
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



public:
    ID3D11ShaderResourceView* targetResourceView;
    std::vector<__m128> transformSizes;
    std::vector<TransformResult> transformResults;
    DrawInfo drawInfo;
    Globals globals;

    std::vector<Vertex_t> verts;
    std::vector<uint16_t> indices;
    std::vector<IndexSegment_t> segments;
    std::vector<StyleDescriptorShader_t> styleDescriptor;
    std::map<std::string,Argument_t> arguments;

    float elementWidth;
    float elementHeight;
    float elementWidthRatio;
    float elementHeightRatio;


    void AddImageAtlasSegment(ImageAtlas* atlas) {
        if (segments.size() == 0) {
            IndexSegment_t segment;
            segment.imageAtlas = atlas;
            segment.fontAtlas = 0;
            segment.indexStart = indices.size();
            segments.push_back(segment);
        }
        IndexSegment_t& curLast = segments[segments.size()-1];
        if (curLast.imageAtlas == atlas) {
            return;
        }
        if (curLast.imageAtlas == 0) {
            curLast.imageAtlas = atlas;
            return;
        }
        IndexSegment_t segment;
        segment.imageAtlas = atlas;
        segment.fontAtlas = 0;
        segment.indexStart = indices.size();
        segments.push_back(segment);
    }

    void AddFontAtlasSegment(FontAtlas_t* atlas) {
        if (segments.size() == 0) {
            IndexSegment_t segment;
            segment.imageAtlas = 0;
            segment.fontAtlas = atlas;
            segment.indexStart = indices.size();
            segments.push_back(segment);
        }
        IndexSegment_t& curLast = segments[segments.size()-1];
        if (curLast.fontAtlas == atlas) {
            return;
        }
        if (curLast.fontAtlas == 0) {
            curLast.fontAtlas = atlas;
            return;
        }
        IndexSegment_t segment;
        segment.imageAtlas = 0;
        segment.fontAtlas = atlas;
        segment.indexStart = indices.size();
        segments.push_back(segment);
    }

    void AddQuad(RenderQuad& quad);
    void sub_FEF30(__m128 *a3, __m128 *a4, __m128 *a5);
    void generateDrawTriangle(RenderQuad* v180,bool v24,__m128 a8,__m128* a3a, int a6,TransformResult* a5, __m128* triangleType );
    void sub_F9B80_rev(
        __m128 ruiSize,
        RenderQuad& a4,
        TransformResult *a5,
        int a6,
        Asset_t &a7,
        __m128 *a8,
        __m128 *a9,
        __m128 *a10,
        __m128 *a11);
    void sub_FFAE0(__m128 a1,__m128 a2, __m128* a3);


    void initBuffers_v30();
    void setD11Device(ID3D11Device* dev, ID3D11DeviceContext* cont) {
        device = dev;
        deviceContext = cont;
    }
    void StartFrame(float time);
    void EndFrame();

    void SetSize(float width, float height) {
        elementHeight = height;
        elementHeightRatio = 1/height;
        elementWidth = width;
        elementWidthRatio = 1/width;
    
        memset(&drawInfo,0,sizeof(DrawInfo));
        drawInfo.float_18 = 0;
        drawInfo.float_1C = 1;
        drawInfo.ruiUnk3[0].screenWidth = width;
        drawInfo.ruiUnk3[0].screenHeight = height;
        drawInfo.ruiUnk3[0].float_4 = 0;
        drawInfo.ruiUnk3[0].float_8 = 0;
        drawInfo.ruiUnk3[0].float_C = 1;
        drawInfo.ruiUnk3[0].float_10 = 0;
        drawInfo.ruiUnk3[0].float_18 = 0;
        drawInfo.ruiUnk3[0].float_1C = 1;
        drawInfo.ruiUnk3[1] = drawInfo.ruiUnk3[0];
        drawInfo.ruiUnk3[0].float_20 = 0;
        drawInfo.ruiUnk3[0].float_24 = 0;
        drawInfo.ruiUnk3[0].float_28 = 0;
        drawInfo.ruiUnk3[0].float_2C = 1;
    }

    RenderInstance(ID3D11Device* dev, ID3D11DeviceContext* context, float width, float height) {
        SetSize(width,height);
        device = dev;
        deviceContext = context;
        initBuffers_v30();
    }

    void DrawImage() {
        ImGui::Begin("Render Image");
        float width = ImGui::GetWindowWidth();
        ImGui::Image(targetResourceView,ImVec2(width,elementHeight/elementWidth*width),ImVec2(0,0),ImVec2(1,1));
        ImGui::End();
    }
};

extern __m128 stru_5F4740a[];
extern __m128 xmm_12A4E830a[];