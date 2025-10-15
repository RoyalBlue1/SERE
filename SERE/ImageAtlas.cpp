
#include "ImageAtlas.h"

#include <filesystem>
#include <fstream>

#include "ThirdParty/rapidjson/document.h"
#include "Thirdparty/DDSTextureLoader11.h"


#undef GetObject
namespace fs = std::filesystem;



std::map<uint32_t, Asset_t> imageAssetMap{};
std::vector<ImageAtlas> imageAtlases{};
void loadImageAtlases(std::shared_ptr<RenderFramework> render) {
	fs::path folderPath = ".\\Assets\\Atlases";

	for (const auto& dirEntry : fs::recursive_directory_iterator(folderPath)) {
		if(!dirEntry.is_regular_file())continue;
		fs::path jsonName = dirEntry;
		if(jsonName.extension()!=".json")continue;

		std::ifstream jsonFile{jsonName};
		if(jsonFile.fail())return;
		std::stringstream jsonStringStream;
		while (jsonFile.peek() != EOF)
			jsonStringStream << (char)jsonFile.get();
		jsonFile.close();

		rapidjson::Document doc;
		doc.Parse(jsonStringStream.str().c_str());
		rapidjson::ParseErrorCode error = doc.GetParseError();
		if(doc.HasParseError())return;

		rapidjson::GenericObject root = doc.GetObject();

		if((!root.HasMember("textureOffsets")&&root["textureOffsets"].IsArray()))return;
		if((!root.HasMember("textureDimentions")&&root["textureDimentions"].IsArray()))return;
		if((!root.HasMember("textureHashes")&&root["textureHashes"].IsArray()))return;
		if((!root.HasMember("renderOffsets")&&root["renderOffsets"].IsArray()))return;
		if((!root.HasMember("shaderData")&&root["shaderData"].IsArray()))return;



		ImageAtlas atlas;
		atlas.render = render;
		atlas.name = jsonName.filename().replace_extension("").string();
		rapidjson::GenericArray textureOffsets = root["textureOffsets"].GetArray();
		for(auto itr = textureOffsets.Begin();itr != textureOffsets.End();itr++) {
			if(!itr->IsObject())continue;
			rapidjson::GenericObject texOff = itr->GetObject();

			if(!(texOff.HasMember("f0")&&texOff["f0"].IsNumber()))continue;
			if(!(texOff.HasMember("f1")&&texOff["f1"].IsNumber()))continue;
			if(!(texOff.HasMember("endX")&&texOff["endX"].IsNumber()))continue;
			if(!(texOff.HasMember("endY")&&texOff["endY"].IsNumber()))continue;
			if(!(texOff.HasMember("startX")&&texOff["startX"].IsNumber()))continue;
			if(!(texOff.HasMember("startY")&&texOff["startY"].IsNumber()))continue;
			if(!(texOff.HasMember("unkX")&&texOff["unkX"].IsNumber()))continue;
			if(!(texOff.HasMember("unkY")&&texOff["unkY"].IsNumber()))continue;


			textureOffset_ offset;
			offset.m128_0.m128_f32[0] = texOff["f0"].GetFloat();
			offset.m128_0.m128_f32[1] = texOff["f1"].GetFloat();
			offset.m128_0.m128_f32[2] = texOff["endX"].GetFloat();
			offset.m128_0.m128_f32[3] = texOff["endY"].GetFloat();
			offset.m128_10.m128_f32[0] = texOff["startX"].GetFloat();
			offset.m128_10.m128_f32[1] = texOff["startY"].GetFloat();
			offset.m128_10.m128_f32[2] = texOff["unkX"].GetFloat();
			offset.m128_10.m128_f32[3] = texOff["unkY"].GetFloat();
			atlas.offsets.push_back(offset);
		}

		rapidjson::GenericArray textureDimentions = root["textureDimentions"].GetArray();
		for(auto itr = textureDimentions.Begin();itr != textureDimentions.End();itr++) {
			if(!itr->IsObject())continue;
			rapidjson::GenericObject dimObj = itr->GetObject();

			if(!(dimObj.HasMember("width")&&dimObj["width"].IsInt()))continue;
			if(!(dimObj.HasMember("height")&&dimObj["height"].IsInt()))continue;

			ImageAtlasTextureDimention_ dim;
			dim.width = dimObj["width"].GetInt();
			dim.height = dimObj["height"].GetInt();
			atlas.dimentions.push_back(dim);
		}
		rapidjson::GenericArray textureHashes = root["textureHashes"].GetArray();
		for(auto itr = textureHashes.Begin();itr != textureHashes.End();itr++) {
			if(!itr->IsObject())continue;
			rapidjson::GenericObject hashObj = itr->GetObject();

			if(!(hashObj.HasMember("hash")&&hashObj["hash"].IsUint()))continue;
			if(!(hashObj.HasMember("flags")&&hashObj["flags"].IsUint()))continue;

			uint32_t hash = hashObj["hash"].GetUint();
			uint16_t flags = hashObj["flags"].GetUint();
			std::string name;
			if(hashObj.HasMember("name")&&hashObj["name"].IsString())
				name = hashObj["name"].GetString();
			else
				name = std::format("0x{:X}",hash);
			imageAssetMap.insert({ hash, {name,imageAtlases.size(),atlas.hashes.size(),flags} });
			atlas.hashes.push_back(hash);
			atlas.names.push_back(name);

		}

		rapidjson::GenericArray renderOffsets = root["renderOffsets"].GetArray();
		for(auto itr = renderOffsets.Begin();itr != renderOffsets.End();itr++) {
			if(!itr->IsObject())continue;
			rapidjson::GenericObject renderOff = itr->GetObject();

			if(!(renderOff.HasMember("f0")&&renderOff["f0"].IsNumber()))continue;
			if(!(renderOff.HasMember("f1")&&renderOff["f1"].IsNumber()))continue;
			if(!(renderOff.HasMember("endX")&&renderOff["endX"].IsNumber()))continue;
			if(!(renderOff.HasMember("endY")&&renderOff["endY"].IsNumber()))continue;
			if(!(renderOff.HasMember("startX")&&renderOff["startX"].IsNumber()))continue;
			if(!(renderOff.HasMember("startY")&&renderOff["startY"].IsNumber()))continue;
			if(!(renderOff.HasMember("unkX")&&renderOff["unkX"].IsNumber()))continue;
			if(!(renderOff.HasMember("unkY")&&renderOff["unkY"].IsNumber()))continue;


			uiImageAtlasUnk_ offset;
			offset.m128_0.m128_f32[0] = renderOff["f0"].GetFloat();
			offset.m128_0.m128_f32[1] = renderOff["f1"].GetFloat();
			offset.m128_0.m128_f32[2] = renderOff["endX"].GetFloat();
			offset.m128_0.m128_f32[3] = renderOff["endY"].GetFloat();
			offset.m128_10.m128_f32[0] = renderOff["startX"].GetFloat();
			offset.m128_10.m128_f32[1] = renderOff["startY"].GetFloat();
			offset.m128_10.m128_f32[2] = renderOff["unkX"].GetFloat();
			offset.m128_10.m128_f32[3] = renderOff["unkY"].GetFloat();

			atlas.renderOffsets.push_back(offset);
		}
			
		rapidjson::GenericArray shaderData = root["shaderData"].GetArray();
		for(auto itr = shaderData.Begin();itr != shaderData.End();itr++) {
			if(!itr->IsObject())continue;
			rapidjson::GenericObject shaderData = itr->GetObject();

			if(!(shaderData.HasMember("minX")&&shaderData["minX"].IsNumber()))continue;
			if(!(shaderData.HasMember("minY")&&shaderData["minY"].IsNumber()))continue;
			if(!(shaderData.HasMember("maxX")&&shaderData["maxX"].IsNumber()))continue;
			if(!(shaderData.HasMember("maxY")&&shaderData["maxY"].IsNumber()))continue;

			ShaderSizeData_t shdDat;
			shdDat.minX = shaderData["minX"].GetFloat();
			shdDat.minY = shaderData["minY"].GetFloat();
			shdDat.sizeX = shaderData["maxX"].GetFloat();
			shdDat.sizeY = shaderData["maxY"].GetFloat();
			atlas.shaderData.push_back(shdDat);
		}

		fs::path ddsName = jsonName.replace_extension("dds");

		atlas.textureId = render->LoadTexture(ddsName);

		if (atlas.shaderData.size()) {
			atlas.shaderDataId = render->CreateShaderDataBuffer(atlas.shaderData);
		}
		else {
			atlas.shaderDataId = ~0ull;
		}


		imageAtlases.push_back(atlas);
		


	}

}

