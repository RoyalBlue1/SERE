#include "GlobalNodes.h"


TimeNode::TimeNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this,styles.pinStyles);
	}
	getOut<FloatVariable>("Time")->behaviour([this]() {
		return FloatVariable(proto.globals.currentTime,false,1);
	});

}

void TimeNode::draw() {


	ImGui::Text("Time %f", proto.globals.currentTime);


}

std::vector<std::shared_ptr<ImFlow::PinProto>> TimeNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Time"));
	return info;
}

void AddGlobalNodes(NodeEditor& editor) {
	editor.AddNodeType<TimeNode>();
}