#pragma once

#include "RuiNodeEditor.h"
#include "CustomImGuiWidgets.h"

void AddMathNodes(NodeEditor& editor);

class MultiplyNode : public ImFlow::BaseNode
{public:
	static inline std::string name = "Multiply";
	static inline std::string category = "Math";
private:
	RenderInstance& proto;
public:
	explicit MultiplyNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class AdditionNode : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Add";
	static inline std::string category = "Math";
private:
	RenderInstance& proto;
public:
	explicit AdditionNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();

};


class SubtractNode : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Subtract";
	static inline std::string category = "Math";
private:
	RenderInstance& proto;
public:
	explicit SubtractNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class DivideNode : public ImFlow::BaseNode
{public:
	static inline std::string name = "Divide";
	static inline std::string category = "Math";
private:
	RenderInstance& proto;
	std::shared_ptr<ImFlow::NodeStyle> style;
	std::shared_ptr<ImFlow::NodeStyle> errorStyle;
public:
	explicit DivideNode(RenderInstance& prot,NodeStyles& style);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class ModuloNode : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Modulo";
	static inline std::string category = "Math";
private:
	RenderInstance& proto;
	std::shared_ptr<ImFlow::NodeStyle> style;
	std::shared_ptr<ImFlow::NodeStyle> errorStyle;
public:
	explicit ModuloNode(RenderInstance& prot,NodeStyles& style);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class AbsoluteNode : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Absolute";
	static inline std::string category = "Math";
private:
	RenderInstance& proto;

public:
	explicit AbsoluteNode(RenderInstance& prot,NodeStyles& style);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class SineNode : public ImFlow::BaseNode
{public:
	static inline std::string name = "Sine";
	static inline std::string category = "Math";
private:
	RenderInstance& proto;

public:
	explicit SineNode(RenderInstance& prot,NodeStyles& style);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class ExponentNode : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Exponent";
	static inline std::string category = "Math";
private:
	RenderInstance& proto;

public:
	explicit ExponentNode(RenderInstance& prot,NodeStyles& style);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class MappingNode : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Mapping";
	static inline std::string category = "Math";
private:
	RenderInstance& proto;
	Mapping map;
public:
	explicit MappingNode(RenderInstance& prot,NodeStyles& style);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};