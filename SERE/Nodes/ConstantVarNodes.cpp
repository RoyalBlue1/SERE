#include "ConstantVarNodes.h"
#include "CustomImGuiWidgets.h"
#include "imgui/imgui_stdlib.h"

IntVarNode::IntVarNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	minVal = 0;
	maxVal = 32;
	value = 0;
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this);
	}
	getOut<IntVariable>("Value")->behaviour([this]() {
		return IntVariable(value);
	});
}

void IntVarNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputInt("Value",&value);
	ImGui::PopItemWidth();
}

std::vector<std::shared_ptr<ImFlow::PinProto>> IntVarNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<IntVariable>>("Value"));
	return info;
}

BoolVarNode::BoolVarNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	value = false;
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this);
	}
	getOut<BoolVariable>("Value")->behaviour([this]() {
		return BoolVariable(value);

	});
}

void BoolVarNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::Selectable("Value",&value);
	ImGui::PopItemWidth();
}

std::vector<std::shared_ptr<ImFlow::PinProto>> BoolVarNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<BoolVariable>>("Value"));
	return info;
}

FloatVarNode::FloatVarNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	minVal = 0;
	maxVal = 1;
	value = 0;
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this);
	}
	getOut<FloatVariable>("Value")->behaviour([this]() {
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

std::vector<std::shared_ptr<ImFlow::PinProto>> FloatVarNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Value"));
	return info;
}

Float2VarNode::Float2VarNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	minVal = 0;
	maxVal = 1;
	value[0] = 0;
	value[1] = 0;
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this);
	}
	getOut<Float2Variable>("Value")->behaviour([this]() {
		return Float2Variable(value[0],value[1]);

	});
}

void Float2VarNode::draw() {
	ImGui::PushItemWidth(90);
	if (Slider2D("Slider", &value[0], &value[1])) {
		ImFlow::ImNodeFlow* handler = getHandler();
		if(handler)
			handler->getGrid().config().zoom_enabled = false;
	}
	ImGui::InputFloat2("Value",value);
	ImGui::PopItemWidth();
}

std::vector<std::shared_ptr<ImFlow::PinProto>> Float2VarNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float2Variable>>("Value"));
	return info;
}

Float3VarNode::Float3VarNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	minVal = 0;
	maxVal = 1;
	value[0] = 0;
	value[1] = 0;
	value[2] = 0;
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this);
	}
	getOut<Float3Variable>("Value")->behaviour([this]() {
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

std::vector<std::shared_ptr<ImFlow::PinProto>> Float3VarNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float3Variable>>("Value"));
	return info;
}

ColorVarNode::ColorVarNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	value[0] = 1.f;
	value[1] = 1.f;
	value[2] = 1.f;
	value[3] = 1.f;
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this);
	}
	getOut<ColorVariable>("Value")->behaviour([this]() {
		return ColorVariable(value[0],value[1],value[2],value[3]);

	});
}

void ColorVarNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::ColorPicker4("Value",value);
	ImGui::PopItemWidth();
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ColorVarNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<ColorVariable>>("Value"));
	return info;
}

StringVarNode::StringVarNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this);
	}
	getOut<StringVariable>("Value")->behaviour([this]() {
		return StringVariable(value);
	});
}

void StringVarNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputTextMultiline("Value",&value);
	ImGui::PopItemWidth();
}

std::vector<std::shared_ptr<ImFlow::PinProto>> StringVarNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<StringVariable>>("Value"));
	return info;
}

AssetVarNode::AssetVarNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	hash = loadAsset("white");
	showSelectionUi = false;
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this);
	}
	getOut<AssetVariable>("Value")->behaviour([this]() {
		return AssetVariable(hash);
	});
}

std::vector<std::shared_ptr<ImFlow::PinProto>> AssetVarNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<AssetVariable>>("Value"));
	return info;
}

void AssetVarNode::draw() {
	if (AtlasImageButton("Open Selection", hash, ImVec2(200.f, 400.f))) {
		ImGui::OpenPopup("Asset Selection");
	}
	AssetSelectionPopup("Asset Selection",&hash);
	
}

SizeVarNode::SizeVarNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	minVal = 0;
	maxVal = 128;
	value[0] = 1.f;
	value[1] = 1.f;
	value[2] = 1.f;
	value[3] = 1.f;
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this);
	}
	getOut<TransformSize>("Value")->behaviour([this]() {
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

std::vector<std::shared_ptr<ImFlow::PinProto>> SizeVarNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<TransformSize>>("Value"));
	return info;
}

void AddConstantVarNodes(NodeEditor& editor) {
	editor.AddNodeType<IntVarNode>();
	editor.AddNodeType<BoolVarNode>();
	editor.AddNodeType<FloatVarNode>();
	editor.AddNodeType<Float2VarNode>();
	editor.AddNodeType<Float3VarNode>();
	editor.AddNodeType<ColorVarNode>();
	editor.AddNodeType<StringVarNode>();
	editor.AddNodeType<AssetVarNode>();
	editor.AddNodeType<SizeVarNode>();
}