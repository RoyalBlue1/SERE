#pragma once

#include "RuiNodeEditor/RuiNodeEditor.h"


class TimeNode : public RuiBaseNode
{
public:
	static inline std::string name = "Current Time";
	static inline std::string category = "Globals";

	explicit TimeNode(RenderInstance& prot,ImFlow::StyleManager& styles);
	explicit TimeNode(RenderInstance& prot,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class ADSFracNode : public RuiBaseNode
{
public:
	static inline std::string name = "ADS Fraction";
	static inline std::string category = "Globals";

	explicit ADSFracNode(RenderInstance& prot, ImFlow::StyleManager& styles);
	explicit ADSFracNode(RenderInstance& prot, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};


class LocalPlayerPosNode : public RuiBaseNode
{
public:
	static inline std::string name = "Local Player Position";
	static inline std::string category = "Globals";

	explicit LocalPlayerPosNode(RenderInstance& prot, ImFlow::StyleManager& styles);
	explicit LocalPlayerPosNode(RenderInstance& prot, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	float minVal;
	float maxVal;
};

class ScreenWidthNode : public RuiBaseNode
{
public:
	static inline std::string name = "Screen Width";
	static inline std::string category = "Globals";

	explicit ScreenWidthNode(RenderInstance& prot, ImFlow::StyleManager& styles);
	explicit ScreenWidthNode(RenderInstance& prot, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	float minVal;
	float maxVal;
};

class ScreenHeightNode : public RuiBaseNode
{
public:
	static inline std::string name = "Screen Height";
	static inline std::string category = "Globals";

	explicit ScreenHeightNode(RenderInstance& prot, ImFlow::StyleManager& styles);
	explicit ScreenHeightNode(RenderInstance& prot, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	float minVal;
	float maxVal;
};

void AddGlobalNodes(NodeEditor& editor);
