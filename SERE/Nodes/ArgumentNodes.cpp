#include "ArgumentNodes.h"
#include "imgui/imgui_stdlib.h"
#include "Imgui/ImNodeFlow.h"
#include "CustomImGuiWidgets.h"



IntArgNode::IntArgNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<IntVariable>("Value")->behaviour([this]() {
		int val = std::any_cast<int>(render.arguments[argName]);
		return IntVariable(val,argName);

	});
}

IntArgNode::IntArgNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):IntArgNode(rend,style) {

	if(obj.HasMember("ArgName")&&obj["ArgName"].IsString())
		argName = obj["ArgName"].GetString();

}


void IntArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	int val = std::any_cast<int>(render.arguments[argName]);
	ImGui::InputInt("Default Value",&val);
	render.arguments[argName] = val;
	ImGui::PopItemWidth();
}

void IntArgNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("ArgName",argName,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void IntArgNode::Export(RuiExportPrototype& proto) {
	proto.arguments.emplace(argName,VariableType::INT);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> IntArgNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<IntVariable>>("Value"));
	return info;
}

BoolArgNode::BoolArgNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<BoolVariable>("Value")->behaviour([this]() {
		int val = std::any_cast<int>(render.arguments[argName]);
		return BoolVariable(val,argName);
	});
}

BoolArgNode::BoolArgNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):BoolArgNode(rend,style) {
	
	if(obj.HasMember("ArgName")&&obj["ArgName"].IsString())
		argName = obj["ArgName"].GetString();

}


void BoolArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	int val = std::any_cast<int>(render.arguments[argName]);
	ImGui::InputInt("Default Value",&val);
	render.arguments[argName] = val;
	ImGui::PopItemWidth();
}

void BoolArgNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("ArgName",argName,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void BoolArgNode::Export(RuiExportPrototype& proto) {
	proto.arguments.emplace(argName,VariableType::BOOL);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> BoolArgNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<BoolVariable>>("Value"));
	return info;
}

FloatArgNode::FloatArgNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<FloatVariable>("Value")->behaviour([this]() {
		float val = std::any_cast<float>(render.arguments[argName]);
		return FloatVariable(val,argName);
	});
}

FloatArgNode::FloatArgNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):FloatArgNode(rend,style) {

	if(obj.HasMember("ArgName")&&obj["ArgName"].IsString())
		argName = obj["ArgName"].GetString();

}

void FloatArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	float val = std::any_cast<float>(render.arguments[argName]);
	ImGui::InputFloat("Default Value",&val);
	render.arguments[argName] = val;
	ImGui::PopItemWidth();
}

void FloatArgNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("ArgName",argName,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void FloatArgNode::Export(RuiExportPrototype& proto) {
	proto.arguments.emplace(argName,VariableType::FLOAT);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> FloatArgNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Value"));
	return info;
}

Float2ArgNode::Float2ArgNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<Float2Variable>("Value")->behaviour([this]() {
		Vector2 val = std::any_cast<Vector2>(render.arguments[argName]);
		return Float2Variable(
			val,
			argName);

	});
}

Float2ArgNode::Float2ArgNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):Float2ArgNode(rend,style) {

	if(obj.HasMember("ArgName")&&obj["ArgName"].IsString())
		argName = obj["ArgName"].GetString();

}

void Float2ArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	Vector2 val = std::any_cast<Vector2>(render.arguments[argName]);
	ImGui::InputFloat2("Default Value",&val.x);
	render.arguments[argName] = val;
	ImGui::PopItemWidth();
}

void Float2ArgNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("ArgName",argName,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void Float2ArgNode::Export(RuiExportPrototype& proto) {
	proto.arguments.emplace(argName,VariableType::FLOAT2);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> Float2ArgNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float2Variable>>("Value"));
	return info;
}

Float3ArgNode::Float3ArgNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<Float3Variable>("Value")->behaviour([this]() {
		Vector3 val = std::any_cast<Vector3>(render.arguments[argName]);
		return Float3Variable(
			val,
			argName);

	});
}

