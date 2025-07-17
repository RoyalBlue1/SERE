#pragma once

#include "RuiNodeEditor.h"


class TimeNode : public ImFlow::BaseNode
{
	RenderInstance& proto;
public:
	explicit TimeNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};