#pragma once

#include <intrin.h>
#include <map>
#include <vector>
#include <string>
#include <d3d11_1.h>

#define INVALID_ASSET 0xFFFFFFFF

struct textureOffset_
{
	__m128 m128_0;
	__m128 m128_10;
};

struct ImageAtlasTextureDimention_
{
	uint16_t width;
	uint16_t height;
};

struct uiImageAtlasUnk_
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

struct ImageAtlas {

	std::string name;
	std::vector<textureOffset_> offsets;
	std::vector<ImageAtlasTextureDimention_> dimentions;
	std::vector<uiImageAtlasUnk_> renderOffsets;
	std::vector<uint32_t> hashes;
	std::vector<std::string> names;
	ID3D11Resource* imageResource;
	ID3D11ShaderResourceView* imageResourceView;
	ID3D11Buffer *boundsBuffer;
	ID3D11ShaderResourceView *boundsResourceView;
	std::vector<ShaderData_t> shaderData;

};


uint32_t loadAsset(const char* a2);
void loadImageAtlases(ID3D11Device* d11Device);

extern std::map<uint32_t,Asset_t> imageAssetMap;
extern std::vector<ImageAtlas> imageAtlases;