unsigned __int64 calculateRpakHash(const char* a1a) {

	uint32_t* v1; // r8
	uint64_t         v2; // r10
	int                   v3; // er11
	uint32_t         v4; // er9
	uint32_t          i; // edx
	uint64_t         v6; // rcx
	int                   v7; // er9
	int                   v8; // edx
	int                   v9; // eax
	uint32_t        v10; // er8
	int                  v12; // ecx
	uint32_t* a1 = (uint32_t*)a1a;

	v1 = a1;
	v2 = 0i64;
	v3 = 0;
	v4 = (*a1 - 45 * ((~(*a1 ^ 0x5C5C5C5Cu) >> 7) & (((*a1 ^ 0x5C5C5C5Cu) - 0x1010101) >> 7) & 0x1010101)) & 0xDFDFDFDF;
	for (i = ~*a1 & (*a1 - 0x1010101) & 0x80808080; !i; i = v8 & 0x80808080)
	{
		v6 = v4;
		v7 = v1[1];
		++v1;
		v3 += 4;
		v2 = ((((uint64_t)(0xFB8C4D96501i64 * v6) >> 24) + 0x633D5F1 * v2) >> 61) ^ (((uint64_t)(0xFB8C4D96501i64 * v6) >> 24)
			+ 0x633D5F1 * v2);
		v8 = ~v7 & (v7 - 0x1010101);
		v12 = 45 * ((~(v7 ^ 0x5C5C5C5Cu) >> 7) & (((v7 ^ 0x5C5C5C5Cu) - 0x1010101) >> 7) & 0x1010101);
		v4 = (v7 - v12) & 0xDFDFDFDF;

	}
	v9 = -1;
	v10 = (i & -(signed)i) - 1;
	if (_BitScanReverse((unsigned long*)&v12, v10))
	{
		v9 = v12;
	}
	return 0x633D5F1 * v2 + ((0xFB8C4D96501i64 * (uint64_t)(v4 & v10)) >> 24) - 0xAE502812AA7333i64 * (uint32_t)(v3 + v9 / 8);

}



uint32_t calculateUimgHash(const char* a1) {
	uint64_t hash = calculateRpakHash(a1);
	return (uint32_t)hash ^ (hash >> 32);
}

uint32_t loadAsset(const char* a2)
{

	uint32_t nameHash;

	if (!a2 || !*a2)
		return INVALID_ASSET;

	nameHash = calculateUimgHash(a2);

	if (imageAssetMap.contains(nameHash))
		return nameHash;
	nameHash = calculateUimgHash("missing");
	if (imageAssetMap.contains(nameHash))
		return nameHash;
	return INVALID_ASSET;

}