#include "GlobalNodes.h"


TimeNode::TimeNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Current Time");
	setStyle(styles.globalNode);
	ImFlow::BaseNode::addOUT<FloatVariable>("Res",styles.floatVariable)->behaviour([this]() {
		FloatVariable var;
		var.isGlobal = true;
		var.value = proto.globals.currentTime;
		return var;

	});

}

void TimeNode::draw() {


	ImGui::Text("Time %f", proto.globals.currentTime);


}