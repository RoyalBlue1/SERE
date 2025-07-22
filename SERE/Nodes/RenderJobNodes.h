#pragma once

#include "RuiNodeEditor.h"
#include "RenderFunctions.h"

void AddRenderNodes(NodeEditor& editor);

class AssetRenderNode : public ImFlow::BaseNode {
public:
	static inline std::string name = "Render Image";
	static inline std::string category = "Image Render";
private:
	RenderInstance& proto;
public:
	explicit AssetRenderNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};


class TextStyleNode : public ImFlow::BaseNode {
public:
	static inline std::string name = "Text Style";
	static inline std::string category = "Text Render";
private:
	RenderInstance& proto;
	Font_t* currentFont;
public:
	explicit TextStyleNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class TextSizeNode : public ImFlow::BaseNode {
public:
	static inline std::string name = "Text Size";
	static inline std::string category = "Text Render";
private:
	RenderInstance& proto;
public:
	explicit TextSizeNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class TextRenderNode : public ImFlow::BaseNode {
public:
	static inline std::string name = "Text Render";
	static inline std::string category = "Text Render";
private:
	RenderInstance& proto;
public:
	explicit TextRenderNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

