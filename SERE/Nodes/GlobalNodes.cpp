#include "GlobalNodes.h"


TimeNode::TimeNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Current Time");
	setStyle(styles.globalNode);
	ImFlow::BaseNode::addOUT<FloatVariable>("Res",styles.floatVariable)->behaviour([this]() {
		return FloatVariable(proto.globals.currentTime,false,1);
	});

}

void TimeNode::draw() {


	ImGui::Text("Time %f", proto.globals.currentTime);


}