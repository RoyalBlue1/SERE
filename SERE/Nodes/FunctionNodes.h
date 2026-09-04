#pragma once

#include "RuiNodeEditor/RuiNodeEditor.h"

class SetNoRenderNode : public RuiBaseNode
{
public:
	static inline std::string name = "Set No Render";
	static inline std::string category = "Functions";

	explicit SetNoRenderNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles);
	explicit SetNoRenderNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles,
		rapidjson::GenericObject<false, rapidjson::Value> obj);

	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,
		rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};


class RandomFloatNode : public RuiBaseNode
{
public:
	static inline std::string name = "Random Float";
	static inline std::string category = "Functions";
	explicit RandomFloatNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles);
	explicit RandomFloatNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	float GenRandom();
	float randomFloat;
	int lastRandomChangedFrame;
};


class ProjectionNode : public RuiBaseNode
{
public:
	static inline std::string name = "Project";
	static inline std::string category = "Functions";
	explicit ProjectionNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style);
	explicit ProjectionNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class ToUpperNode : public RuiBaseNode
{
public:
	static inline std::string name = "To Upper";
	static inline std::string category = "Functions";
	explicit ToUpperNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles);
	explicit ToUpperNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);

	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;
	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};


class LocalizeNode : public RuiBaseNode
{
public:
	static inline std::string name = "Localize";
	static inline std::string category = "Functions";

	explicit LocalizeNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles);
	explicit LocalizeNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();

	std::string fmt;
};

class PrintFNode : public RuiBaseNode
{
public:
	static inline std::string name = "PrintF";
	static inline std::string category = "Functions";

	explicit PrintFNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles);
	explicit PrintFNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles,
		rapidjson::GenericObject<false, rapidjson::Value> obj);

	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,
		rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();

private:
	void AddArgumentPin();
	void SyncArgumentPins();
	bool Print(std::string& out);
	std::string FormatArgument(size_t index, const std::string& options);
	int GetPrintfString(std::string& out, std::vector<bool>* floatArgumentsAsInt = nullptr);

	std::string fmt;
	size_t argumentCount = 0;
};


void AddFunctionNodes(const std::unique_ptr<NodeEditor>& editor);