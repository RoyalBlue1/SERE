#include "FunctionNodes.h"

#include <algorithm>
#include <cctype>

SetNoRenderNode::SetNoRenderNode(RenderInstance& rend, ImFlow::StyleManager& style)
	: RuiBaseNode(name, category, GetPinInfo(), rend, style)
{}

SetNoRenderNode::SetNoRenderNode(RenderInstance& rend, ImFlow::StyleManager& style,
	rapidjson::GenericObject<false, rapidjson::Value> obj) : SetNoRenderNode(rend, style)
{}

void SetNoRenderNode::draw()
{
	const BoolVariable& value = getInVal<BoolVariable>("Value");
	ImGui::Text("Value: %s", value.value ? "true" : "false");
}

void SetNoRenderNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,
	rapidjson::Document::AllocatorType& allocator)
{
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void SetNoRenderNode::Export(RuiExportPrototype& proto)
{
	const BoolVariable& value = getInVal<BoolVariable>("Value");
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.dependencys = { value.name };
	ele.identifier = Variable::UniqueName();
	ele.callback = [value](RuiExportPrototype& proto) {
		proto.codeLines.push_back(std::format(
			"if ({}) {{\n\tfuncs->setNoRender(inst);\n\treturn;\n}}",
			value.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> SetNoRenderNode::GetPinInfo()
{
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<BoolVariable>>(
		"Value", ImFlow::ConnectionFilter::SameType(), BoolVariable(false)));
	return info;
}


static float getRandomFloat(float min, float max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}

RandomFloatNode::RandomFloatNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	randomFloat = getRandomFloat(0.0f, 1.0f);
	getOut<FloatVariable>("Out")->behaviour([this, outName]() {
		return FloatVariable(randomFloat, outName);
		});
}

RandomFloatNode::RandomFloatNode(RenderInstance& prot, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj) : RandomFloatNode(prot, styles)
{

}

void RandomFloatNode::draw()
{
	ImGui::Text("Value: %f", randomFloat);
}

void RandomFloatNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator)
{
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void RandomFloatNode::Export(RuiExportPrototype& proto)
{
	auto out = getOut<FloatVariable>("Out")->val();
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.identifier = out.name;
	ele.callback = [out](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = randomFloat(inst);", out.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = randomFloat(inst);", out.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> RandomFloatNode::GetPinInfo()
{
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Out"));
	return info;
}


ProjectionNode::ProjectionNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<Float2Variable>("Res")->behaviour([this, outName]() {
		return Float2Variable(Vector2(0.5f, 0.5f), name);
		});

}

ProjectionNode::ProjectionNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :ProjectionNode(rend, style) {}

void ProjectionNode::draw() {


}

void ProjectionNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void ProjectionNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<Float2Variable>("Res")->val();
	const auto& pos = getInVal<Float3Variable>("Position");

	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.dependencys = { pos.name };
	ele.identifier = out.name;
	ele.callback = [out, pos](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = project3d(funcs,globals,inst,&{});", out.GetFormattedName(proto), pos.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = project3d(funcs,globals,inst,&{});", out.GetFormattedName(proto), pos.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ProjectionNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<Float3Variable>>("Position", ImFlow::ConnectionFilter::SameType(), Float3Variable(Vector3(0.f, 0.f, 0.f))));
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float2Variable>>("Res"));
	return info;
}

static std::string ToUpper(std::string value)
{
	std::transform(value.begin(), value.end(), value.begin(), [](unsigned char character) {
		return static_cast<char>(std::toupper(character));
		});
	return value;
}

ToUpperNode::ToUpperNode(RenderInstance& prot, ImFlow::StyleManager& styles)
	: RuiBaseNode(name, category, GetPinInfo(), prot, styles)
{
	std::string outName = Variable::UniqueName();
	getOut<StringVariable>("Out")->behaviour([this, outName]() {
		const StringVariable& input = getInVal<StringVariable>("In");
		return StringVariable(ToUpper(input.value), input.IsConstant() ? "" : outName);
		});
}

ToUpperNode::ToUpperNode(RenderInstance& prot, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj)
	: ToUpperNode(prot, styles)
{}

void ToUpperNode::draw()
{}

void ToUpperNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator)
{
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void ToUpperNode::Export(RuiExportPrototype& proto)
{
	const StringVariable& out = getOut<StringVariable>("Out")->val();
	const StringVariable& input = getInVal<StringVariable>("In");
	if (out.IsConstant())
		return;

	ExportElement<std::string> element;
#if _DEBUG
	element.sourceNodeName = typeid(*this).name();
#endif
	element.dependencys = { input.name };
	element.identifier = out.name;
	element.callback = [out, input](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = funcs->toUpper(inst, {});", out.GetFormattedName(proto), input.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("const char* {} = funcs->toUpper(inst, {});", out.GetFormattedName(proto), input.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(element);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ToUpperNode::GetPinInfo()
{
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<StringVariable>>(
		"In", ImFlow::ConnectionFilter::SameType(), StringVariable("Default Text")));
	info.push_back(std::make_shared<ImFlow::OutPinProto<StringVariable>>("Out"));
	return info;
}


void AddFunctionNodes(NodeEditor& editor)
{
	editor.AddNodeType<SetNoRenderNode>();
	editor.AddNodeType<RandomFloatNode>();
	editor.AddNodeType<ProjectionNode>();
	editor.AddNodeType<ToUpperNode>();

}