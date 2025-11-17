#pragma once


#include <fstream>
#include <map>
#include <string>
#include <any>


class RenderInstance;

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "intTypes.h"
#include "FontAtlas.h"
#include "ImageAtlas.h"
#include "ShaderStructs.h"
#include "Util.h"
#include "RuiNodeEditor/RuiVariables.h"
#include "RenderFrameworks/RenderFramework.h"

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



struct IndexSegment_t {
    FontAtlas_t* fontAtlas;
    ImageAtlas* imageAtlas;
    size_t indexStart;
};

struct TriData {
    __m128 a;
    __m128 b;
};

struct TransformResult {
    __m128 directionVector;
    __m128 position;
    __m128 inputSize;
    uint64_t hash;
    
    TransformResult():
        directionVector(_mm_setzero_ps()),
        position(_mm_setzero_ps()),
        inputSize(_mm_setzero_ps()),
        hash(randomInt64())
    { }

    TransformResult(__m128 dir,__m128 pos,__m128 size,uint64_t hash_):
        directionVector(dir),
        position(pos),
        inputSize(size),
        hash(hash_)
    { }
    TriData GenTri(__m128 x,__m128 y) const {
        TriData res;
        res.a =_mm_add_ps(
            _mm_add_ps(
                _mm_mul_ps(_mm_shuffle_ps(directionVector, directionVector, _MM_SHUFFLE(2, 2, 2, 2)), x),
                _mm_mul_ps(_mm_shuffle_ps(directionVector, directionVector, _MM_SHUFFLE(0, 0, 0, 0)), y)),
            _mm_shuffle_ps(position, position, _MM_SHUFFLE(0, 0, 0, 0)));
        res.b = _mm_add_ps(
            _mm_add_ps(
                _mm_mul_ps(_mm_shuffle_ps(directionVector, directionVector, _MM_SHUFFLE(3, 3, 3, 3)), x),
                _mm_mul_ps(_mm_shuffle_ps(directionVector, directionVector, _MM_SHUFFLE(1, 1, 1, 1)), y)),
            _mm_shuffle_ps(position, position, _MM_SHUFFLE(1, 1, 1, 1)));
        return res;
    }
};

struct RenderQuad
{
    __m128 xUvVector;
    __m128 yUvVector;
    __m128 UvBase;
    __m128 m128_30;
    __m128 m128_40;
    __m128 m128_50;
    short assetIndex;
    short assetIndex2;
    short styleDescriptorIndex;
    short flags;
    float vert[4][2];
};


struct Globals {
    float currentTime = 0.f;
    float adsFracValue = 0.f;
    float localPlayerPos[3];
    float screenWidth = 0.f;
    float screenHeight = 0.f;
};
class RenderInstance;

struct RenderJob {
    int layer;
    std::function<void(RenderInstance& render)> func;
};

class RenderInstance
{


public:

    std::vector<__m128> transformSizes;
    std::vector<TransformResult> transformResults;
    DrawInfo drawInfo;
    Globals globals;
    

    std::vector<RenderJob> jobs;
    std::vector<Vertex_t> verts;
    std::vector<uint16_t> indices;
    std::vector<IndexSegment_t> segments;
    std::vector<StyleDescriptorShader_t> styleDescriptor;
    std::map<std::string,std::any> arguments;

    float elementWidth;
    float elementHeight;
    float elementWidthRpc;
    float elementHeightRpc;

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
    void sub_FEF30(__m128 *a3, __m128 *a4, TriData& a5);
    void generateDrawTriangle(RenderQuad* v180,bool v24,__m128 a8,__m128* a3a, int a6,const TransformResult& a5, __m128* triangleType );
    void sub_F9B80_rev(
        __m128 ruiSize,
        RenderQuad& a4,
        const TransformResult& a5,
        int a6,
        Asset_t &a7,
        const __m128& a8,
        const __m128& a9,
        const __m128& texMin,
        const __m128& texSize);
    void sub_FFAE0(__m128 a1,__m128 a2, __m128* a3);



    void StartFrame(float time);
    void EndFrame();

    void SetSize(float width, float height) {
        elementHeight = height;
        elementHeightRpc = 1.f/height;
        elementWidth = width;
        elementWidthRpc = 1.f/width;
    
        memset(&drawInfo,0,sizeof(DrawInfo));
        drawInfo.float_18 = 0.f;
        drawInfo.float_1C = 1.f;
        drawInfo.ruiUnk3[0].screenWidth = width;
        drawInfo.ruiUnk3[0].screenHeight = height;
        drawInfo.ruiUnk3[0].float_4 = 0.f;
        drawInfo.ruiUnk3[0].float_8 = 0.f;
        drawInfo.ruiUnk3[0].float_C = 1.f;
        drawInfo.ruiUnk3[0].float_10 = 0.f;
        drawInfo.ruiUnk3[0].float_18 = 0.f;
        drawInfo.ruiUnk3[0].float_1C = 1.f;
        drawInfo.ruiUnk3[1] = drawInfo.ruiUnk3[0];
        drawInfo.ruiUnk3[0].float_20 = 0.f;
        drawInfo.ruiUnk3[0].float_24 = 0.f;
        drawInfo.ruiUnk3[0].float_28 = 0.f;
        drawInfo.ruiUnk3[0].float_2C = 1.f;
    }

    RenderInstance(float width, float height) {
        SetSize(width,height);
    }

    void DrawImage() {
        ImGui::Begin("Render Image");
        float width = ImGui::GetWindowWidth();
        width -= 10; //margin
        ImGui::Image(g_renderFramework->GetRuiView(), ImVec2(width, elementHeight / elementWidth * width), ImVec2(0, 0), ImVec2(1, 1));
        ImGui::End();
    }
};

extern __m128 stru_5F4740a[];
extern __m128 xmm_12A4E830a[];
