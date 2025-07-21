#pragma once

#include "RuiNodeEditor.h"
#include "RenderFunctions.h"

class AssetRenderNode : public ImFlow::BaseNode {
	RenderInstance& proto;
public:
	explicit AssetRenderNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};


class TextStyleNode : public ImFlow::BaseNode {
	RenderInstance& proto;
	Font_t* currentFont;
public:
	explicit TextStyleNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};

class TextSizeNode : public ImFlow::BaseNode {
	RenderInstance& proto;
public:
	explicit TextSizeNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;


};

class TextRenderNode : public ImFlow::BaseNode {
	RenderInstance& proto;
public:
	explicit TextRenderNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};