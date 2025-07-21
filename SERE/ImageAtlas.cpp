
#include "ImageAtlas.h"

#include <filesystem>
#include <fstream>

#include "ThirdParty/yyjson.h"
#include "Thirdparty/DDSTextureLoader11.h"

namespace fs = std::filesystem;



std::map<uint32_t, Asset_t> imageAssetMap{};
std::vector<ImageAtlas> imageAtlases{};
void loadImageAtlases(ID3D11Device* d11Device) {
	fs::path folderPath = ".\\Assets\\Atlases";

	for (const auto& dirEntry : fs::recursive_directory_iterator(folderPath)) {
		if(!dirEntry.is_regular_file())continue;
		fs::path jsonName = dirEntry;
		if(jsonName.extension()!=".json")continue;

		yyjson_read_err jsonErr;
		yyjson_doc* doc = yyjson_read_file(jsonName.string().c_str(), 0, NULL, &jsonErr);
		if (doc) {
			ImageAtlas atlas;
			atlas.name = jsonName.filename().replace_extension("").string();
			yyjson_val* offsets = yyjson_obj_get(yyjson_doc_get_root(doc),"textureOffsets");
			size_t idx,size;
			yyjson_val *entry;
			yyjson_arr_foreach(offsets, idx, size, entry) {
				textureOffset_ offset;
				offset.m128_0.m128_f32[0] = yyjson_get_num(yyjson_obj_get(entry, "f0"));
				offset.m128_0.m128_f32[1] = yyjson_get_num(yyjson_obj_get(entry,"f1"));
				offset.m128_0.m128_f32[2] = yyjson_get_num(yyjson_obj_get(entry,"endX"));
				offset.m128_0.m128_f32[3] = yyjson_get_num(yyjson_obj_get(entry,"endY"));
				offset.m128_10.m128_f32[0] = yyjson_get_num(yyjson_obj_get(entry,"startX"));
				offset.m128_10.m128_f32[1] = yyjson_get_num(yyjson_obj_get(entry,"startY"));
				offset.m128_10.m128_f32[2] = yyjson_get_num(yyjson_obj_get(entry,"unkX"));
				offset.m128_10.m128_f32[3] = yyjson_get_num(yyjson_obj_get(entry,"unkY"));
				atlas.offsets.push_back(offset);
			}
			yyjson_val* dimentions = yyjson_obj_get(yyjson_doc_get_root(doc),"textureDimentions");
			yyjson_arr_foreach(dimentions, idx, size, entry) {
				ImageAtlasTextureDimention_ dim;
				dim.width = yyjson_get_num(yyjson_obj_get(entry, "width"));
				dim.height = yyjson_get_num(yyjson_obj_get(entry, "height"));
				atlas.dimentions.push_back(dim);
			}
			yyjson_val* hashes = yyjson_obj_get(yyjson_doc_get_root(doc),"textureHashes");
			yyjson_arr_foreach(hashes, idx, size, entry) {
				uint32_t hash = yyjson_get_uint(yyjson_obj_get(entry,"hash"));
				uint16_t flags = yyjson_get_uint(yyjson_obj_get(entry,"flags"));
				std::string name = yyjson_get_str(yyjson_obj_get(entry,"name"));
				imageAssetMap.insert({ hash, {name,imageAtlases.size(),atlas.hashes.size(),flags} });
				atlas.hashes.push_back(hash);
				atlas.names.push_back(name);

			}
			yyjson_val* renderOffsets = yyjson_obj_get(yyjson_doc_get_root(doc),"renderOffsets");
			yyjson_arr_foreach(renderOffsets, idx, size, entry) {
				uiImageAtlasUnk_ offset;
				offset.m128_0.m128_f32[0] = yyjson_get_num(yyjson_obj_get(entry, "f0"));
				offset.m128_0.m128_f32[1] = yyjson_get_num(yyjson_obj_get(entry,"f1"));
				offset.m128_0.m128_f32[2] = yyjson_get_num(yyjson_obj_get(entry,"endX"));
				offset.m128_0.m128_f32[3] = yyjson_get_num(yyjson_obj_get(entry,"endY"));
				offset.m128_10.m128_f32[0] = yyjson_get_num(yyjson_obj_get(entry,"startX"));
				offset.m128_10.m128_f32[1] = yyjson_get_num(yyjson_obj_get(entry,"startY"));
				offset.m128_10.m128_f32[2] = yyjson_get_num(yyjson_obj_get(entry,"unkX"));
				offset.m128_10.m128_f32[3] = yyjson_get_num(yyjson_obj_get(entry,"unkY"));
				atlas.renderOffsets.push_back(offset);
			}
			yyjson_val* jsonShaderData = yyjson_obj_get(yyjson_doc_get_root(doc),"shaderData");
			std::vector<ShaderData_t> shaderData;
			yyjson_arr_foreach(jsonShaderData, idx, size, entry) {
				ShaderData_t shdDat;
				shdDat.minX = yyjson_get_num(yyjson_obj_get(entry, "minX"));
				shdDat.minY = yyjson_get_num(yyjson_obj_get(entry, "minY"));
				shdDat.maxX = yyjson_get_num(yyjson_obj_get(entry, "maxX"));
				shdDat.maxY = yyjson_get_num(yyjson_obj_get(entry, "maxY"));
				shaderData.push_back(shdDat);
			}
			yyjson_doc_free(doc);
			fs::path ddsName = jsonName.replace_extension("dds");

			DirectX::CreateDDSTextureFromFile(d11Device,ddsName.wstring().c_str(), &atlas.imageResource, &atlas.imageResourceView);

			if (shaderData.size()) {
				D3D11_SUBRESOURCE_DATA boundSubresoureDesc;
				D3D11_SHADER_RESOURCE_VIEW_DESC boundsShaderResourceViewDesc;
				D3D11_BUFFER_DESC boundsBufferDesc;

				boundsBufferDesc.ByteWidth = 16 * shaderData.size();
				boundsBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
				boundsBufferDesc.MiscFlags = 64;
				boundsBufferDesc.StructureByteStride = 16;
				boundsBufferDesc.BindFlags =  8;
				boundsBufferDesc.CPUAccessFlags = 0;

				boundSubresoureDesc.pSysMem = shaderData.data();
				boundSubresoureDesc.SysMemPitch = 0;
				boundSubresoureDesc.SysMemSlicePitch = 0;

				if (HRESULT res = d11Device->CreateBuffer(&boundsBufferDesc, &boundSubresoureDesc, &atlas.boundsBuffer); res || (atlas.boundsBuffer ==NULL)) {
					printf("D3D11Decive::CreateBuffer returned 0x%x\n",(uint32_t)res);
				};
				boundsShaderResourceViewDesc.Format = DXGI_FORMAT_UNKNOWN;
				boundsShaderResourceViewDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
				boundsShaderResourceViewDesc.Buffer.FirstElement = 0;
				boundsShaderResourceViewDesc.Buffer.NumElements = shaderData.size();
				if (HRESULT res = d11Device->CreateShaderResourceView(atlas.boundsBuffer, &boundsShaderResourceViewDesc, &atlas.boundsResourceView); res) {
					printf("D3D11Decive::CreateShaderResourceView returned 0x%x\n",(uint32_t)res);
				}
			}
			else {
				atlas.boundsBuffer = NULL;
				atlas.boundsResourceView = NULL;
			}


			imageAtlases.push_back(atlas);
		}


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