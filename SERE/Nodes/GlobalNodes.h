#pragma once

#include "RuiNodeEditor.h"


class TimeNode : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Current Time";
	static inline std::string category = "Globals";
private:
	RenderInstance& proto;
public:
	explicit TimeNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

void AddGlobalNodes(NodeEditor& editor);