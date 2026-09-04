#pragma once

#include "RuiNodeEditor/RuiNodeEditor.h"
#include <format>

class SplitFloat2Node : public RuiBaseNode
{
public:
	static inline std::string name = "Split Vector2";
	static inline std::string category = "Split Merge";

	explicit SplitFloat2Node(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit SplitFloat2Node(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};


class MergeFloat2Node : public RuiBaseNode
{
public:
	static inline std::string name = "Merge Vector2";
	static inline std::string category = "Split Merge";

	explicit MergeFloat2Node(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit MergeFloat2Node(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class SplitFloat3Node : public RuiBaseNode
{
public:
	static inline std::string name = "Split Vector3";
	static inline std::string category = "Split Merge";

	explicit SplitFloat3Node(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit SplitFloat3Node(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};


class MergeFloat3Node : public RuiBaseNode
{
public:
	static inline std::string name = "Merge Vector3";
	static inline std::string category = "Split Merge";

	explicit MergeFloat3Node(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit MergeFloat3Node(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class SplitColorNode : public RuiBaseNode
{
public:
	static inline std::string name = "Split Color RGB";
	static inline std::string category = "Split Merge";

	explicit SplitColorNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit SplitColorNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};


class RGBToColorNode : public RuiBaseNode
{public:
	static inline std::string name = "Merge Color RGB";
	static inline std::string category = "Split Merge";

	explicit RGBToColorNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit RGBToColorNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};


class HSVToColorNode : public RuiBaseNode
{public:
	static inline std::string name = "Merge Color HSV";
	static inline std::string category = "Split Merge";

	explicit HSVToColorNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit HSVToColorNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class SplitTransformSizeNode : public RuiBaseNode
{
public:
	static inline std::string name = "Split Size";
	static inline std::string category = "Split Merge";

	explicit SplitTransformSizeNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles);
	explicit SplitTransformSizeNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class MergeTransformSizeNode : public RuiBaseNode
{
public:
	static inline std::string name = "Merge Size";
	static inline std::string category = "Split Merge";

	explicit MergeTransformSizeNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles);
	explicit MergeTransformSizeNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

void AddSplitMergeNodes(const std::unique_ptr<NodeEditor>& editor);
