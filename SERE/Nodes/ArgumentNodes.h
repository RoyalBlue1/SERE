#pragma once

#include "RuiNodeEditor.h"

void AddArgumentNodes(NodeEditor& editor);

class IntArgNode : public RuiBaseNode
{
public:
	static inline std::string name = "Integer Arg";
	static inline std::string category = "Argument";

	explicit IntArgNode(RenderInstance& prot,NodeStyles& styles);
	explicit IntArgNode(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	std::string argName;
};

class BoolArgNode : public RuiBaseNode
{	
public:
	static inline std::string name = "Boolean Arg";
	static inline std::string category = "Argument";

	explicit BoolArgNode(RenderInstance& prot,NodeStyles& styles);
	explicit BoolArgNode(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	std::string argName;
};

class FloatArgNode : public RuiBaseNode
{	
public:
	static inline std::string name = "Float Arg";
	static inline std::string category = "Argument";

	explicit FloatArgNode(RenderInstance& prot,NodeStyles& styles);
	explicit FloatArgNode(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	std::string argName;
};

class Float2ArgNode : public RuiBaseNode
{
public:
	static inline std::string name = "Vector2 Arg";
	static inline std::string category = "Argument";

	explicit Float2ArgNode(RenderInstance& prot,NodeStyles& styles);
	explicit Float2ArgNode(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	std::string argName;
};

class Float3ArgNode : public RuiBaseNode
{
public:
	static inline std::string name = "Vector3 Arg";
	static inline std::string category = "Argument";

	explicit Float3ArgNode(RenderInstance& prot,NodeStyles& styles);
	explicit Float3ArgNode(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	std::string argName;
};

class ColorArgNode : public RuiBaseNode
{
public:
	static inline std::string name = "Color Arg";
	static inline std::string category = "Argument";

	explicit ColorArgNode(RenderInstance& prot,NodeStyles& styles);
	explicit ColorArgNode(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	std::string argName;
};

class StringArgNode : public RuiBaseNode
{
public:
	static inline std::string name = "String Arg";
	static inline std::string category = "Argument";

	explicit StringArgNode(RenderInstance& prot,NodeStyles& styles);
	explicit StringArgNode(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	std::string argName;
};

class AssetArgNode : public RuiBaseNode
{
public:
	static inline std::string name = "Asset Arg";
	static inline std::string category = "Argument";

	explicit AssetArgNode(RenderInstance& prot,NodeStyles& styles);
	explicit AssetArgNode(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
private:
	std::string argName;
};

