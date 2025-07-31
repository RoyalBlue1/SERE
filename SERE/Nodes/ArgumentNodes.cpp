#include "ArgumentNodes.h"
#include "imgui/imgui_stdlib.h"
#include "Imgui/ImNodeFlow.h"



IntArgNode::IntArgNode(RenderInstance& rend,NodeStyles& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<IntVariable>("Value")->behaviour([this]() {
		return IntVariable(render.arguments[argName].defaultVal.intVal,false);

	});
}

IntArgNode::IntArgNode(RenderInstance& rend,NodeStyles& style, rapidjson::GenericObject<false,rapidjson::Value> obj):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	if(obj.HasMember("ArgName")&&obj["ArgName"].IsString())
		argName = obj["ArgName"].GetString();

	getOut<IntVariable>("Value")->behaviour([this]() {
		return IntVariable(render.arguments[argName].defaultVal.intVal,false);

	});
}


void IntArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	ImGui::InputInt("Default Value",&render.arguments[argName].defaultVal.intVal);
	ImGui::PopItemWidth();
}

void IntArgNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("ArgName",argName,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> IntArgNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<IntVariable>>("Value"));
	return info;
}

BoolArgNode::BoolArgNode(RenderInstance& rend,NodeStyles& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<BoolVariable>("Value")->behaviour([this]() {
		return BoolVariable(render.arguments[argName].defaultVal.intVal,false);
	});
}

BoolArgNode::BoolArgNode(RenderInstance& rend,NodeStyles& style, rapidjson::GenericObject<false,rapidjson::Value> obj):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	
	if(obj.HasMember("ArgName")&&obj["ArgName"].IsString())
		argName = obj["ArgName"].GetString();

	getOut<BoolVariable>("Value")->behaviour([this]() {
		return BoolVariable(render.arguments[argName].defaultVal.intVal,false);
	});
}


void BoolArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	ImGui::InputInt("Default Value",&render.arguments[argName].defaultVal.intVal);
	ImGui::PopItemWidth();
}

void BoolArgNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("ArgName",argName,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> BoolArgNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<BoolVariable>>("Value"));
	return info;
}

FloatArgNode::FloatArgNode(RenderInstance& rend,NodeStyles& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<FloatVariable>("Value")->behaviour([this]() {
		return FloatVariable(render.arguments[argName].defaultVal.floatVal[0],false);
	});
}

FloatArgNode::FloatArgNode(RenderInstance& rend,NodeStyles& style, rapidjson::GenericObject<false,rapidjson::Value> obj):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	if(obj.HasMember("ArgName")&&obj["ArgName"].IsString())
		argName = obj["ArgName"].GetString();

	getOut<FloatVariable>("Value")->behaviour([this]() {
		return FloatVariable(render.arguments[argName].defaultVal.floatVal[0],false);
	});
}

void FloatArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	ImGui::InputFloat("Default Value",render.arguments[argName].defaultVal.floatVal);
	ImGui::PopItemWidth();
}

void FloatArgNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("ArgName",argName,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> FloatArgNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Value"));
	return info;
}

Float2ArgNode::Float2ArgNode(RenderInstance& rend,NodeStyles& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<Float2Variable>("Value")->behaviour([this]() {
		return Float2Variable(
			render.arguments[argName].defaultVal.floatVal[0],
			render.arguments[argName].defaultVal.floatVal[1],
			false);

	});
}

Float2ArgNode::Float2ArgNode(RenderInstance& rend,NodeStyles& style, rapidjson::GenericObject<false,rapidjson::Value> obj):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	if(obj.HasMember("ArgName")&&obj["ArgName"].IsString())
		argName = obj["ArgName"].GetString();

	getOut<Float2Variable>("Value")->behaviour([this]() {
		return Float2Variable(
			render.arguments[argName].defaultVal.floatVal[0],
			render.arguments[argName].defaultVal.floatVal[1],
			false);

	});
}

void Float2ArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	ImGui::InputFloat2("Default Value",render.arguments[argName].defaultVal.floatVal);
	ImGui::PopItemWidth();
}

void Float2ArgNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("ArgName",argName,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> Float2ArgNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float2Variable>>("Value"));
	return info;
}

