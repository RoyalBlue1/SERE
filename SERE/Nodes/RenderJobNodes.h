#pragma once

#include "RuiNodeEditor/RuiNodeEditor.h"
#include "RuiRendering/RenderFunctions.h"

void AddRenderNodes(const std::unique_ptr<NodeEditor>& editor);

class AssetRenderNode : public RuiBaseNode {
public:
	static inline std::string name = "Render Image Image Mask";
	static inline std::string category = "Image Render";

	explicit AssetRenderNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit AssetRenderNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();

	bool maskFlag;
	int layer;
};

class AssetCircleRenderNode : public RuiBaseNode {
public:
	static inline std::string name = "Render Image Circle Mask";
	static inline std::string category = "Image Render";

	explicit AssetCircleRenderNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit AssetCircleRenderNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();

	int layer;
};


class TextStyleNode : public RuiBaseNode {
public:
	static inline std::string name = "Text Style";
	static inline std::string category = "Text Render";

	explicit TextStyleNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit TextStyleNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
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

	explicit TextSizeNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit TextSizeNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class TextRenderNode : public RuiBaseNode {
public:
	static inline std::string name = "Text Render";
	static inline std::string category = "Text Render";

	explicit TextRenderNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit TextRenderNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
	int layer;
};

