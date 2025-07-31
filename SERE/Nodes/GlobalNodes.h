#pragma once

#include "RuiNodeEditor.h"


class TimeNode : public RuiBaseNode
{
public:
	static inline std::string name = "Current Time";
	static inline std::string category = "Globals";

	explicit TimeNode(RenderInstance& prot,NodeStyles& styles);
	explicit TimeNode(RenderInstance& prot,NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

void AddGlobalNodes(NodeEditor& editor);