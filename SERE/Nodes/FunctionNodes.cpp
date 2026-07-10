#include "FunctionNodes.h"

SetNoRenderNode::SetNoRenderNode(RenderInstance& rend, ImFlow::StyleManager& style)
	: RuiBaseNode(name, category, GetPinInfo(), rend, style)
{
}

SetNoRenderNode::SetNoRenderNode(RenderInstance& rend, ImFlow::StyleManager& style,
	rapidjson::GenericObject<false, rapidjson::Value> obj) : SetNoRenderNode(rend, style)
{
}

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
			"if ({}) {{\n\tsetNoRender(inst);\n\treturn;\n}}",
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

void AddFunctionNodes(NodeEditor& editor)
{
	editor.AddNodeType<SetNoRenderNode>();
}
