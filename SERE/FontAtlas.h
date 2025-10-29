#pragma once

#include <d3d11_1.h>
#include <map>
#include <filesystem>

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

struct FontShaderData_t {
    float minX;
    float minY;
    float maxX;
    float maxY;
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

struct UIFontHeader_v7_t
{
    char* name;

    uint16_t fontIndex;// index used in engine/by ruis to get this fonts

    uint16_t numProportions; // number of character proportions

    // number of chunks for glyph and unicode lookup, 64 unique glyphs/unicodes per chunk
    uint16_t numKernings; // unused
    uint16_t numUnicodeChunks;

    int glyphIndex; // base glyph index, unused
    int unicodeIndex; // base unicode character index, this gets subtracted off input characters, everything before this index is invalid.

    uint32_t numTextures; // used for mem alloc

    // for scaling the character proportions
    float proportionScaleX;
    float proportionScaleY;

    float unk_24;
    float unk_28;

    // base index for texture count
    uint32_t textureIndex; // used for mem alloc

    // for getting a texture from a provided unicode/glyph
    uint16_t* unicodeChunks; // index per 64 unicodes, index into other arrays to get assigned texture (unicodeBaseIndex & unicodeIndexMask)
    uint16_t* unicodeChunksIndex; // the base texture index, added with popcount from unicodeIndexMask
    uint64_t* unicodeChunksMask; // each bit represents a single unicode

    Proportion_t* proportions; // array of UIFontProportion_v7_t
    Glyph_t* glyphs; // array of UIFontTexture_v7_t

    KerningInfo_t* kerningInfo; // for kerning?
};

struct UIFontAtlasAssetHeader_v6_t
{
    uint16_t fontCount;
    uint16_t unk_2; // count for the data at unk_18

    uint16_t width;
    uint16_t height;

    // like ui image atlas
    float widthRatio;
    float heightRatio;
    UIFontHeader_v7_t* fonts;
    uint8_t* unk_18;

    uint64_t atlasGUID; // guid
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
    

    ID3D11Resource* imageResource;
    ID3D11ShaderResourceView* imageResourceView;
    ID3D11Buffer *boundsBuffer;
    ID3D11ShaderResourceView *boundsResourceView;

    FontAtlas_t(fs::path& jsonPath,size_t atlasIndex,ID3D11Device* d11Device);
    FontAtlas_t(UIFontAtlasAssetHeader_v6_t* fontAtlasHdr, ID3D11Device* device,ID3D11Texture2D* texture,ID3D11ShaderResourceView* textureView);
    void loadFromFile(fs::path& jsonPath,ID3D11Device* d11Device);
    void CreateShaderDataBuffer(ID3D11Device* device);
};

extern std::vector<FontAtlas_t> fonts;


void loadFonts(ID3D11Device* d11Device);
Font_t* getFontByIndex(uint16_t id);
FontAtlas_t* getFontAtlasByIndex(uint16_t id);
void loadRpakFont(UIFontAtlasAssetHeader_v6_t* font, ID3D11Device* device,ID3D11Texture2D* texture,ID3D11ShaderResourceView* textureView);
void clearFontAtlases();