#pragma once

#include "RuiNodeEditor.h"



class AssetRenderNode : public ImFlow::BaseNode {
	RenderInstance& proto;
public:
	explicit AssetRenderNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;
};