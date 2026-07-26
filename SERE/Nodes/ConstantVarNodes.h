#pragma once

#include "RuiNodeEditor/RuiNodeEditor.h"

void AddConstantVarNodes(const std::unique_ptr<NodeEditor>& editor);

class IntVarNode : public RuiBaseNode
{
public:
	static inline std::string name = "Integer Constant";
	static inline std::string category = "Constant";

	explicit IntVarNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit IntVarNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	int value;
	int minVal;
	int maxVal;
};

class BoolVarNode : public RuiBaseNode
{
public:
	static inline std::string name = "Bool Constant";
	static inline std::string category = "Constant";

	explicit BoolVarNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit BoolVarNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	bool value;
};

class FloatVarNode : public RuiBaseNode
{
public:
	static inline std::string name = "Float Constant";
	static inline std::string category = "Constant";

	explicit FloatVarNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit FloatVarNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	float value;
	float minVal;
	float maxVal;
};

class Float2VarNode : public RuiBaseNode
{
public:
	static inline std::string name = "Vector2 Constant";
	static inline std::string category = "Constant";

	explicit Float2VarNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit Float2VarNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	float minVal;
	float maxVal;
	float value[2];
};

class Float3VarNode : public RuiBaseNode
{
public:
	static inline std::string name = "Vector3 Constant";
	static inline std::string category = "Constant";

	explicit Float3VarNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit Float3VarNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	float minVal;
	float maxVal;
	float value[3];
};

class ColorVarNode : public RuiBaseNode
{
public:
	static inline std::string name = "Color Constant";
	static inline std::string category = "Constant";

	explicit ColorVarNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit ColorVarNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	float value[4];
};

class StringVarNode : public RuiBaseNode
{
public:
	static inline std::string name = "String Constant";
	static inline std::string category = "Constant";

	explicit StringVarNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit StringVarNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	std::string value;
};

class AssetVarNode : public RuiBaseNode
{
public:
	static inline std::string name = "Asset Constant";
	static inline std::string category = "Constant";

	explicit AssetVarNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit AssetVarNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	uint32_t hash;
	bool showSelectionUi;
};

class SizeVarNode : public RuiBaseNode
{
public:
	static inline std::string name = "Size Constant";
	static inline std::string category = "Constant";

	explicit SizeVarNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit SizeVarNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	float minVal;
	float maxVal;
	float value[4];
};

