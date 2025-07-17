

#include "FontAtlas.h"

#include <fstream>
#include "ThirdParty/DDSTextureLoader11.h"
#include "ThirdParty/yyjson.h"


std::vector<FontAtlas_t> fonts;
std::map<uint16_t,size_t> fontAtlasIndices;


struct ProportionData {
    float x;
    float y;
};

struct FontShaderData_t {
    float minX;
    float minY;
    float maxX;
    float maxY;
};

void FontAtlas_t::loadFromFile(fs::path& jsonPath,ID3D11Device* d11Device) {
    yyjson_read_err jsonErr;
    yyjson_doc* doc = yyjson_read_file(jsonPath.string().c_str(), 0, NULL, &jsonErr);
    if (doc) {

        width = yyjson_get_int(yyjson_obj_get(yyjson_doc_get_root(doc),"width"));
        height = yyjson_get_int(yyjson_obj_get(yyjson_doc_get_root(doc),"height"));
        widthRatio = yyjson_get_num(yyjson_obj_get(yyjson_doc_get_root(doc),"widthRatio"));
        heightRatio = yyjson_get_num(yyjson_obj_get(yyjson_doc_get_root(doc),"heightRatio"));
        unk_2 = yyjson_get_int(yyjson_obj_get(yyjson_doc_get_root(doc),"unk_2"));
        yyjson_val* fnts = yyjson_obj_get(yyjson_doc_get_root(doc),"fonts");
        size_t idx,size;
        yyjson_val *fontEntry;
        yyjson_arr_foreach(fnts, idx, size, fontEntry) {
            Font_t font;
            font.name = yyjson_get_str(yyjson_obj_get(fontEntry, "name"));
            font.fontIndex = yyjson_get_int(yyjson_obj_get(fontEntry, "fontId"));
            font.proportionScaleX = yyjson_get_num(yyjson_obj_get(fontEntry, "proportionScaleX"));
            font.proportionScaleY = yyjson_get_num(yyjson_obj_get(fontEntry, "proportionScaleY"));
            font.float_24 = yyjson_get_num(yyjson_obj_get(fontEntry, "unk_24"));
            font.float_28 = yyjson_get_num(yyjson_obj_get(fontEntry, "unk_28"));
            font.unicodeIndex = yyjson_get_int(yyjson_obj_get(fontEntry,"unicodeIndex"));
            yyjson_val* uniChunks = yyjson_obj_get(fontEntry,"unicodeChunks");
            size_t i_idx,i_size;
            yyjson_val *entry;
            yyjson_arr_foreach(uniChunks,i_idx,i_size,entry){
                font.unicodeChunk.push_back(yyjson_get_uint(entry));
            }
            yyjson_val* glyphCh = yyjson_obj_get(fontEntry,"glyphChunks");
            yyjson_arr_foreach(glyphCh, i_idx, i_size,entry ) {
                GlyphChunk_t chunk;
                chunk.glyphIndex = yyjson_get_int(yyjson_obj_get(entry, "unicodeChunksIndex"));
                chunk.mask = yyjson_get_uint(yyjson_obj_get(entry, "mask"));
                font.glyphChunks.push_back(chunk);
            }
            yyjson_val* props = yyjson_obj_get(fontEntry,"proportions");
            yyjson_arr_foreach(props, i_idx, i_size, entry) {
                Proportion_t prop;
                prop.scaleBounds = yyjson_get_num(yyjson_obj_get(entry, "scaleBounds"));
                prop.scaleSize = yyjson_get_num(yyjson_obj_get(entry, "scaleSize"));
                font.proportions.push_back(prop);
            }
            yyjson_val* glyphs = yyjson_obj_get(fontEntry,"glyphs");
            yyjson_arr_foreach(glyphs, i_idx, i_size, entry) {
                Glyph_t glyph;
                glyph.float_0 = yyjson_get_num(yyjson_obj_get(entry, "unk_0"));
                glyph.kerningStartIndex = yyjson_get_int(yyjson_obj_get(entry, "kerningBaseIndex"));
                glyph.byte_6 = yyjson_get_int(yyjson_obj_get(entry, "unk_6"));
                glyph.proportionIndex = yyjson_get_int(yyjson_obj_get(entry, "proportionIndex"));
                glyph.posBaseX = yyjson_get_num(yyjson_obj_get(entry, "posBaseX"));
                glyph.posBaseY = yyjson_get_num(yyjson_obj_get(entry, "posBaseY"));
                glyph.posMinX = yyjson_get_num(yyjson_obj_get(entry, "posMinX"));
                glyph.posMinY = yyjson_get_num(yyjson_obj_get(entry, "posMinY"));
                glyph.posMaxX = yyjson_get_num(yyjson_obj_get(entry, "posMaxX"));
                glyph.posMaxY = yyjson_get_num(yyjson_obj_get(entry, "posMaxY"));
                font.glyphs.push_back(glyph);
            }
            yyjson_val* kerning = yyjson_obj_get(fontEntry,"KerningInfo");
            yyjson_arr_foreach(kerning, i_idx, i_size, entry) {
                KerningInfo_t kern;
                kern.otherChar = yyjson_get_int(yyjson_obj_get(entry, "otherIndex"));
                kern.kerningDistance = yyjson_get_num(yyjson_obj_get(entry,"distance"));
                font.kerningInfos.push_back(kern);
            }
            fonts.emplace(font.fontIndex,font);
        }
        yyjson_val* unk18 = yyjson_obj_get(yyjson_doc_get_root(doc),"unk_18");
        yyjson_val* unk;
        yyjson_arr_foreach(unk18, idx, size, unk) {
            unk_18.push_back(yyjson_get_int(unk));
        }
    }

    fs::path ddsName = jsonPath.replace_extension("dds");

    DirectX::CreateDDSTextureFromFile(d11Device,ddsName.wstring().c_str(), &imageResource, &imageResourceView);
    std::vector<FontShaderData_t> shaderData;

    for (auto& font : fonts)
    {

        font.second.baseTextureIndex = shaderData.size();
        std::vector<ProportionData> propA;
        std::vector<ProportionData>  propB;

        for (auto& proportion : font.second.proportions) {

            ProportionData a;
            a.x = proportion.scaleBounds * font.second.proportionScaleX;
            a.y = proportion.scaleBounds * font.second.proportionScaleY;
            propA.push_back(a);

            ProportionData b;
            b.x = proportion.scaleSize * a.x;
            b.y = proportion.scaleSize * a.y;
            propB.push_back(b);
        }

        for (auto& glyph : font.second.glyphs)
        {

            FontShaderData_t shdDat;

            if ((glyph.posMinX == glyph.posMaxX) || (glyph.proportionIndex >= font.second.proportions.size()))
            {
                shdDat.minX = 1.f;
                shdDat.minY = 1.f;
                shdDat.maxX = 0.f;
                shdDat.maxY = 0.f;
            }
            else
            {
                uint8_t propIndex = glyph.proportionIndex;

                shdDat.minX = glyph.posMinX * propA[propIndex].x + glyph.posBaseX - propB[propIndex].x;
                shdDat.minY = glyph.posMinY * propA[propIndex].y + glyph.posBaseY - propB[propIndex].y;

                shdDat.maxX = glyph.posMaxX * propA[propIndex].x + glyph.posBaseX + propB[propIndex].x;
                shdDat.maxY = glyph.posMaxY * propA[propIndex].y + glyph.posBaseY + propB[propIndex].y;
            }
            shaderData.push_back(shdDat);
        }
    }

    if (shaderData.size()) {
        D3D11_SUBRESOURCE_DATA boundSubresoureDesc;
        D3D11_SHADER_RESOURCE_VIEW_DESC boundsShaderResourceViewDesc;
        D3D11_BUFFER_DESC boundsBufferDesc;

        boundsBufferDesc.ByteWidth = 16 * shaderData.size();
        boundsBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        boundsBufferDesc.MiscFlags = 64;
        boundsBufferDesc.StructureByteStride = 16;
        boundsBufferDesc.BindFlags = 8;
        boundsBufferDesc.CPUAccessFlags = 0;

        boundSubresoureDesc.pSysMem = shaderData.data();
        boundSubresoureDesc.SysMemPitch = 0;
        boundSubresoureDesc.SysMemSlicePitch = 0;

        if (HRESULT res = d11Device->CreateBuffer(&boundsBufferDesc, &boundSubresoureDesc, &boundsBuffer); res || (boundsBuffer == NULL)) {
            printf("D3D11Decive::CreateBuffer returned 0x%x", (uint32_t)res);
        };
        boundsShaderResourceViewDesc.Format = DXGI_FORMAT_UNKNOWN;
        boundsShaderResourceViewDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
        boundsShaderResourceViewDesc.Buffer.FirstElement = 0;
        boundsShaderResourceViewDesc.Buffer.NumElements = shaderData.size();
        if (HRESULT res = d11Device->CreateShaderResourceView(boundsBuffer, &boundsShaderResourceViewDesc, &boundsResourceView); res) {
            printf("D3D11Decive::CreateShaderResourceView returned 0x%x", (uint32_t)res);
        }
    }
    else {
        boundsBuffer = 0;
        boundsResourceView = 0;
    }
}


FontAtlas_t::FontAtlas_t(fs::path& jsonPath,size_t atlasIndex,ID3D11Device* d11Device) { 
    loadFromFile(jsonPath,d11Device); 
    for (auto& fnt : fonts) {
        fontAtlasIndices.emplace(fnt.first,atlasIndex);
    }
};


void loadFonts(ID3D11Device* d11Device) {
    fs::path folderPath = ".\\Assets\\Fonts";

    for (const auto& dirEntry : fs::recursive_directory_iterator(folderPath)) {
        if(!dirEntry.is_regular_file())continue;
        fs::path jsonName = dirEntry;
        if(jsonName.extension()!=".json")continue;
        fonts.emplace_back(jsonName,fonts.size(),d11Device);

    }
}


Font_t* getFontByIndex(uint16_t id) {
    return &fonts[fontAtlasIndices[id]].fonts[id];
}
FontAtlas_t* getFontAtlasByIndex(uint16_t id) {
    return &fonts[fontAtlasIndices[id]];
}