#pragma once

#include <intrin.h>
#include <map>
#include <vector>
#include <string>
#include "RenderFrameworks/RenderFramework.h"
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

	ImageAtlas(fs::path& jsonName, size_t atlasIndex);
	ImageAtlas(UIImageAtlasAssetHeader_v10_t* hdr,ShaderSizeData_t* sharderData, size_t atlasIndex, size_t textureID);
	std::string name;
	std::vector<textureOffset> offsets;
	std::vector<ImageAtlasTextureDimention> dimentions;
	std::vector<uiImageAtlasUnk> renderOffsets;
	std::vector<uint32_t> hashes;
	std::vector<std::string> names;
	size_t textureId;
	size_t shaderDataId;
	std::vector<ShaderSizeData_t> shaderData;

	void* GetImageView() const {
		return g_renderFramework->GetTextureView(textureId);
	};
};


uint32_t loadAsset(const char* a2);
void loadImageAtlases();
void loadImageAtlasFromRpak(UIImageAtlasAssetHeader_v10_t* hdr, ShaderSizeData_t* shaderData, size_t textureId);

void clearImageAtlases();

extern std::map<uint32_t,Asset_t> imageAssetMap;
extern std::vector<ImageAtlas> imageAtlases;
