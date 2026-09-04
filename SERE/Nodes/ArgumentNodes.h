#pragma once

#include "RuiNodeEditor/RuiNodeEditor.h"



void AddArgumentNodes(const std::unique_ptr<NodeEditor>& editor);

class IntArgNode : public RuiBaseNode
{
public:
	static inline std::string name = "Integer Arg";
	static inline std::string category = "Argument";

	explicit IntArgNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit IntArgNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	std::string argName;
};

class BoolArgNode : public RuiBaseNode
{	
public:
	static inline std::string name = "Boolean Arg";
	static inline std::string category = "Argument";

	explicit BoolArgNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit BoolArgNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	std::string argName;
};

class FloatArgNode : public RuiBaseNode
{	
public:
	static inline std::string name = "Float Arg";
	static inline std::string category = "Argument";

	explicit FloatArgNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit FloatArgNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	std::string argName;
};

class GametimeArgNode : public RuiBaseNode
{	
public:
	static inline std::string name = "Gametime Arg";
	static inline std::string category = "Argument";

	explicit GametimeArgNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit GametimeArgNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	std::string argName;
};

class Float2ArgNode : public RuiBaseNode
{
public:
	static inline std::string name = "Vector2 Arg";
	static inline std::string category = "Argument";

	explicit Float2ArgNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit Float2ArgNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	std::string argName;
};

class Float3ArgNode : public RuiBaseNode
{
public:
	static inline std::string name = "Vector3 Arg";
	static inline std::string category = "Argument";

	explicit Float3ArgNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit Float3ArgNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	std::string argName;
};

class ColorArgNode : public RuiBaseNode
{
public:
	static inline std::string name = "Color Arg";
	static inline std::string category = "Argument";

	explicit ColorArgNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit ColorArgNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	std::string argName;
};

class StringArgNode : public RuiBaseNode
{
public:
	static inline std::string name = "String Arg";
	static inline std::string category = "Argument";

	explicit StringArgNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit StringArgNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	std::string argName;
};

class AssetArgNode : public RuiBaseNode
{
public:
	static inline std::string name = "Asset Arg";
	static inline std::string category = "Argument";

	explicit AssetArgNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit AssetArgNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	std::string argName;
};

class UiHandleArgNode : public RuiBaseNode
{
public:
	static inline std::string name = "Ui Handle Arg";
	static inline std::string category = "Argument";

	explicit UiHandleArgNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles);
	explicit UiHandleArgNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	std::string argName;
};

