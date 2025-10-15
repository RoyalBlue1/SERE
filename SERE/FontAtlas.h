#pragma once


#include <map>
#include <filesystem>
#include "RenderFrameworks/RenderFramework.h"

namespace fs = std::filesystem;


struct KerningInfo_t
{
    int otherChar;
    float kerningDistance;
};


struct Proportion_t
{
    float scaleBounds; // for the addition size added on to of the base size
    float scaleSize; // for the basic character size
};

struct GlyphChunk_t {
    uint16_t glyphIndex;
    uint64_t mask;
};

struct Glyph_t
{
    float float_0;
    uint16_t kerningStartIndex;
    uint8_t byte_6;
    uint8_t proportionIndex;
    float posBaseX;
    float posBaseY;
    float posMinX;
    float posMinY;
    float posMaxX;
    float posMaxY;
};


struct Font_t {
    std::string name;
    uint16_t fontIndex;
    float proportionScaleX;
    float proportionScaleY;
    float float_24;
    float float_28;
    int unicodeIndex;
    size_t baseTextureIndex;
    std::vector<Proportion_t> proportions;
    std::vector<KerningInfo_t> kerningInfos;
    std::vector<Glyph_t> glyphs;
    std::vector<uint16_t> unicodeChunk;
    std::vector<GlyphChunk_t> glyphChunks;

};




struct FontAtlas_t {

    uint16_t unk_2;
    uint16_t width;
    uint16_t height;

    float widthRatio;
    float heightRatio;

    std::map<uint16_t,Font_t> fonts;
    std::vector<uint8_t> unk_18;

    size_t textureId;
    size_t shaderDataId;
    std::shared_ptr<RenderFramework> render;
    FontAtlas_t(fs::path& jsonPath,size_t atlasIndex,std::shared_ptr<RenderFramework> rend);
    void loadFromFile(fs::path& jsonPath);
};

extern std::vector<FontAtlas_t> fonts;

void loadFonts(std::shared_ptr<RenderFramework> render);
Font_t* getFontByIndex(uint16_t id);
FontAtlas_t* getFontAtlasByIndex(uint16_t id);