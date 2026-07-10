#pragma once

#include "RuiNodeEditor/RuiNodeEditor.h"

class SetNoRenderNode : public RuiBaseNode
{
public:
	static inline std::string name = "Set No Render";
	static inline std::string category = "Functions";

	explicit SetNoRenderNode(RenderInstance& prot, ImFlow::StyleManager& styles);
	explicit SetNoRenderNode(RenderInstance& prot, ImFlow::StyleManager& styles,
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
	explicit RandomFloatNode(RenderInstance& prot, ImFlow::StyleManager& styles);
	explicit RandomFloatNode(RenderInstance& prot, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	float randomFloat;
};


class ProjectionNode : public RuiBaseNode
{
public:
	static inline std::string name = "Project";
	static inline std::string category = "Functions";
	explicit ProjectionNode(RenderInstance& prot, ImFlow::StyleManager& style);
	explicit ProjectionNode(RenderInstance& prot, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
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
	explicit ToUpperNode(RenderInstance& prot, ImFlow::StyleManager& styles);
	explicit ToUpperNode(RenderInstance& prot, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);

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

	explicit LocalizeNode(RenderInstance& prot, ImFlow::StyleManager& styles);
	explicit LocalizeNode(RenderInstance& prot, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();

	bool Print(const std::string& fmt, std::string& out);
	std::string Format(int valId, std::string options);
	int GetPrintfString(std::string& out);
	std::string fmt;
};


void AddFunctionNodes(NodeEditor& editor);