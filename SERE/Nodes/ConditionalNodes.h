#pragma once

#include "RuiNodeEditor/RuiNodeEditor.h"


class GreaterNode : public RuiBaseNode
{
public:
	static inline std::string name = "Greater Than";
	static inline std::string category = "Conditionals";
private:


public:
	explicit GreaterNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style);
	explicit GreaterNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
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
	explicit LessNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style);
	explicit LessNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
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
	explicit ConditionalFloatNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style);
	explicit ConditionalFloatNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class ConditionalValueNode : public RuiBaseNode
{
public:
	static inline std::string name = "Conditional (Any)";
	static inline std::string category = "Conditionals";
private:


public:
	explicit ConditionalValueNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style);
	explicit ConditionalValueNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
	bool CanCreateLink(ImFlow::Pin* pin, ImFlow::Pin* other) override;
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
	explicit EqualFloatNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style);
	explicit EqualFloatNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
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
	explicit NotGateNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style);
	explicit NotGateNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
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
	explicit AndGateNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style);
	explicit AndGateNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
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
	explicit OrGateNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style);
	explicit OrGateNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
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
	explicit EqualStringNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style);
	explicit EqualStringNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
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
	explicit ConditionalStringNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style);
	explicit ConditionalStringNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) override;
	void Export(RuiExportPrototype& proto) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

void AddConditionalNodes(const std::unique_ptr<NodeEditor>& editor);
