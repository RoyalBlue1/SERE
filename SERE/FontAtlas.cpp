

#include "FontAtlas.h"

#include <fstream>
#include "ThirdParty/DDSTextureLoader11.h"
#include "ThirdParty/rapidjson/document.h"


#undef GetObject

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
    std::ifstream jsonFile{jsonPath};
    if(jsonFile.fail())return;
    std::stringstream jsonStringStream;
    while (jsonFile.peek() != EOF)
        jsonStringStream << (char)jsonFile.get();
    jsonFile.close();

    rapidjson::Document doc;
    doc.Parse(jsonStringStream.str().c_str());
    if(doc.HasParseError())return;
    
    rapidjson::GenericObject root = doc.GetObject();



    if(!(root.HasMember("width")&&root["width"].IsUint()))return;
    if(!(root.HasMember("height")&&root["height"].IsUint()))return;
    if(!(root.HasMember("widthRatio")&&root["widthRatio"].IsNumber()))return; 
    if(!(root.HasMember("heightRatio")&&root["heightRatio"].IsNumber()))return;
    if(!(root.HasMember("unk_2")&&root["unk_2"].IsUint()))return;
    if((!root.HasMember("fonts")&&root["fonts"].IsArray()))return;
    if(!(root.HasMember("unk_18")&&root["unk_18"].IsArray()))return;

    width = root["width"].GetUint();
    widthRatio = root["widthRatio"].GetFloat();
    height = root["height"].GetUint();
    heightRatio = root["heightRatio"].GetFloat();
    unk_2 = root["unk_2"].GetUint();
    
    rapidjson::GenericArray fontArray = root["fonts"].GetArray();

    for(auto fontItr = fontArray.Begin();fontItr != fontArray.End();fontItr++) {
        if(!fontItr->IsObject())continue;
        rapidjson::GenericObject fontObj = fontItr->GetObject();

        if(!(fontObj.HasMember("name")&&fontObj["name"].IsString()))continue;
        if(!(fontObj.HasMember("fontId")&&fontObj["fontId"].IsInt()))continue;
        if(!(fontObj.HasMember("proportionScaleX")&&fontObj["proportionScaleX"].IsNumber()))continue;
        if(!(fontObj.HasMember("proportionScaleY")&&fontObj["proportionScaleY"].IsNumber()))continue;
        if(!(fontObj.HasMember("unk_24")&&fontObj["unk_24"].IsNumber()))continue;
        if(!(fontObj.HasMember("unk_28")&&fontObj["unk_28"].IsNumber()))continue;
        if(!(fontObj.HasMember("unicodeIndex")&&fontObj["unicodeIndex"].IsInt()))continue;
        if(!(fontObj.HasMember("unicodeChunks")&&fontObj["unicodeChunks"].IsArray()))continue;
        if(!(fontObj.HasMember("glyphChunks")&&fontObj["glyphChunks"].IsArray()))continue;
        if(!(fontObj.HasMember("proportions")&&fontObj["proportions"].IsArray()))continue;
        if(!(fontObj.HasMember("glyphs")&&fontObj["glyphs"].IsArray()))continue;
        if(!(fontObj.HasMember("KerningInfo")&&fontObj["KerningInfo"].IsArray()))continue;
        
        

        Font_t font;
        font.name = fontObj["name"].GetString();
        font.fontIndex = fontObj["fontId"].GetInt();
        font.proportionScaleX = fontObj["proportionScaleX"].GetFloat();
        font.proportionScaleY = fontObj["proportionScaleY"].GetFloat();
        font.float_24 = fontObj["unk_24"].GetFloat();
        font.float_28 = fontObj["unk_28"].GetFloat();
        font.unicodeIndex = fontObj["unicodeIndex"].GetInt();
        rapidjson::GenericArray unicodeChunks = fontObj["unicodeChunks"].GetArray();
        for(auto itr = unicodeChunks.Begin();itr != unicodeChunks.End();itr++){
            if(!itr->IsInt())continue;
            font.unicodeChunk.push_back(itr->GetInt());
        }
        rapidjson::GenericArray glyphCh = fontObj["glyphChunks"].GetArray();
        for(auto itr = glyphCh.Begin();itr != glyphCh.End();itr++) {
            if(!itr->IsObject())continue;
            rapidjson::GenericObject ch = itr->GetObject();

            if(!(ch.HasMember("unicodeChunksIndex")&&ch["unicodeChunksIndex"].IsInt()))continue;
            if(!(ch.HasMember("mask")&&ch["mask"].IsUint64()))continue;

            GlyphChunk_t chunk;
            chunk.glyphIndex = ch["unicodeChunksIndex"].GetInt();
            chunk.mask = ch["mask"].GetUint64();
            font.glyphChunks.push_back(chunk);
        }
        rapidjson::GenericArray props = fontObj["proportions"].GetArray();
        for(auto itr = props.Begin();itr != props.End();itr++) {
            if(!itr->IsObject())continue;
            rapidjson::GenericObject propObj = itr->GetObject();

            if(!(propObj.HasMember("scaleBounds")&&propObj["scaleBounds"].IsNumber()))continue;
            if(!(propObj.HasMember("scaleSize")&&propObj["scaleSize"].IsNumber()))continue;

            Proportion_t prop;
            prop.scaleBounds = propObj["scaleBounds"].GetFloat();
            prop.scaleSize = propObj["scaleSize"].GetFloat();
            font.proportions.push_back(prop);
        }
        rapidjson::GenericArray glyphs = fontObj["glyphs"].GetArray();
        for(auto itr = glyphs.Begin();itr != glyphs.End();itr++) {
            if(!itr->IsObject())continue;
            rapidjson::GenericObject glyphObj = itr->GetObject();

            if(!(glyphObj.HasMember("unk_0")&&glyphObj["unk_0"].IsNumber()))continue;
            if(!(glyphObj.HasMember("kerningBaseIndex")&&glyphObj["kerningBaseIndex"].IsInt()))continue;
            if(!(glyphObj.HasMember("unk_6")&&glyphObj["unk_6"].IsInt()))continue;
            if(!(glyphObj.HasMember("proportionIndex")&&glyphObj["proportionIndex"].IsInt()))continue;
            if(!(glyphObj.HasMember("posBaseX")&&glyphObj["posBaseX"].IsNumber()))continue;
            if(!(glyphObj.HasMember("posBaseY")&&glyphObj["posBaseY"].IsNumber()))continue;
            if(!(glyphObj.HasMember("posMinX")&&glyphObj["posMinX"].IsNumber()))continue;
            if(!(glyphObj.HasMember("posMinY")&&glyphObj["posMinY"].IsNumber()))continue;
            if(!(glyphObj.HasMember("posMaxX")&&glyphObj["posMaxX"].IsNumber()))continue;
            if(!(glyphObj.HasMember("posMaxY")&&glyphObj["posMaxY"].IsNumber()))continue;
            Glyph_t glyph;
            glyph.float_0 = glyphObj["unk_0"].GetFloat();
            glyph.kerningStartIndex = glyphObj["kerningBaseIndex"].GetInt();
            glyph.byte_6 = glyphObj["unk_6"].GetInt();
            glyph.proportionIndex = glyphObj["proportionIndex"].GetInt();
            glyph.posBaseX = glyphObj["posBaseX"].GetFloat();
            glyph.posBaseY = glyphObj["posBaseY"].GetFloat();
            glyph.posMinX = glyphObj["posMinX"].GetFloat();
            glyph.posMinY = glyphObj["posMinY"].GetFloat();
            glyph.posMaxX = glyphObj["posMaxX"].GetFloat();
            glyph.posMaxY = glyphObj["posMaxY"].GetFloat();
            font.glyphs.push_back(glyph);
        }
        rapidjson::GenericArray kernings = fontObj["KerningInfo"].GetArray();
        for(auto itr = kernings.Begin();itr != kernings.End();itr++) {
            if(!itr->IsObject())continue;
            rapidjson::GenericObject kernObj = itr->GetObject();
            if(!(kernObj.HasMember("otherIndex")&&kernObj["otherIndex"].IsInt()))continue;
            if(!(kernObj.HasMember("distance")&&kernObj["distance"].IsNumber()))continue;

            KerningInfo_t kern;
            kern.otherChar = kernObj["otherIndex"].GetInt();
            kern.kerningDistance = kernObj["distance"].GetFloat();
            font.kerningInfos.push_back(kern);
        }
        fonts.emplace(font.fontIndex,font);
    }
    rapidjson::GenericArray unk18 = root["unk_18"].GetArray();
    for(auto itr = unk18.Begin();itr != unk18.End();itr++) {
        if(!itr->IsInt())continue;
        unk_18.push_back(itr->GetInt());
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