#include "ConstantVarNodes.h"
#include "CustomImGuiWidgets.h"
#include "imgui/imgui_stdlib.h"

IntVarNode::IntVarNode(RenderInstance& rend, ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {


	value = 0;

	getOut<IntVariable>("Value")->behaviour([this]() {
		return IntVariable(value);
	});
}

IntVarNode::IntVarNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	value = 0;
	if(obj.HasMember("Value")&&obj["Value"].IsInt())
		value = obj["Value"].GetInt();

	getOut<IntVariable>("Value")->behaviour([this]() {
		return IntVariable(value);
	});
}

void IntVarNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputInt("Value",&value);
	ImGui::PopItemWidth();
}

void IntVarNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("Value",value,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> IntVarNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<IntVariable>>("Value"));
	return info;
}

BoolVarNode::BoolVarNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	value = false;

	getOut<BoolVariable>("Value")->behaviour([this]() {
		return BoolVariable(value);

	});
}

BoolVarNode::BoolVarNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	value = false;
	if(obj.HasMember("Value")&&obj["Value"].IsBool())
		value = obj["Value"].GetBool();
	getOut<BoolVariable>("Value")->behaviour([this]() {
		return BoolVariable(value);

	});
}

void BoolVarNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::Selectable("Value",&value);
	ImGui::PopItemWidth();
}

void BoolVarNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("Value",value,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> BoolVarNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<BoolVariable>>("Value"));
	return info;
}

FloatVarNode::FloatVarNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	minVal = 0;
	maxVal = 1;
	value = 0;

	getOut<FloatVariable>("Value")->behaviour([this]() {
		return FloatVariable(value);
	});
}

FloatVarNode::FloatVarNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	minVal = 0;
	maxVal = 1;
	value = 0;

	if(obj.HasMember("Min")&&obj["Min"].IsNumber())
		minVal = obj["Min"].GetFloat();
	if(obj.HasMember("Max")&&obj["Max"].IsNumber())
		maxVal = obj["Max"].GetFloat();
	if(obj.HasMember("Value")&&obj["Value"].IsNumber())
		value = obj["Value"].GetFloat();

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

void FloatVarNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("Min",minVal, allocator);
	obj.AddMember("Max",maxVal, allocator);
	obj.AddMember("Value",value, allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> FloatVarNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Value"));
	return info;
}

Float2VarNode::Float2VarNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	minVal = 0;
	maxVal = 1;
	value[0] = 0;
	value[1] = 0;

	getOut<Float2Variable>("Value")->behaviour([this]() {
		return Float2Variable(value[0],value[1]);

	});
}

Float2VarNode::Float2VarNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	minVal = 0;
	maxVal = 1;
	value[0] = 0;
	value[1] = 0;

	if(obj.HasMember("Value_X")&&obj["Value_X"].IsNumber())
		value[0] = obj["Value_X"].GetFloat();
	if(obj.HasMember("Value_Y")&&obj["Value_Y"].IsNumber())
		value[1] = obj["Value_Y"].GetFloat();

	if(obj.HasMember("Min")&&obj["Min"].IsNumber())
		minVal = obj["Min"].GetFloat();
	if(obj.HasMember("Max")&&obj["Max"].IsNumber())
		maxVal = obj["Max"].GetFloat();

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

void Float2VarNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("Min",minVal, allocator);
	obj.AddMember("Max",maxVal, allocator);
	obj.AddMember("Value_X",value[0], allocator);
	obj.AddMember("Value_Y",value[0], allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> Float2VarNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float2Variable>>("Value"));
	return info;
}

Float3VarNode::Float3VarNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {


	minVal = 0;
	maxVal = 1;
	value[0] = 0;
	value[1] = 0;
	value[2] = 0;

	getOut<Float3Variable>("Value")->behaviour([this]() {
		return Float3Variable(value[0],value[1],value[2]);

	});
}

Float3VarNode::Float3VarNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):RuiBaseNode(name,category,GetPinInfo(),rend,style) {


	minVal = 0;
	maxVal = 1;
	value[0] = 0;
	value[1] = 0;
	value[2] = 0;

	if(obj.HasMember("Value_X")&&obj["Value_X"].IsNumber())
		value[0] = obj["Value_X"].GetFloat();
	if(obj.HasMember("Value_Y")&&obj["Value_Y"].IsNumber())
		value[1] = obj["Value_Y"].GetFloat();
	if(obj.HasMember("Value_Z")&&obj["Value_Z"].IsNumber())
		value[2] = obj["Value_Z"].GetFloat();

	if(obj.HasMember("Min")&&obj["Min"].IsNumber())
		minVal = obj["Min"].GetFloat();
	if(obj.HasMember("Max")&&obj["Max"].IsNumber())
		maxVal = obj["Max"].GetFloat();

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

void Float3VarNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("Min",minVal, allocator);
	obj.AddMember("Max",maxVal, allocator);
	obj.AddMember("Value_X",value[0], allocator);
	obj.AddMember("Value_Y",value[1], allocator);
	obj.AddMember("Value_Z",value[2], allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> Float3VarNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float3Variable>>("Value"));
	return info;
}

