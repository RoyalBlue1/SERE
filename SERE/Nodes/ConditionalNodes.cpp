

#include "Nodes/ConditionalNodes.h"

GreaterNode::GreaterNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<BoolVariable>("Res")->behaviour([this, outName]() {

		const FloatVariable& a = getInVal<FloatVariable>("A");
		const FloatVariable& b = getInVal<FloatVariable>("B");
		std::string name = (a.IsConstant() && b.IsConstant()) ? "" : outName;

		return BoolVariable(a.value > b.value, name);

		});

}

GreaterNode::GreaterNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :GreaterNode(rend, style) {}


void GreaterNode::draw() {
	const FloatVariable& a = getInVal<FloatVariable>("A");
	const FloatVariable& b = getInVal<FloatVariable>("B");
	ImGui::Text("A %f", a.value);
	ImGui::Text("B %f", b.value);
	if (a.value > b.value)
		ImGui::Text("Res True");
	else
		ImGui::Text("Res False");
}

void GreaterNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void GreaterNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<BoolVariable>("Res")->val();
	const auto& a = getInVal<FloatVariable>("A");
	const auto& b = getInVal<FloatVariable>("B");
	ExportElement<std::string> ele;
	ele.dependencys = { a.name,b.name };
	ele.identifier = out.name;
	ele.callback = [out, a, b](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = {} > {};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("bool {} = {} > {};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> GreaterNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A", ImFlow::ConnectionFilter::SameType(), FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B", ImFlow::ConnectionFilter::SameType(), FloatVariable(2.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<BoolVariable>>("Res"));
	return info;
}

LessNode::LessNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<BoolVariable>("Res")->behaviour([this, outName]() {

		const FloatVariable& a = getInVal<FloatVariable>("A");
		const FloatVariable& b = getInVal<FloatVariable>("B");
		std::string name = (a.IsConstant() && b.IsConstant()) ? "" : outName;

		return BoolVariable(a.value < b.value, name);

		});

}

LessNode::LessNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :LessNode(rend, style) {}


void LessNode::draw() {
	const FloatVariable& a = getInVal<FloatVariable>("A");
	const FloatVariable& b = getInVal<FloatVariable>("B");
	ImGui::Text("A %f", a.value);
	ImGui::Text("B %f", b.value);
	if (a.value < b.value)
		ImGui::Text("Res True");
	else
		ImGui::Text("Res False");
}

void LessNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void LessNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<BoolVariable>("Res")->val();
	const auto& a = getInVal<FloatVariable>("A");
	const auto& b = getInVal<FloatVariable>("B");
	ExportElement<std::string> ele;
	ele.dependencys = { a.name,b.name };
	ele.identifier = out.name;
	ele.callback = [out, a, b](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = {} < {};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("bool {} = {} < {};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> LessNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A", ImFlow::ConnectionFilter::SameType(), FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B", ImFlow::ConnectionFilter::SameType(), FloatVariable(2.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<BoolVariable>>("Res"));
	return info;
}

ConditionalFloatNode::ConditionalFloatNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this, outName]() {

		const BoolVariable& a = getInVal<BoolVariable>("A");
		const FloatVariable& b = getInVal<FloatVariable>("B");
		const FloatVariable& c = getInVal<FloatVariable>("C");

		std::string name = (a.IsConstant() && b.IsConstant() && c.IsConstant()) ? "" : outName;

		if (a.value)
			return FloatVariable(b.value, name);
		else
			return FloatVariable(c.value, name);

		});

}

ConditionalFloatNode::ConditionalFloatNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :ConditionalFloatNode(rend, style) {}


void ConditionalFloatNode::draw() {
	const BoolVariable& a = getInVal<BoolVariable>("A");

	const FloatVariable& b = getInVal<FloatVariable>("B");
	const FloatVariable& c = getInVal<FloatVariable>("C");

	if (a.value)
		ImGui::Text("In True");
	else
		ImGui::Text("In False");
	ImGui::Text("True %f", b.value);
	ImGui::Text("False %f", c.value);

}

void ConditionalFloatNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void ConditionalFloatNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<FloatVariable>("Res")->val();
	const auto& a = getInVal<BoolVariable>("A");
	const auto& b = getInVal<FloatVariable>("B");
	const auto& c = getInVal<FloatVariable>("C");

	ExportElement<std::string> ele;
	ele.dependencys = { a.name,b.name, c.name };
	ele.identifier = out.name;
	ele.callback = [out, a, b, c](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
		{
			proto.codeLines.push_back(std::format("{} = {}?{}:{};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto), c.GetFormattedName(proto)));

		}
		else
			proto.codeLines.push_back(std::format("float {} = {}?{}:{};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto), c.GetFormattedName(proto)));

		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ConditionalFloatNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<BoolVariable>>("A", ImFlow::ConnectionFilter::SameType(), BoolVariable(false)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B", ImFlow::ConnectionFilter::SameType(), FloatVariable(1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("C", ImFlow::ConnectionFilter::SameType(), FloatVariable(2.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

EqualFloatNode::EqualFloatNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<BoolVariable>("Res")->behaviour([this, outName]() {

		const FloatVariable& a = getInVal<FloatVariable>("A");
		const FloatVariable& b = getInVal<FloatVariable>("B");
		std::string name = (a.IsConstant() && b.IsConstant()) ? "" : outName;

		return BoolVariable(a.value == b.value, name);

		});

}

EqualFloatNode::EqualFloatNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :EqualFloatNode(rend, style) {}


void EqualFloatNode::draw() {
	const FloatVariable& a = getInVal<FloatVariable>("A");
	const FloatVariable& b = getInVal<FloatVariable>("B");
	ImGui::Text("A %f", a.value);
	ImGui::Text("B %f", b.value);
	if (a.value == b.value)
		ImGui::Text("Res True");
	else
		ImGui::Text("Res False");
}

void EqualFloatNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void EqualFloatNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<BoolVariable>("Res")->val();
	const auto& a = getInVal<FloatVariable>("A");
	const auto& b = getInVal<FloatVariable>("B");
	ExportElement<std::string> ele;
	ele.dependencys = { a.name,b.name };
	ele.identifier = out.name;
	ele.callback = [out, a, b](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = {} == {};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("bool {} = {} == {};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> EqualFloatNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A", ImFlow::ConnectionFilter::SameType(), FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B", ImFlow::ConnectionFilter::SameType(), FloatVariable(2.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<BoolVariable>>("Res"));
	return info;
}



NotGateNode::NotGateNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<BoolVariable>("Res")->behaviour([this, outName]() {

		const BoolVariable& a = getInVal<BoolVariable>("A");
		std::string name = (a.IsConstant()) ? "" : outName;

		return BoolVariable(!a.value, name);

		});

}

NotGateNode::NotGateNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :NotGateNode(rend, style) {}


void NotGateNode::draw() {
	const BoolVariable& a = getInVal<BoolVariable>("A");

	if (!a.value)
	{
		ImGui::Text("A False");
		ImGui::Text("Res True");
	}
	else
	{
		ImGui::Text("A True");
		ImGui::Text("Res False");
	}
}
void NotGateNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void NotGateNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<BoolVariable>("Res")->val();
	const auto& a = getInVal<BoolVariable>("A");
	ExportElement<std::string> ele;
	ele.dependencys = { a.name };
	ele.identifier = out.name;
	ele.callback = [out, a](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = !{};", out.GetFormattedName(proto), a.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("bool {} = !{};", out.GetFormattedName(proto), a.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> NotGateNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<BoolVariable>>("A", ImFlow::ConnectionFilter::SameType(), BoolVariable(false)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<BoolVariable>>("Res"));
	return info;
}

AndGateNode::AndGateNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<BoolVariable>("Res")->behaviour([this, outName]() {

		const BoolVariable& a = getInVal<BoolVariable>("A");
		const BoolVariable& b = getInVal<BoolVariable>("B");
		std::string name = (a.IsConstant() && b.IsConstant()) ? "" : outName;

		return BoolVariable(a.value == b.value, name);

		});

}

AndGateNode::AndGateNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :AndGateNode(rend, style) {}


void AndGateNode::draw() {
	const BoolVariable& a = getInVal<BoolVariable>("A");
	const BoolVariable& b = getInVal<BoolVariable>("B");
	ImGui::Text("A %s", a.value ? "True" : "False");
	ImGui::Text("A %s", b.value ? "True" : "False");

	if (a.value == b.value)
		ImGui::Text("Res True");
	else
		ImGui::Text("Res False");
}

void AndGateNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void AndGateNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<BoolVariable>("Res")->val();
	const auto& a = getInVal<BoolVariable>("A");
	const auto& b = getInVal<BoolVariable>("B");
	ExportElement<std::string> ele;
	ele.dependencys = { a.name,b.name };
	ele.identifier = out.name;
	ele.callback = [out, a, b](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = {} == {};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("bool {} = {} == {};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> AndGateNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<BoolVariable>>("A", ImFlow::ConnectionFilter::SameType(), BoolVariable(false)));
	info.push_back(std::make_shared<ImFlow::InPinProto<BoolVariable>>("B", ImFlow::ConnectionFilter::SameType(), BoolVariable(false)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<BoolVariable>>("Res"));
	return info;
}

OrGateNode::OrGateNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<BoolVariable>("Res")->behaviour([this, outName]() {

		const BoolVariable& a = getInVal<BoolVariable>("A");
		const BoolVariable& b = getInVal<BoolVariable>("B");
		std::string name = (a.IsConstant() && b.IsConstant()) ? "" : outName;

		return BoolVariable(a.value || b.value, name);

		});

}

OrGateNode::OrGateNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :OrGateNode(rend, style) {}


void OrGateNode::draw() {
	const BoolVariable& a = getInVal<BoolVariable>("A");
	const BoolVariable& b = getInVal<BoolVariable>("B");
	ImGui::Text("A %s", a.value ? "True" : "False");
	ImGui::Text("A %s", b.value ? "True" : "False");

	if (a.value || b.value)
		ImGui::Text("Res True");
	else
		ImGui::Text("Res False");
}

void OrGateNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void OrGateNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<BoolVariable>("Res")->val();
	const auto& a = getInVal<BoolVariable>("A");
	const auto& b = getInVal<BoolVariable>("B");
	ExportElement<std::string> ele;
	ele.dependencys = { a.name,b.name };
	ele.identifier = out.name;
	ele.callback = [out, a, b](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = {} || {};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("bool {} = {} || {};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> OrGateNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<BoolVariable>>("A", ImFlow::ConnectionFilter::SameType(), BoolVariable(false)));
	info.push_back(std::make_shared<ImFlow::InPinProto<BoolVariable>>("B", ImFlow::ConnectionFilter::SameType(), BoolVariable(false)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<BoolVariable>>("Res"));
	return info;
}

EqualStringNode::EqualStringNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<BoolVariable>("Res")->behaviour([this, outName]() {

		const StringVariable& a = getInVal<StringVariable>("A");
		const StringVariable& b = getInVal<StringVariable>("B");
		std::string name = (a.IsConstant() && b.IsConstant()) ? "" : outName;

		return BoolVariable(a.value == b.value, name);

		});

}

EqualStringNode::EqualStringNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :EqualStringNode(rend, style) {}


void EqualStringNode::draw() {
	const StringVariable& a = getInVal<StringVariable>("A");
	const StringVariable& b = getInVal<StringVariable>("B");
	ImGui::Text("A %s", a.value.c_str());
	ImGui::Text("B %s", b.value.c_str());
	if (a.value == b.value)
		ImGui::Text("Res True");
	else
		ImGui::Text("Res False");
}

void EqualStringNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void EqualStringNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<BoolVariable>("Res")->val();
	const auto& a = getInVal<StringVariable>("A");
	const auto& b = getInVal<StringVariable>("B");
	ExportElement<std::string> ele;
	ele.dependencys = { a.name,b.name };
	ele.identifier = out.name;
	ele.callback = [out, a, b](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} !strcmp({}, {});", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("bool {} = !strcmp({}, {});", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> EqualStringNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<StringVariable>>("A", ImFlow::ConnectionFilter::SameType(), StringVariable("Default String")));
	info.push_back(std::make_shared<ImFlow::InPinProto<StringVariable>>("B", ImFlow::ConnectionFilter::SameType(), StringVariable("Default String")));
	info.push_back(std::make_shared<ImFlow::OutPinProto<BoolVariable>>("Res"));
	return info;
}


ConditionalStringNode::ConditionalStringNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<StringVariable>("Res")->behaviour([this, outName]() {

		const BoolVariable& a = getInVal<BoolVariable>("A");
		const StringVariable& b = getInVal<StringVariable>("B");
		const StringVariable& c = getInVal<StringVariable>("C");

		std::string name = (a.IsConstant() && b.IsConstant() && c.IsConstant()) ? "" : outName;

		if (a.value)
			return StringVariable(b.value, name);
		else
			return StringVariable(c.value, name);

		});

}

ConditionalStringNode::ConditionalStringNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :ConditionalStringNode(rend, style) {}


void ConditionalStringNode::draw() {
	const BoolVariable& a = getInVal<BoolVariable>("A");

	const StringVariable& b = getInVal<StringVariable>("B");
	const StringVariable& c = getInVal<StringVariable>("C");

	if (a.value)
		ImGui::Text("In True");
	else
		ImGui::Text("In False");
	ImGui::Text("True %s", b.value.c_str());
	ImGui::Text("False %s", c.value.c_str());

}

void ConditionalStringNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void ConditionalStringNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<StringVariable>("Res")->val();
	const auto& a = getInVal<BoolVariable>("A");
	const auto& b = getInVal<StringVariable>("B");
	const auto& c = getInVal<StringVariable>("C");

	ExportElement<std::string> ele;
	ele.dependencys = { a.name,b.name, c.name };
	ele.identifier = out.name;
	ele.callback = [out, a, b, c](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
		{
			proto.codeLines.push_back(std::format("{} = {}?{}:{};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto), c.GetFormattedName(proto)));

		}
		else
			proto.codeLines.push_back(std::format("const char* {} = {}?{}:{};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto), c.GetFormattedName(proto)));

		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ConditionalStringNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<BoolVariable>>("A", ImFlow::ConnectionFilter::SameType(), BoolVariable(false)));
	info.push_back(std::make_shared<ImFlow::InPinProto<StringVariable>>("B", ImFlow::ConnectionFilter::SameType(), StringVariable("Default Text")));
	info.push_back(std::make_shared<ImFlow::InPinProto<StringVariable>>("C", ImFlow::ConnectionFilter::SameType(), StringVariable("Default Test")));
	info.push_back(std::make_shared<ImFlow::OutPinProto<StringVariable>>("Res"));
	return info;
}

void AddConditionalNodes(NodeEditor& editor) {

	editor.AddNodeType<GreaterNode>();
	editor.AddNodeType<LessNode>();
	editor.AddNodeType<ConditionalFloatNode>();
	editor.AddNodeType<EqualFloatNode>();
	editor.AddNodeType<AndGateNode>();
	editor.AddNodeType<NotGateNode>();
	editor.AddNodeType<OrGateNode>();
	editor.AddNodeType<EqualStringNode>();
	editor.AddNodeType<ConditionalStringNode>();

}
