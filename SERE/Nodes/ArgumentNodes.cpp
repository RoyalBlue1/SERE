#include "ArgumentNodes.h"
#include "imgui/imgui_stdlib.h"




IntArgNode::IntArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addOUT<IntVariable>("Value",styles.intVariable)->behaviour([this]() {
		return IntVariable(proto.arguments[argName].defaultVal.intVal,false);

	});
}

void IntArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	ImGui::InputInt("Default Value",&proto.arguments[argName].defaultVal.intVal);
	ImGui::PopItemWidth();
}

std::vector<PinInfo> IntArgNode::GetPinInfo() {
	return {};
}

BoolArgNode::BoolArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addOUT<IntVariable>("Value",styles.boolVariable)->behaviour([this]() {
		return IntVariable(proto.arguments[argName].defaultVal.intVal,false);
	});
}


void BoolArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	ImGui::InputInt("Default Value",&proto.arguments[argName].defaultVal.intVal);
	ImGui::PopItemWidth();
}

std::vector<PinInfo> BoolArgNode::GetPinInfo() {
	return {};
}

FloatArgNode::FloatArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addOUT<FloatVariable>("Value",styles.floatVariable)->behaviour([this]() {
		return FloatVariable(proto.arguments[argName].defaultVal.floatVal[0],false);
	});
}

void FloatArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	ImGui::InputFloat("Default Value",proto.arguments[argName].defaultVal.floatVal);
	ImGui::PopItemWidth();
}

std::vector<PinInfo> FloatArgNode::GetPinInfo() {
	return {};
}

Float2ArgNode::Float2ArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addOUT<Float2Variable>("Value",styles.float2Variable)->behaviour([this]() {
		return Float2Variable(
			proto.arguments[argName].defaultVal.floatVal[0],
			proto.arguments[argName].defaultVal.floatVal[1],
			false);

	});
}

void Float2ArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	ImGui::InputFloat2("Default Value",proto.arguments[argName].defaultVal.floatVal);
	ImGui::PopItemWidth();
}

std::vector<PinInfo> Float2ArgNode::GetPinInfo() {
	return {};
}

Float3ArgNode::Float3ArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addOUT<Float3Variable>("Value",styles.float2Variable)->behaviour([this]() {
		return Float3Variable(
			proto.arguments[argName].defaultVal.floatVal[0],
			proto.arguments[argName].defaultVal.floatVal[1],
			proto.arguments[argName].defaultVal.floatVal[2],
			false);


	});
}

void Float3ArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	ImGui::InputFloat3("Default Value",proto.arguments[argName].defaultVal.floatVal);
	ImGui::PopItemWidth();
}

std::vector<PinInfo> Float3ArgNode::GetPinInfo() {
	return {};
}

ColorArgNode::ColorArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addOUT<ColorVariable>("Value",styles.colorVariable)->behaviour([this]() {
		return ColorVariable(
			proto.arguments[argName].defaultVal.floatVal[0],
			proto.arguments[argName].defaultVal.floatVal[1],
			proto.arguments[argName].defaultVal.floatVal[2],
			proto.arguments[argName].defaultVal.floatVal[3],
			false);


	});
}

void ColorArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	ImGui::ColorPicker4("Default Value",proto.arguments[argName].defaultVal.floatVal);
	ImGui::PopItemWidth();
}

std::vector<PinInfo> ColorArgNode::GetPinInfo() {
	return {};
}

StringArgNode::StringArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addOUT<StringVariable>("Value",styles.stringVariable)->behaviour([this]() {
		return StringVariable(proto.arguments[argName].defaultVal.stringVal,false);

	});
}

void StringArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	ImGui::InputText("Default Value",&proto.arguments[argName].defaultVal.stringVal);
	ImGui::PopItemWidth();
}

std::vector<PinInfo> StringArgNode::GetPinInfo() {
	return {};
}

AssetArgNode::AssetArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addOUT<AssetVariable>("Value",styles.assetVariable)->behaviour([this]() {
		return AssetVariable(proto.arguments[argName].defaultVal.stringVal,false);
	});
}

void AssetArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	ImGui::InputText("Default Value",&proto.arguments[argName].defaultVal.stringVal);
	ImGui::PopItemWidth();
}

std::vector<PinInfo> AssetArgNode::GetPinInfo() {
	return {};
}

void AddArgumentNodes(NodeEditor& editor) {
	editor.AddNodeType<IntArgNode>();
	editor.AddNodeType<BoolArgNode>();
	editor.AddNodeType<FloatArgNode>();
	editor.AddNodeType<Float2ArgNode>();
	editor.AddNodeType<Float3ArgNode>();
	editor.AddNodeType<ColorArgNode>();
	editor.AddNodeType<StringArgNode>();
	editor.AddNodeType<AssetArgNode>();

}