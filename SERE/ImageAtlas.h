#pragma once

#include <intrin.h>
#include <map>
#include <vector>
#include <string>
#include "RenderFrameworks/RenderFramework.h"

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
	size_t textureId;
	size_t shaderDataId;
	std::vector<ShaderSizeData_t> shaderData;
	std::shared_ptr<RenderFramework> render;
	void* GetImageView() const {
		return render->GetTextureView(textureId);
	};
};


uint32_t loadAsset(const char* a2);
void loadImageAtlases(std::shared_ptr<RenderFramework> render);

extern std::map<uint32_t,Asset_t> imageAssetMap;
extern std::vector<ImageAtlas> imageAtlases;