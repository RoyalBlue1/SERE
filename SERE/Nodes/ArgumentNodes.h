#pragma once

#include "RuiNodeEditor.h"

void AddArgumentNodes(NodeEditor& editor);

class IntArgNode : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Integer Arg";
	static inline std::string category = "Argument";
private:
	RenderInstance& proto;
	std::string argName;
public:
	explicit IntArgNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class BoolArgNode : public ImFlow::BaseNode
{	
public:
	static inline std::string name = "Boolean Arg";
	static inline std::string category = "Argument";
private:
	RenderInstance& proto;
	std::string argName;
public:
	explicit BoolArgNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class FloatArgNode : public ImFlow::BaseNode
{	
public:
	static inline std::string name = "Float Arg";
	static inline std::string category = "Argument";
private:
	RenderInstance& proto;
	std::string argName;
public:
	explicit FloatArgNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class Float2ArgNode : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Vector2 Arg";
	static inline std::string category = "Argument";
private:
	RenderInstance& proto;
	std::string argName;
public:
	explicit Float2ArgNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class Float3ArgNode : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Vector3 Arg";
	static inline std::string category = "Argument";
private:
	RenderInstance& proto;
	std::string argName;
public:
	explicit Float3ArgNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class ColorArgNode : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Color Arg";
	static inline std::string category = "Argument";
private:
	RenderInstance& proto;
	std::string argName;
public:
	explicit ColorArgNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class StringArgNode : public ImFlow::BaseNode
{
public:
	static inline std::string name = "String Arg";
	static inline std::string category = "Argument";
private:
	RenderInstance& proto;
	std::string argName;
public:
	explicit StringArgNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

class AssetArgNode : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Asset Arg";
	static inline std::string category = "Argument";
private:
	RenderInstance& proto;
	std::string argName;
public:
	explicit AssetArgNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