Float3ArgNode::Float3ArgNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):Float3ArgNode(rend,style) {

	if(obj.HasMember("ArgName")&&obj["ArgName"].IsString())
		argName = obj["ArgName"].GetString();

}

void Float3ArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	Vector3 val = std::any_cast<Vector3>(render.arguments[argName]);
	ImGui::InputFloat3("Default Value",&val.x);
	render.arguments[argName] = val;
	ImGui::PopItemWidth();
}

void Float3ArgNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("ArgName",argName,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void Float3ArgNode::Export(RuiExportPrototype& proto) {
	proto.arguments.emplace(argName,VariableType::FLOAT3);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> Float3ArgNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float3Variable>>("Value"));
	return info;
}

ColorArgNode::ColorArgNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<ColorVariable>("Value")->behaviour([this]() {
		Color val = std::any_cast<Color>(render.arguments[argName]);
		return ColorVariable(
			val,
			argName);
	});
}

ColorArgNode::ColorArgNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):ColorArgNode(rend,style) {

	if(obj.HasMember("ArgName")&&obj["ArgName"].IsString())
		argName = obj["ArgName"].GetString();

}

void ColorArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	Color val = std::any_cast<Color>(render.arguments[argName]);
	ImGui::ColorPicker4("Default Value",&val.red);
	render.arguments[argName] = val;
	ImGui::PopItemWidth();
}

void ColorArgNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("ArgName",argName,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void ColorArgNode::Export(RuiExportPrototype& proto) {
	proto.arguments.emplace(argName,VariableType::COLOR_ALPHA);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ColorArgNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<ColorVariable>>("Value"));
	return info;
}

StringArgNode::StringArgNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<StringVariable>("Value")->behaviour([this]() {
		std::string val = std::any_cast<std::string>(render.arguments[argName]);
		return StringVariable(val,argName);

	});
}

StringArgNode::StringArgNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):StringArgNode(rend,style) {

	if(obj.HasMember("ArgName")&&obj["ArgName"].IsString())
		argName = obj["ArgName"].GetString();
}

void StringArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	std::string val = std::any_cast<std::string>(render.arguments[argName]);
	ImGui::InputText("Default Value",&val);
	render.arguments[argName] = val;
	ImGui::PopItemWidth();
}

void StringArgNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("ArgName",argName,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void StringArgNode::Export(RuiExportPrototype& proto) {
	proto.arguments.emplace(argName,VariableType::STRING);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> StringArgNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<StringVariable>>("Value"));
	return info;
}

AssetArgNode::AssetArgNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<AssetVariable>("Value")->behaviour([this]() {
		std::string val = std::any_cast<std::string>(render.arguments[argName]);
		return AssetVariable(val,Variable::UniqueName());
	});
}

void AssetArgNode::Export(RuiExportPrototype& proto) {
	proto.arguments.emplace(argName,VariableType::ASSET);
	auto& out = getOut<AssetVariable>("Value")->val();
	ExportElement<std::string> ele;
	ele.dependencys = { argName };
	ele.identifier = out.name;
	ele.callback =[this](RuiExportPrototype& proto) {
		auto& out = getOut<AssetVariable>("Value")->val();
		proto.codeLines.push_back(std::format("{} = funcs->LoadAsset(data->{})",out.GetFormattedName(proto),argName));
	};
	proto.codeElements.push_back(ele);
}

AssetArgNode::AssetArgNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):AssetArgNode(rend,style){

	if(obj.HasMember("ArgName")&&obj["ArgName"].IsString())
		argName = obj["ArgName"].GetString();

}

void AssetArgNode::draw() {
	ImGui::PushItemWidth(90);
	ImGui::InputText("Name",&argName);
	std::string val = "white";
	if(render.arguments.contains(argName)&&(render.arguments[argName].type() == typeid(std::string)))
		val = std::any_cast<std::string>(render.arguments[argName]);
	uint32_t hash = loadAsset(val.c_str());
	if (AtlasImageButton("AssetButton", imageAssetMap[hash])) {
		ImGui::OpenPopup("Asset Selection");
	}
	AssetSelectionPopup("Default Value",&hash);
	val = imageAssetMap[hash].name;
	render.arguments[argName] = val;
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