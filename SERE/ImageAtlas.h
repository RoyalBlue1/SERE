#pragma once

#include <intrin.h>
#include <map>
#include <vector>
#include <string>
#include <d3d11_1.h>
#include <filesystem>


#define INVALID_ASSET 0xFFFFFFFF

namespace fs = std::filesystem;

struct textureOffset
{
	__m128 m128_0;
	__m128 m128_10;
};

struct ImageAtlasTextureDimention
{
	uint16_t width;
	uint16_t height;
};

struct uiImageAtlasUnk
{
	__m128 m128_0;
	__m128 m128_10;
};

struct ShaderData_t {
	float minX;
	float minY;
	float sizeX;
	float sizeY;
};

struct Asset_t {
	std::string name;
	size_t atlasIndex;
	size_t imageIndex;
	uint16_t flags;
};

struct UiAtlasImageHash {
	uint32_t hash;
	uint16_t flags;
	uint16_t nameOffset;
};


struct UIImageAtlasAssetHeader_v10_t
{
	float widthRatio;
	float heightRatio;

	uint16_t width;
	uint16_t height;

	uint16_t textureCount;
	uint16_t renderOffsetCount;

	textureOffset* textureOffsets;
	ImageAtlasTextureDimention* textureDimensions;

	uiImageAtlasUnk* renderOffsets;

	UiAtlasImageHash* textureHashes;
	const char* textureNames;
	uint64_t atlasGUID;
};



struct ImageAtlas {

	ImageAtlas(fs::path& jsonName, uint32_t atlasIndex, ID3D11Device* device);
	ImageAtlas(UIImageAtlasAssetHeader_v10_t* hdr,ShaderData_t* sharderData, uint32_t atlasIndex, ID3D11Device* device,ID3D11Texture2D* texture,ID3D11ShaderResourceView* textureView);
	std::string name;
	std::vector<textureOffset> offsets;
	std::vector<ImageAtlasTextureDimention> dimentions;
	std::vector<uiImageAtlasUnk> renderOffsets;
	std::vector<uint32_t> hashes;
	std::vector<std::string> names;
	ID3D11Resource* imageResource;
	ID3D11ShaderResourceView* imageResourceView;
	ID3D11Buffer *boundsBuffer;
	ID3D11ShaderResourceView *boundsResourceView;
	std::vector<ShaderData_t> shaderData;

	void CreateShaderDataBuffers(ID3D11Device* device);

};


uint32_t loadAsset(const char* a2);
void loadImageAtlases(ID3D11Device* d11Device);
void loadImageAtlasFromRpak(UIImageAtlasAssetHeader_v10_t* hdr, ShaderData_t* shaderData, ID3D11Device* device, ID3D11Texture2D* texture,ID3D11ShaderResourceView* textureView);

void clearImageAtlases();

extern std::map<uint32_t,Asset_t> imageAssetMap;
extern std::vector<ImageAtlas> imageAtlases;
