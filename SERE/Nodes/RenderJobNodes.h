#pragma once

#include "RuiNodeEditor/RuiNodeEditor.h"
#include "RuiRendering/RenderFunctions.h"

void AddRenderNodes(NodeEditor& editor);

class AssetRenderNode : public RuiBaseNode {
public:
	static inline std::string name = "Render Image Image Mask";
	static inline std::string category = "Image Render";

	explicit AssetRenderNode(RenderInstance& prot,ImFlow::StyleManager& styles);
	explicit AssetRenderNode(RenderInstance& prot,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class AssetCircleRenderNode : public RuiBaseNode {
public:
	static inline std::string name = "Render Image Circle Mask";
	static inline std::string category = "Image Render";

	explicit AssetCircleRenderNode(RenderInstance& prot,ImFlow::StyleManager& styles);
	explicit AssetCircleRenderNode(RenderInstance& prot,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};


class TextStyleNode : public RuiBaseNode {
public:
	static inline std::string name = "Text Style";
	static inline std::string category = "Text Render";

	explicit TextStyleNode(RenderInstance& prot,ImFlow::StyleManager& styles);
	explicit TextStyleNode(RenderInstance& prot,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();

private:

	Font_t* currentFont;
};

class TextSizeNode : public RuiBaseNode {
public:
	static inline std::string name = "Text Size";
	static inline std::string category = "Text Render";

	explicit TextSizeNode(RenderInstance& prot,ImFlow::StyleManager& styles);
	explicit TextSizeNode(RenderInstance& prot,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class TextRenderNode : public RuiBaseNode {
public:
	static inline std::string name = "Text Render";
	static inline std::string category = "Text Render";

	explicit TextRenderNode(RenderInstance& prot,ImFlow::StyleManager& styles);
	explicit TextRenderNode(RenderInstance& prot,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

