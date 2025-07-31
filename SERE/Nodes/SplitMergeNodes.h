#pragma once

#include "RuiNodeEditor.h"


class SplitFloat2Node : public RuiBaseNode
{
public:
	static inline std::string name = "Split Vector2";
	static inline std::string category = "Split Merge";

	explicit SplitFloat2Node(RenderInstance& prot,NodeStyles& styles);
	explicit SplitFloat2Node(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};


class MergeFloat2Node : public RuiBaseNode
{
public:
	static inline std::string name = "Merge Vector2";
	static inline std::string category = "Split Merge";

	explicit MergeFloat2Node(RenderInstance& prot,NodeStyles& styles);
	explicit MergeFloat2Node(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class SplitFloat3Node : public RuiBaseNode
{
public:
	static inline std::string name = "Split Vector3";
	static inline std::string category = "Split Merge";

	explicit SplitFloat3Node(RenderInstance& prot,NodeStyles& styles);
	explicit SplitFloat3Node(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};


class MergeFloat3Node : public RuiBaseNode
{
public:
	static inline std::string name = "Merge Vector3";
	static inline std::string category = "Split Merge";

	explicit MergeFloat3Node(RenderInstance& prot,NodeStyles& styles);
	explicit MergeFloat3Node(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class SplitColorNode : public RuiBaseNode
{
public:
	static inline std::string name = "Split Color RGB";
	static inline std::string category = "Split Merge";

	explicit SplitColorNode(RenderInstance& prot,NodeStyles& styles);
	explicit SplitColorNode(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};


class RGBToColorNode : public RuiBaseNode
{public:
	static inline std::string name = "Merge Color RGB";
	static inline std::string category = "Split Merge";

	explicit RGBToColorNode(RenderInstance& prot,NodeStyles& styles);
	explicit RGBToColorNode(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};


class HSVToColorNode : public RuiBaseNode
{public:
	static inline std::string name = "Merge Color HSV";
	static inline std::string category = "Split Merge";

	explicit HSVToColorNode(RenderInstance& prot,NodeStyles& styles);
	explicit HSVToColorNode(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

void AddSplitMergeNodes(NodeEditor& editor);