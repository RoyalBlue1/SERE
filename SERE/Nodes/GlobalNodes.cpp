#include "GlobalNodes.h"


TimeNode::TimeNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Time")->behaviour([this,outName]() {
		return FloatVariable(render.globals.currentTime,outName);
	});

}

TimeNode::TimeNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):TimeNode(rend,style) {


}

void TimeNode::draw() {


	ImGui::Text("Time %f", render.globals.currentTime);


}

void TimeNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void TimeNode::Export(RuiExportPrototype& proto) {
	auto out = getOut<FloatVariable>("Time")->val();
	ExportElement<std::string> ele;
	ele.identifier = out.name;
	ele.callback = [out](RuiExportPrototype& proto) {
		if(proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = globals->currentTime;",out.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = globals->currentTime;",out.GetFormattedName(proto)));
	};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> TimeNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Time"));
	return info;
}

void AddGlobalNodes(NodeEditor& editor) {
	editor.AddNodeType<TimeNode>();
}