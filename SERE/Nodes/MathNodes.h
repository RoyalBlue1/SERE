#pragma once

#include "RuiNodeEditor.h"


class MultiplyNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
public:
	explicit MultiplyNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};

class AddNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
public:
	explicit AddNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};


class SubtractNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
public:
	explicit SubtractNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};

class DivideNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
	NodeStyles& styles;
public:
	explicit DivideNode(RenderInstance& prot,NodeStyles& style);
	void draw() override;
};

class ModuloNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
	NodeStyles& styles;
public:
	explicit ModuloNode(RenderInstance& prot,NodeStyles& style);
	void draw() override;
};

class AbsoluteNode : public ImFlow::BaseNode
{
	RenderInstance& proto;

public:
	explicit AbsoluteNode(RenderInstance& prot,NodeStyles& style);
	void draw() override;
};

class SineNode : public ImFlow::BaseNode
{
	RenderInstance& proto;

public:
	explicit SineNode(RenderInstance& prot,NodeStyles& style);
	void draw() override;
};

class ExponentNode : public ImFlow::BaseNode
{
	RenderInstance& proto;

public:
	explicit ExponentNode(RenderInstance& prot,NodeStyles& style);
	void draw() override;
};