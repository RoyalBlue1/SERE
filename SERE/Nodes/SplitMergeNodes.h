#pragma once

#include "RuiNodeEditor.h"


class SplitFloat2Node : public ImFlow::BaseNode
{
	RenderInstance& proto;
public:
	explicit SplitFloat2Node(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};


class MergeFloat2Node : public ImFlow::BaseNode
{
	RenderInstance& proto;
public:
	explicit MergeFloat2Node(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};

class SplitFloat3Node : public ImFlow::BaseNode
{
	RenderInstance& proto;
public:
	explicit SplitFloat3Node(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};


class MergeFloat3Node : public ImFlow::BaseNode
{
	RenderInstance& proto;
public:
	explicit MergeFloat3Node(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};

class SplitColorNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
public:
	explicit SplitColorNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};


class RGBToColorNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
public:
	explicit RGBToColorNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};


class HSVToColorNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
public:
	explicit HSVToColorNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};