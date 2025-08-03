#pragma once

#include "RuiNodeEditor/RuiNodeEditor.h"


class TimeNode : public RuiBaseNode
{
public:
	static inline std::string name = "Current Time";
	static inline std::string category = "Globals";

	explicit TimeNode(RenderInstance& prot,ImFlow::StyleManager& styles);
	explicit TimeNode(RenderInstance& prot,ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj);
	void draw() override;
	void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,rapidjson::Document::AllocatorType& allocator) override;

	static std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo();
};

void AddGlobalNodes(NodeEditor& editor);