Float3ArgNode::Float3ArgNode(RenderInstance& rend,NodeStyles& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<Float3Variable>("Value")->behaviour([this]() {
		return Float3Variable(
			render.arguments[argName].defaultVal.floatVal[0],
			render.arguments[argName].defaultVal.floatVal[1],
			render.arguments[argName].defaultVal.floatVal[2],
			false);

	});
}

Float3ArgNode::Float3ArgNode(RenderInstance& rend,NodeStyles& style, rapidjson::GenericObject<false,rapidjson::Value> obj):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	if(obj.HasMember("ArgName")&&obj["ArgName"].IsString())
		argName = obj["ArgName"].GetString();

	getOut<Float3Variable>("Value")->behaviour([this]() {
		return Float3Variable(
			render.arguments[argName].defaultVal.floatVal[0],
			render.arguments[argName].defaultVal.floatVal[1],
			render.arguments[argName].defaultVal.floatVal[2],
			false);

	});
}

void Float3ArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	ImGui::InputFloat3("Default Value",render.arguments[argName].defaultVal.floatVal);
	ImGui::PopItemWidth();
}

void Float3ArgNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("ArgName",argName,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> Float3ArgNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float3Variable>>("Value"));
	return info;
}

ColorArgNode::ColorArgNode(RenderInstance& rend,NodeStyles& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<ColorVariable>("Value")->behaviour([this]() {
		return ColorVariable(
			render.arguments[argName].defaultVal.floatVal[0],
			render.arguments[argName].defaultVal.floatVal[1],
			render.arguments[argName].defaultVal.floatVal[2],
			render.arguments[argName].defaultVal.floatVal[3],
			false);
	});
}

ColorArgNode::ColorArgNode(RenderInstance& rend,NodeStyles& style, rapidjson::GenericObject<false,rapidjson::Value> obj):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	if(obj.HasMember("ArgName")&&obj["ArgName"].IsString())
		argName = obj["ArgName"].GetString();

	getOut<ColorVariable>("Value")->behaviour([this]() {
		return ColorVariable(
			render.arguments[argName].defaultVal.floatVal[0],
			render.arguments[argName].defaultVal.floatVal[1],
			render.arguments[argName].defaultVal.floatVal[2],
			render.arguments[argName].defaultVal.floatVal[3],
			false);
	});
}

void ColorArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	ImGui::ColorPicker4("Default Value",render.arguments[argName].defaultVal.floatVal);
	ImGui::PopItemWidth();
}

void ColorArgNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("ArgName",argName,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ColorArgNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<ColorVariable>>("Value"));
	return info;
}

StringArgNode::StringArgNode(RenderInstance& rend,NodeStyles& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<StringVariable>("Value")->behaviour([this]() {
		return StringVariable(render.arguments[argName].defaultVal.stringVal,false);

	});
}

StringArgNode::StringArgNode(RenderInstance& rend,NodeStyles& style, rapidjson::GenericObject<false,rapidjson::Value> obj):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	if(obj.HasMember("ArgName")&&obj["ArgName"].IsString())
		argName = obj["ArgName"].GetString();

	getOut<StringVariable>("Value")->behaviour([this]() {
		return StringVariable(render.arguments[argName].defaultVal.stringVal,false);

	});
}

void StringArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	ImGui::InputText("Default Value",&render.arguments[argName].defaultVal.stringVal);
	ImGui::PopItemWidth();
}

void StringArgNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("ArgName",argName,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> StringArgNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<StringVariable>>("Value"));
	return info;
}

AssetArgNode::AssetArgNode(RenderInstance& rend,NodeStyles& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<AssetVariable>("Value")->behaviour([this]() {
		return AssetVariable(render.arguments[argName].defaultVal.stringVal,false);
	});
}

AssetArgNode::AssetArgNode(RenderInstance& rend,NodeStyles& style, rapidjson::GenericObject<false,rapidjson::Value> obj):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	if(obj.HasMember("ArgName")&&obj["ArgName"].IsString())
		argName = obj["ArgName"].GetString();

	getOut<AssetVariable>("Value")->behaviour([this]() {
		return AssetVariable(render.arguments[argName].defaultVal.stringVal,false);
	});
}

void AssetArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	ImGui::InputText("Default Value",&render.arguments[argName].defaultVal.stringVal);
	ImGui::PopItemWidth();
}

void AssetArgNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("ArgName",argName,allocator);
	RuiBaseNode::Serialize(obj,allocator);
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