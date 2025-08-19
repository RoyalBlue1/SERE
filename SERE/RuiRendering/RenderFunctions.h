#pragma once

#include "RuiNodeEditor/RuiNodeEditor.h"
#include <intrin.h>

struct AssetInputData {
	ColorVariable mainColor;
	ColorVariable maskColor;
	ColorVariable tertColor;
	AssetVariable mainAsset;
	AssetVariable maskAsset;
	Float2Variable mins;
	Float2Variable maxs;
	Float2Variable texMins;
	Float2Variable texMaxs;
	FloatVariable blend;
	FloatVariable premul;
	Float2Variable maskCenter;
	Float2Variable maskTranslate;
	Float2Variable maskSize;
	FloatVariable maskRotation;
	int flags;
	TransformResult transform;
	AssetInputData():
		mainColor(1.f,1.f,1.f,1.f),
		maskColor(0.f,0.f,0.f,0.f),
		tertColor(0.f,0.f,0.f,0.f),
		mainAsset("white"),
		maskAsset(INVALID_ASSET),
		mins(0.f,0.f),
		maxs(1.f,1.f),
		texMins(0.f,0.f),
		texMaxs(1.f,1.f),
		blend(1.f),
		premul(0.f),
		maskCenter(0.f,0.f),
		maskTranslate(0.f,0.f),
		maskSize(1.f,1.f),
		maskRotation(0.f),
		flags(0),
		transform()
	{ }

};

struct AssetCircleInputData {
	ColorVariable mainColor;
	ColorVariable scndColor;
	ColorVariable tertColor;
	AssetVariable mainAsset;
	Float2Variable mins;
	Float2Variable maxs;
	Float2Variable texMins;
	Float2Variable texMaxs;
	FloatVariable blend;
	FloatVariable premul;
	FloatVariable innerSliceBlend;
	FloatVariable sliceBegin;
	FloatVariable sliceEnd;
	Float2Variable ellipseSize;
	FloatVariable innerMask;
	FloatVariable vingette;
	int flags;
	TransformResult transform;
	AssetCircleInputData():
		mainColor(1.f,1.f,1.f,1.f),
		scndColor(0.f,0.f,0.f,0.f),
		tertColor(0.f,0.f,0.f,0.f),
		mainAsset("white"),
		blend(1.f),
		premul(0.f),
		mins(0.f,0.f),
		maxs(1.f,1.f),
		texMins(0.f,0.f),
		texMaxs(1.f,1.f),
		innerSliceBlend(1.f),
		sliceBegin(0.f),
		sliceEnd(1.f),
		ellipseSize(1.f,1.f),
		innerMask(1.f),
		vingette(0.f),
		flags(0x1),
		transform()
	{}
};

struct TextStyleData {
	ColorVariable mainColor;
	ColorVariable scndColor;
	ColorVariable tertColor;
	FloatVariable blend;
	FloatVariable premul;
	int fontIndex;
	FloatVariable dropShadowHardness;
	Float2Variable dropShadowOffset;
	FloatVariable dropShadowBlur;
	FloatVariable size;
	FloatVariable stretchX;
	FloatVariable backgroundSize;
	FloatVariable boltness;
	FloatVariable blur;
	FloatVariable style_32;
	TextStyleData():
		mainColor(1.f,1.f,1.f,1.f),
		scndColor(0.f,0.f,0.f,0.f),
		tertColor(0.f,0.f,0.f,0.f),
		blend(1.f),
		premul(0.f),
		fontIndex(9),
		dropShadowHardness(0.f),
		dropShadowOffset(1.f,1.f),
		dropShadowBlur(0.f),
		size(50.f),
		stretchX(1.f),
		backgroundSize(0.f),
		boltness(0.f),
		blur(0.f),
		style_32(0.f)
	{ }
};


struct InlineImage_t {
	uint32_t hash;
	uint32_t styleId;
	float afloat_4[4];
};

struct TextLine_t {
	int length;
	float float_4;
	uint32_t uint_8;
};

struct TextInputData {

	StringVariable text;
	Float2Variable minSize;
	Float2Variable maxSize;
	TextStyleData styles[4];
	std::vector<InlineImage_t> inlineImages;
	std::vector<TextLine_t> textLines;
	std::string sizeName;
	float float_0;
	float lastLine;
	TextInputData():
		text("Default Text"),
		minSize(0.f,0.f),
		maxSize(10000000.f,10000000.f),
		styles{{},{},{},{}},
		float_0(0.f),
		lastLine(0.f)
		{}
};

bool Render_Asset(RenderInstance& proto,AssetInputData& input);
bool Render_AssetSmall(RenderInstance& proto, const AssetCircleInputData& data);
bool Text_Render(RenderInstance& proto,const TextInputData& data,const TransformResult& transform);
__m128 GetTextSize(TextInputData& data);