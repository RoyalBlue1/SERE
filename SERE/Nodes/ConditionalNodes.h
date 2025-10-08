#pragma once

#include "RuiNodeEditor/RuiNodeEditor.h"


class GreaterNode : public RuiBaseNode
{
public:
	static inline std::string name = "Greater Than";
	static inline std::string category = "Conditionals";
private:


public:
	explicit GreaterNode(RenderInstance& prot, ImFlow::StyleManager& style);
	explicit GreaterNode(RenderInstance& prot, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class LessNode : public RuiBaseNode
{
public:
	static inline std::string name = "Less Than";
	static inline std::string category = "Conditionals";
private:


public:
	explicit LessNode(RenderInstance& prot, ImFlow::StyleManager& style);
	explicit LessNode(RenderInstance& prot, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class ConditionalFloatNode : public RuiBaseNode
{
public:
	static inline std::string name = "Conditional (Float)";
	static inline std::string category = "Conditionals";
private:


public:
	explicit ConditionalFloatNode(RenderInstance& prot, ImFlow::StyleManager& style);
	explicit ConditionalFloatNode(RenderInstance& prot, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class EqualFloatNode : public RuiBaseNode
{
public:
	static inline std::string name = "Equal (Float)";
	static inline std::string category = "Conditionals";
private:


public:
	explicit EqualFloatNode(RenderInstance& prot, ImFlow::StyleManager& style);
	explicit EqualFloatNode(RenderInstance& prot, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class NotGateNode : public RuiBaseNode
{
public:
	static inline std::string name = "NOT";
	static inline std::string category = "Conditionals";
private:


public:
	explicit NotGateNode(RenderInstance& prot, ImFlow::StyleManager& style);
	explicit NotGateNode(RenderInstance& prot, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class AndGateNode : public RuiBaseNode
{
public:
	static inline std::string name = "AND";
	static inline std::string category = "Conditionals";
private:


public:
	explicit AndGateNode(RenderInstance& prot, ImFlow::StyleManager& style);
	explicit AndGateNode(RenderInstance& prot, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class OrGateNode : public RuiBaseNode
{
public:
	static inline std::string name = "OR";
	static inline std::string category = "Conditionals";
private:


public:
	explicit OrGateNode(RenderInstance& prot, ImFlow::StyleManager& style);
	explicit OrGateNode(RenderInstance& prot, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class EqualStringNode : public RuiBaseNode
{
public:
	static inline std::string name = "Equal (String)";
	static inline std::string category = "Conditionals";
private:


public:
	explicit EqualStringNode(RenderInstance& prot, ImFlow::StyleManager& style);
	explicit EqualStringNode(RenderInstance& prot, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class ConditionalStringNode : public RuiBaseNode
{
public:
	static inline std::string name = "Conditional (String)";
	static inline std::string category = "Conditionals";
private:


public:
	explicit ConditionalStringNode(RenderInstance& prot, ImFlow::StyleManager& style);
	explicit ConditionalStringNode(RenderInstance& prot, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

void AddConditionalNodes(NodeEditor& editor);
