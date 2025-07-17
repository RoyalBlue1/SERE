#pragma once

#include "RuiNodeEditor.h"

class IntArgNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
	std::string name;
public:
	explicit IntArgNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};

class BoolArgNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
	std::string name;
public:
	explicit BoolArgNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};

class FloatArgNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
	std::string name;
public:
	explicit FloatArgNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};

class Float2ArgNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
	std::string name;
public:
	explicit Float2ArgNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};

class Float3ArgNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
	std::string name;
public:
	explicit Float3ArgNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};

class ColorArgNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
	std::string name;
public:
	explicit ColorArgNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};

class StringArgNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
	std::string name;
public:
	explicit StringArgNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};

class AssetArgNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
	std::string name;
public:
	explicit AssetArgNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};

