#include "ArgumentNodes.h"
#include "imgui/imgui_stdlib.h"

IntArgNode::IntArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Int Argument");
	setStyle(styles.argumentNode);
	ImFlow::BaseNode::addOUT<IntVariable>("Value",styles.intVariable)->behaviour([this]() {
		IntVariable var;
		var.isConstant = false;
		var.value = proto.arguments[name].defaultVal.intVal;
		return var;

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
		IntVariable var;
		var.isConstant = false;
		var.value = proto.arguments[name].defaultVal.intVal;
		return var;

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
		FloatVariable var;
		var.isConstant = false;
		var.value = proto.arguments[name].defaultVal.floatVal[0];
		return var;

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
		Float2Variable var;
		var.isConstant = false;
		var.value[0] = proto.arguments[name].defaultVal.floatVal[0];
		var.value[1] = proto.arguments[name].defaultVal.floatVal[1];
		return var;

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
		Float3Variable var;
		var.isConstant = false;
		var.value[0] = proto.arguments[name].defaultVal.floatVal[0];
		var.value[1] = proto.arguments[name].defaultVal.floatVal[1];
		var.value[2] = proto.arguments[name].defaultVal.floatVal[2];
		return var;

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
		ColorVariable var;
		var.isConstant = false;
		var.value[0] = proto.arguments[name].defaultVal.floatVal[0];
		var.value[1] = proto.arguments[name].defaultVal.floatVal[1];
		var.value[2] = proto.arguments[name].defaultVal.floatVal[2];
		var.value[3] = proto.arguments[name].defaultVal.floatVal[3];
		return var;

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
		StringVariable var;
		var.isConstant = false;
		var.value = proto.arguments[name].defaultVal.stringVal;
		return var;

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
		AssetVariable var;
		var.isConstant = false;
		var.value = proto.arguments[name].defaultVal.stringVal;
		return var;

	});
}

void AssetArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&name);
	ImGui::InputText("Default Value",&proto.arguments[name].defaultVal.stringVal);
	ImGui::PopItemWidth();
}