ColorVarNode::ColorVarNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	value[0] = 1.f;
	value[1] = 1.f;
	value[2] = 1.f;
	value[3] = 1.f;

	getOut<ColorVariable>("Value")->behaviour([this]() {
		return ColorVariable(value[0],value[1],value[2],value[3]);

	});
}

ColorVarNode::ColorVarNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	value[0] = 1.f;
	value[1] = 1.f;
	value[2] = 1.f;
	value[3] = 1.f;

	if(obj.HasMember("Value_Red")&&obj["Value_Red"].IsNumber())
		value[0] = obj["Value_Red"].GetFloat();
	if(obj.HasMember("Value_Green")&&obj["Value_Green"].IsNumber())
		value[1] = obj["Value_Green"].GetFloat();
	if(obj.HasMember("Value_Blue")&&obj["Value_Blue"].IsNumber())
		value[2] = obj["Value_Blue"].GetFloat();
	if(obj.HasMember("Value_Alpha")&&obj["Value_Alpha"].IsNumber())
		value[3] = obj["Value_Alpha"].GetFloat();

	getOut<ColorVariable>("Value")->behaviour([this]() {
		return ColorVariable(value[0],value[1],value[2],value[3]);

	});
}

void ColorVarNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::ColorPicker4("Value",value);
	ImGui::PopItemWidth();
}

void ColorVarNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("Value_Red",value[0], allocator);
	obj.AddMember("Value_Green",value[1], allocator);
	obj.AddMember("Value_Blue",value[2], allocator);
	obj.AddMember("Value_Alpha",value[3], allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ColorVarNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<ColorVariable>>("Value"));
	return info;
}

StringVarNode::StringVarNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<StringVariable>("Value")->behaviour([this]() {
		return StringVariable(value);
	});
}

StringVarNode::StringVarNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	if(obj.HasMember("Value")&&obj["Value"].IsString())
		value = obj["Value"].GetString();

	getOut<StringVariable>("Value")->behaviour([this]() {
		return StringVariable(value);
	});
}

void StringVarNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputTextMultiline("Value",&value);
	ImGui::PopItemWidth();
}

void StringVarNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("Value",value, allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> StringVarNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<StringVariable>>("Value"));
	return info;
}

AssetVarNode::AssetVarNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	hash = loadAsset("white");
	showSelectionUi = false;

	getOut<AssetVariable>("Value")->behaviour([this]() {
		return AssetVariable(hash);
	});
}

AssetVarNode::AssetVarNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	hash = loadAsset("white");

	if(obj.HasMember("Hash")&&obj["Hash"].IsInt())
		hash = obj["Hash"].GetInt();

	showSelectionUi = false;

	getOut<AssetVariable>("Value")->behaviour([this]() {
		return AssetVariable(hash);
	});
}

void AssetVarNode::draw() {
	if (AtlasImageButton("Open Selection", hash, ImVec2(200.f, 400.f))) {
		ImGui::OpenPopup("Asset Selection");
	}
	AssetSelectionPopup("Asset Selection",&hash);

}

void AssetVarNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("Hash",hash, allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> AssetVarNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<AssetVariable>>("Value"));
	return info;
}

SizeVarNode::SizeVarNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	minVal = 0;
	maxVal = 128;
	value[0] = 1.f;
	value[1] = 1.f;
	value[2] = 1.f;
	value[3] = 1.f;

	getOut<TransformSize>("Value")->behaviour([this]() {
		TransformSize var;

		var.size = _mm_load_ps(value);
		return var;

	});
}

SizeVarNode::SizeVarNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	minVal = 0;
	maxVal = 512;
	value[0] = 128.f;
	value[1] = 128.f;
	value[2] = 128.f;
	value[3] = 128.f;

	if(obj.HasMember("Min")&&obj["Min"].IsNumber())
		minVal = obj["Min"].GetFloat();
	if(obj.HasMember("Max")&&obj["Max"].IsNumber())
		maxVal = obj["Max"].GetFloat();
	if(obj.HasMember("Value_0")&&obj["Value_0"].IsNumber())
		value[0] = obj["Value_0"].GetFloat();
	if(obj.HasMember("Value_1")&&obj["Value_1"].IsNumber())
		value[1] = obj["Value_1"].GetFloat();
	if(obj.HasMember("Value_2")&&obj["Value_2"].IsNumber())
		value[2] = obj["Value_2"].GetFloat();
	if(obj.HasMember("Value_3")&&obj["Value_3"].IsNumber())
		value[3] = obj["Value_3"].GetFloat();



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

void SizeVarNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("Min",minVal, allocator);
	obj.AddMember("Max",maxVal, allocator);
	obj.AddMember("Value_0",value[0], allocator);
	obj.AddMember("Value_1",value[1], allocator);
	obj.AddMember("Value_2",value[2], allocator);
	obj.AddMember("Value_3",value[3], allocator);
	RuiBaseNode::Serialize(obj,allocator);
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