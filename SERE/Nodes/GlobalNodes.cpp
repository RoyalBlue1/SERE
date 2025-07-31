#include "GlobalNodes.h"


TimeNode::TimeNode(RenderInstance& rend,NodeStyles& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<FloatVariable>("Time")->behaviour([this]() {
		return FloatVariable(render.globals.currentTime,false,1);
	});

}

TimeNode::TimeNode(RenderInstance& rend,NodeStyles& style, rapidjson::GenericObject<false,rapidjson::Value> obj):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<FloatVariable>("Time")->behaviour([this]() {
		return FloatVariable(render.globals.currentTime,false,1);
	});

}

void TimeNode::draw() {


	ImGui::Text("Time %f", render.globals.currentTime);


}

void TimeNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> TimeNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Time"));
	return info;
}

void AddGlobalNodes(NodeEditor& editor) {
	editor.AddNodeType<TimeNode>();
}