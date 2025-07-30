#pragma once

#include "RuiNodeEditor.h"
#include "CustomImGuiWidgets.h"

void AddMathNodes(NodeEditor& editor);

class MultiplyNode : public RuiBaseNode
{public:
	static inline std::string name = "Multiply";
	static inline std::string category = "Math";
private:
	
public:
	explicit MultiplyNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class AdditionNode : public RuiBaseNode
{
public:
	static inline std::string name = "Add";
	static inline std::string category = "Math";
private:
	
public:
	explicit AdditionNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();

};


class SubtractNode : public RuiBaseNode
{
public:
	static inline std::string name = "Subtract";
	static inline std::string category = "Math";
private:
	
public:
	explicit SubtractNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class DivideNode : public RuiBaseNode
{public:
	static inline std::string name = "Divide";
	static inline std::string category = "Math";
private:
	
	std::shared_ptr<ImFlow::NodeStyle> style;
	std::shared_ptr<ImFlow::NodeStyle> errorStyle;
public:
	explicit DivideNode(RenderInstance& prot,NodeStyles& style);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class ModuloNode : public RuiBaseNode
{
public:
	static inline std::string name = "Modulo";
	static inline std::string category = "Math";
private:
	
	std::shared_ptr<ImFlow::NodeStyle> style;
	std::shared_ptr<ImFlow::NodeStyle> errorStyle;
public:
	explicit ModuloNode(RenderInstance& prot,NodeStyles& style);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class AbsoluteNode : public RuiBaseNode
{
public:
	static inline std::string name = "Absolute";
	static inline std::string category = "Math";
private:
	

public:
	explicit AbsoluteNode(RenderInstance& prot,NodeStyles& style);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class SineNode : public RuiBaseNode
{public:
	static inline std::string name = "Sine";
	static inline std::string category = "Math";
private:
	

public:
	explicit SineNode(RenderInstance& prot,NodeStyles& style);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class ExponentNode : public RuiBaseNode
{
public:
	static inline std::string name = "Exponent";
	static inline std::string category = "Math";
private:
	

public:
	explicit ExponentNode(RenderInstance& prot,NodeStyles& style);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class MappingNode : public RuiBaseNode
{
public:
	static inline std::string name = "Mapping";
	static inline std::string category = "Math";
private:
	
	Mapping map;
public:
	explicit MappingNode(RenderInstance& prot,NodeStyles& style);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};