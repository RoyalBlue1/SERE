#include "ConstantVarNodes.h"
#include "CustomImGuiWidgets.h"
#include "imgui/imgui_stdlib.h"

IntVarNode::IntVarNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Int Var");
	setStyle(styles.constantNode);
	minVal = 0;
	maxVal = 32;
	value = 0;
	ImFlow::BaseNode::addOUT<IntVariable>("Value",styles.intVariable)->behaviour([this]() {
		return IntVariable(value);
	});
}

void IntVarNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputInt("Value",&value);
	ImGui::PopItemWidth();
}

BoolVarNode::BoolVarNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Bool Var");
	setStyle(styles.constantNode);
	value = false;
	ImFlow::BaseNode::addOUT<BoolVariable>("Value",styles.boolVariable)->behaviour([this]() {
		return BoolVariable(value);

	});
}

void BoolVarNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::Selectable("Value",&value);
	ImGui::PopItemWidth();
}

FloatVarNode::FloatVarNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Float Var");
	setStyle(styles.constantNode);
	minVal = 0;
	maxVal = 1;
	value = 0;
	ImFlow::BaseNode::addOUT<FloatVariable>("Value",styles.floatVariable)->behaviour([this]() {
		return FloatVariable(value);
	});
}

void FloatVarNode::draw() {
	ImGui::PushItemWidth(80);
	ImGui::InputFloat("Min",&minVal);
	ImGui::SameLine();
	ImGui::InputFloat("Max",&maxVal);
	ImGui::PopItemWidth();
	ImGui::PushItemWidth(100);
	ImGui::SliderFloat("Value",&value,minVal,maxVal);
	ImGui::PopItemWidth();
}


Float2VarNode::Float2VarNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Vector2 Var");
	setStyle(styles.constantNode);
	minVal = 0;
	maxVal = 1;
	value[0] = 0;
	value[1] = 0;
	ImFlow::BaseNode::addOUT<Float2Variable>("Value",styles.float2Variable)->behaviour([this]() {
		return Float2Variable(value[0],value[1]);

	});
}

void Float2VarNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputFloat("Min",&minVal);
	ImGui::SameLine();
	ImGui::InputFloat("Max",&maxVal);
	ImGui::PopItemWidth();
	ImGui::PushItemWidth(210);
	ImGui::SliderFloat2("Value",value,minVal,maxVal);
	ImGui::PopItemWidth();
}

Float3VarNode::Float3VarNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Vector3 Var");
	setStyle(styles.constantNode);
	minVal = 0;
	maxVal = 1;
	value[0] = 0;
	value[1] = 0;
	value[2] = 0;
	ImFlow::BaseNode::addOUT<Float3Variable>("Value",styles.float3Variable)->behaviour([this]() {
		return Float3Variable(value[0],value[1],value[2]);

	});
}

void Float3VarNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputFloat("Min",&minVal);
	ImGui::SameLine();
	ImGui::InputFloat("Max",&maxVal);
	ImGui::PopItemWidth();
	ImGui::PushItemWidth(240);
	ImGui::SliderFloat3("Value",value,minVal,maxVal);
	ImGui::PopItemWidth();
}

ColorVarNode::ColorVarNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Color Var");
	setStyle(styles.constantNode);
	value[0] = 1.f;
	value[1] = 1.f;
	value[2] = 1.f;
	value[3] = 1.f;
	ImFlow::BaseNode::addOUT<ColorVariable>("Value",styles.colorVariable)->behaviour([this]() {
		return ColorVariable(value[0],value[1],value[2],value[3]);

	});
}

void ColorVarNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::ColorPicker4("Value",value);
	ImGui::PopItemWidth();
}

StringVarNode::StringVarNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("String Var");
	setStyle(styles.constantNode);
	ImFlow::BaseNode::addOUT<StringVariable>("Value",styles.stringVariable)->behaviour([this]() {
		return StringVariable(value);
	});
}

void StringVarNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputTextMultiline("Value",&value);
	ImGui::PopItemWidth();
}

AssetVarNode::AssetVarNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Asset Var");
	setStyle(styles.constantNode);
	hash = loadAsset("white");
	showSelectionUi = false;
	ImFlow::BaseNode::addOUT<AssetVariable>("Value",styles.assetVariable)->behaviour([this]() {
		return AssetVariable(hash);
	});
}

void AssetVarNode::draw() {
	if (AtlasImageButton("Open Selection", hash, ImVec2(200.f, 400.f))) {
		ImGui::OpenPopup("Asset Selection");
	}
	AssetSelectionPopup("Asset Selection",&hash);
	
}

SizeVarNode::SizeVarNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Color Var");
	setStyle(styles.constantNode);
	minVal = 0;
	maxVal = 128;
	value[0] = 1.f;
	value[1] = 1.f;
	value[2] = 1.f;
	value[3] = 1.f;
	ImFlow::BaseNode::addOUT<TransformSize>("Value",styles.transformSize)->behaviour([this]() {
		TransformSize var;

		var.size = _mm_load_ps(value);
		return var;

	});
}

void SizeVarNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputFloat("Min",&minVal);
	ImGui::SameLine();
	ImGui::InputFloat("Max",&maxVal);
	ImGui::PopItemWidth();
	ImGui::PushItemWidth(240);
	ImGui::SliderFloat4("Value",value,minVal,maxVal);
	ImGui::PopItemWidth();
}