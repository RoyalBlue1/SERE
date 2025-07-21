#pragma once

#include "RuiNodeEditor.h"

class IntVarNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
	int value;
	int minVal;
	int maxVal;
public:
	explicit IntVarNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};

class BoolVarNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
	bool value;
public:
	explicit BoolVarNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};

class FloatVarNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
	float value;
	float minVal;
	float maxVal;
public:
	explicit FloatVarNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};

class Float2VarNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
	float minVal;
	float maxVal;
	float value[2];
public:
	explicit Float2VarNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};

class Float3VarNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
	float minVal;
	float maxVal;
	float value[3];
public:
	explicit Float3VarNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};

class ColorVarNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
	float value[4];
public:
	explicit ColorVarNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};

class StringVarNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
	std::string value;
public:
	explicit StringVarNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};

class AssetVarNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
	uint32_t hash;
	bool showSelectionUi;
public:
	explicit AssetVarNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};

class SizeVarNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
	float minVal;
	float maxVal;
	float value[4];
public:
	explicit SizeVarNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};

