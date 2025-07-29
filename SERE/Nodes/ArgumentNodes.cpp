#include "ArgumentNodes.h"
#include "imgui/imgui_stdlib.h"
#include "Imgui/ImNodeFlow.h"



IntArgNode::IntArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this,styles.pinStyles);
	}
	getOut<IntVariable>("Value")->behaviour([this]() {
		return IntVariable(proto.arguments[argName].defaultVal.intVal,false);

	});
}

void IntArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	ImGui::InputInt("Default Value",&proto.arguments[argName].defaultVal.intVal);
	ImGui::PopItemWidth();
}

std::vector<std::shared_ptr<ImFlow::PinProto>> IntArgNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<IntVariable>>("Value"));
	return info;
}

BoolArgNode::BoolArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this,styles.pinStyles);
	}
	getOut<BoolVariable>("Value")->behaviour([this]() {
		return BoolVariable(proto.arguments[argName].defaultVal.intVal,false);
	});
}


void BoolArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	ImGui::InputInt("Default Value",&proto.arguments[argName].defaultVal.intVal);
	ImGui::PopItemWidth();
}

std::vector<std::shared_ptr<ImFlow::PinProto>> BoolArgNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<BoolVariable>>("Value"));
	return info;
}

FloatArgNode::FloatArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this,styles.pinStyles);
	}
	getOut<FloatVariable>("Value")->behaviour([this]() {
		return FloatVariable(proto.arguments[argName].defaultVal.floatVal[0],false);
	});
}

void FloatArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	ImGui::InputFloat("Default Value",proto.arguments[argName].defaultVal.floatVal);
	ImGui::PopItemWidth();
}

std::vector<std::shared_ptr<ImFlow::PinProto>> FloatArgNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Value"));
	return info;
}

Float2ArgNode::Float2ArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this,styles.pinStyles);
	}
	getOut<Float2Variable>("Value")->behaviour([this]() {
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

std::vector<std::shared_ptr<ImFlow::PinProto>> Float2ArgNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float2Variable>>("Value"));
	return info;
}

Float3ArgNode::Float3ArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this,styles.pinStyles);
	}
	getOut<Float3Variable>("Value")->behaviour([this]() {
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

std::vector<std::shared_ptr<ImFlow::PinProto>> Float3ArgNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float3Variable>>("Value"));
	return info;
}

ColorArgNode::ColorArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this,styles.pinStyles);
	}
	getOut<ColorVariable>("Value")->behaviour([this]() {
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

std::vector<std::shared_ptr<ImFlow::PinProto>> ColorArgNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<ColorVariable>>("Value"));
	return info;
}

StringArgNode::StringArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this,styles.pinStyles);
	}
	getOut<StringVariable>("Value")->behaviour([this]() {
		return StringVariable(proto.arguments[argName].defaultVal.stringVal,false);

	});
}

void StringArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	ImGui::InputText("Default Value",&proto.arguments[argName].defaultVal.stringVal);
	ImGui::PopItemWidth();
}

std::vector<std::shared_ptr<ImFlow::PinProto>> StringArgNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<StringVariable>>("Value"));
	return info;
}

AssetArgNode::AssetArgNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this,styles.pinStyles);
	}
	getOut<AssetVariable>("Value")->behaviour([this]() {
		return AssetVariable(proto.arguments[argName].defaultVal.stringVal,false);
	});
}

void AssetArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	ImGui::InputText("Default Value",&proto.arguments[argName].defaultVal.stringVal);
	ImGui::PopItemWidth();
}

std::vector<std::shared_ptr<ImFlow::PinProto>> AssetArgNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<AssetVariable>>("Value"));
	return info;
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