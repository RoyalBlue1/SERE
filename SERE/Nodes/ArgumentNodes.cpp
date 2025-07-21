#include "ArgumentNodes.h"
#include "imgui/imgui_stdlib.h"

IntArgNode::IntArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Int Argument");
	setStyle(styles.argumentNode);
	ImFlow::BaseNode::addOUT<IntVariable>("Value",styles.intVariable)->behaviour([this]() {
		return IntVariable(proto.arguments[name].defaultVal.intVal,false);

	});
}

void IntArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&name);
	ImGui::InputInt("Default Value",&proto.arguments[name].defaultVal.intVal);
	ImGui::PopItemWidth();
}

BoolArgNode::BoolArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Bool Argument");
	setStyle(styles.argumentNode);
	ImFlow::BaseNode::addOUT<IntVariable>("Value",styles.boolVariable)->behaviour([this]() {
		return IntVariable(proto.arguments[name].defaultVal.intVal,false);
	});
}

void BoolArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&name);
	ImGui::InputInt("Default Value",&proto.arguments[name].defaultVal.intVal);
	ImGui::PopItemWidth();
}

FloatArgNode::FloatArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Float Argument");
	setStyle(styles.argumentNode);
	ImFlow::BaseNode::addOUT<FloatVariable>("Value",styles.floatVariable)->behaviour([this]() {
		return FloatVariable(proto.arguments[name].defaultVal.floatVal[0],false);
	});
}

void FloatArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&name);
	ImGui::InputFloat("Default Value",proto.arguments[name].defaultVal.floatVal);
	ImGui::PopItemWidth();
}


Float2ArgNode::Float2ArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Vector2 Argument");
	setStyle(styles.argumentNode);
	ImFlow::BaseNode::addOUT<Float2Variable>("Value",styles.float2Variable)->behaviour([this]() {
		return Float2Variable(
			proto.arguments[name].defaultVal.floatVal[0],
			proto.arguments[name].defaultVal.floatVal[1],
			false);

	});
}

void Float2ArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&name);
	ImGui::InputFloat2("Default Value",proto.arguments[name].defaultVal.floatVal);
	ImGui::PopItemWidth();
}

Float3ArgNode::Float3ArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Vector3 Argument");
	setStyle(styles.argumentNode);
	ImFlow::BaseNode::addOUT<Float3Variable>("Value",styles.float2Variable)->behaviour([this]() {
		return Float3Variable(
			proto.arguments[name].defaultVal.floatVal[0],
			proto.arguments[name].defaultVal.floatVal[1],
			proto.arguments[name].defaultVal.floatVal[2],
			false);


	});
}

void Float3ArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&name);
	ImGui::InputFloat3("Default Value",proto.arguments[name].defaultVal.floatVal);
	ImGui::PopItemWidth();
}

ColorArgNode::ColorArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Color Argument");
	setStyle(styles.argumentNode);
	ImFlow::BaseNode::addOUT<ColorVariable>("Value",styles.colorVariable)->behaviour([this]() {
		return ColorVariable(
			proto.arguments[name].defaultVal.floatVal[0],
			proto.arguments[name].defaultVal.floatVal[1],
			proto.arguments[name].defaultVal.floatVal[2],
			proto.arguments[name].defaultVal.floatVal[3],
			false);


	});
}

void ColorArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&name);
	ImGui::ColorPicker4("Default Value",proto.arguments[name].defaultVal.floatVal);
	ImGui::PopItemWidth();
}

StringArgNode::StringArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("String Argument");
	setStyle(styles.argumentNode);
	ImFlow::BaseNode::addOUT<StringVariable>("Value",styles.stringVariable)->behaviour([this]() {
		return StringVariable(proto.arguments[name].defaultVal.stringVal,false);

	});
}

void StringArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&name);
	ImGui::InputText("Default Value",&proto.arguments[name].defaultVal.stringVal);
	ImGui::PopItemWidth();
}

AssetArgNode::AssetArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Asset Argument");
	setStyle(styles.argumentNode);
	ImFlow::BaseNode::addOUT<AssetVariable>("Value",styles.assetVariable)->behaviour([this]() {
		return AssetVariable(proto.arguments[name].defaultVal.stringVal,false);
	});
}

void AssetArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&name);
	ImGui::InputText("Default Value",&proto.arguments[name].defaultVal.stringVal);
	ImGui::PopItemWidth();
}
