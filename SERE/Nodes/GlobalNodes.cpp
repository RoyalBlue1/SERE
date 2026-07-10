#include "GlobalNodes.h"


TimeNode::TimeNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Time")->behaviour([this, outName]() {
		return FloatVariable(render.globals.currentTime, outName);
		});

}

TimeNode::TimeNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :TimeNode(rend, style) {


}

void TimeNode::draw() {


	ImGui::Text("Time %f", render.globals.currentTime);


}

void TimeNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void TimeNode::Export(RuiExportPrototype& proto) {
	auto out = getOut<FloatVariable>("Time")->val();
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.identifier = out.name;
	ele.callback = [out](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = globals->currentTime;", out.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = globals->currentTime;", out.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> TimeNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Time"));
	return info;
}

ADSFracNode::ADSFracNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();

	getOut<FloatVariable>("AdsFrac")->behaviour([this, outName]() {
		return FloatVariable(render.globals.adsFracValue, outName);
		});

}

ADSFracNode::ADSFracNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :ADSFracNode(rend, style) {


}

void ADSFracNode::draw() {


	ImGui::PushItemWidth(100);
	ImGui::SliderFloat("ADS Fraction", &render.globals.adsFracValue, 0, 1);
	ImGui::PopItemWidth();


}

void ADSFracNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void ADSFracNode::Export(RuiExportPrototype& proto) {
	auto out = getOut<FloatVariable>("AdsFrac")->val();
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.identifier = out.name;
	ele.callback = [out](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = globals->globalAdsFrac;", out.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = globals->globalAdsFrac;", out.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ADSFracNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("AdsFrac"));
	return info;
}


LocalPlayerPosNode::LocalPlayerPosNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();

	minVal = 0;
	maxVal = 1;
	render.globals.localPlayerPos[0] = 0;
	render.globals.localPlayerPos[1] = 0;
	render.globals.localPlayerPos[2] = 0;

	getOut<Float3Variable>("LocalPlayerPos")->behaviour([this, outName]() {
		return Float3Variable(render.globals.localPlayerPos[0], render.globals.localPlayerPos[1], render.globals.localPlayerPos[2], outName);
		});

}

LocalPlayerPosNode::LocalPlayerPosNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :LocalPlayerPosNode(rend, style) {


}

void LocalPlayerPosNode::draw() {


	ImGui::PushItemWidth(90);
	ImGui::InputFloat("Min", &minVal);
	ImGui::SameLine();
	ImGui::InputFloat("Max", &maxVal);
	ImGui::PopItemWidth();
	ImGui::PushItemWidth(240);
	ImGui::SliderFloat3("Value", render.globals.localPlayerPos, minVal, maxVal);
	ImGui::PopItemWidth();


}

void LocalPlayerPosNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void LocalPlayerPosNode::Export(RuiExportPrototype& proto) {
	auto out = getOut<Float3Variable>("LocalPlayerPos")->val();
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.identifier = out.name;
	ele.callback = [out](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = globals->localPlayerPos;", out.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("Vector3 {} = globals->localPlayerPos;", out.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> LocalPlayerPosNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float3Variable>>("LocalPlayerPos"));
	return info;
}




ScreenWidthNode::ScreenWidthNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();

	minVal = 0;
	maxVal = 1;
	render.globals.screenWidth = 0;


	getOut<FloatVariable>("ScreenWidth")->behaviour([this, outName]() {
		return FloatVariable(render.globals.screenWidth, outName);
		});

}

ScreenWidthNode::ScreenWidthNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :ScreenWidthNode(rend, style) {


}

void ScreenWidthNode::draw() {


	ImGui::PushItemWidth(90);
	ImGui::InputFloat("Min", &minVal);
	ImGui::SameLine();
	ImGui::InputFloat("Max", &maxVal);
	ImGui::PopItemWidth();
	ImGui::PushItemWidth(100);
	ImGui::SliderFloat("Screen Width", &render.globals.screenWidth, minVal, maxVal);
	ImGui::PopItemWidth();


}

void ScreenWidthNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void ScreenWidthNode::Export(RuiExportPrototype& proto) {
	auto out = getOut<FloatVariable>("ScreenWidth")->val();
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.identifier = out.name;
	ele.callback = [out](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = globals->screenWidth;", out.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = globals->screenWidth;", out.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ScreenWidthNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("ScreenWidth"));
	return info;
}


///

ScreenHeightNode::ScreenHeightNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();

	minVal = 0;
	maxVal = 1;
	render.globals.screenHeight = 0;


	getOut<FloatVariable>("ScreenHeight")->behaviour([this, outName]() {
		return FloatVariable(render.globals.screenHeight, outName);
		});

}

ScreenHeightNode::ScreenHeightNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :ScreenHeightNode(rend, style) {


}

void ScreenHeightNode::draw() {


	ImGui::PushItemWidth(90);
	ImGui::InputFloat("Min", &minVal);
	ImGui::SameLine();
	ImGui::InputFloat("Max", &maxVal);
	ImGui::PopItemWidth();
	ImGui::PushItemWidth(100);
	ImGui::SliderFloat("Screen Height", &render.globals.screenHeight, minVal, maxVal);
	ImGui::PopItemWidth();


}

void ScreenHeightNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void ScreenHeightNode::Export(RuiExportPrototype& proto) {
	auto out = getOut<FloatVariable>("ScreenHeight")->val();
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.identifier = out.name;
	ele.callback = [out](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = globals->screenHeight;", out.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = globals->screenHeight;", out.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ScreenHeightNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("ScreenHeight"));
	return info;
}




BoolGlobalNode::BoolGlobalNode(const std::string& nodeName, int Globals::* localField, const std::string& exportField,
	RenderInstance& rend, ImFlow::StyleManager& style)
	: RuiBaseNode(nodeName, "Globals", GetPinInfo(), rend, style),
	nodeName(nodeName), exportField(exportField), localField(localField)
{
	std::string outName = Variable::UniqueName();
	getOut<BoolVariable>("Value")->behaviour([this, outName]() {
		return BoolVariable(render.globals.*this->localField != 0, outName);
		});
}

void BoolGlobalNode::draw()
{
	bool value = render.globals.*localField != 0;
	if (ImGui::Checkbox("Value", &value))
		render.globals.*localField = value;
}

void BoolGlobalNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator)
{
	obj.AddMember("Name", nodeName, allocator);
	obj.AddMember("Category", "Globals", allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void BoolGlobalNode::Export(RuiExportPrototype& proto)
{
	auto out = getOut<BoolVariable>("Value")->val();
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.identifier = out.name;
	ele.callback = [out, exportField = exportField](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = globals->{};", out.GetFormattedName(proto), exportField));
		else
			proto.codeLines.push_back(std::format("bool {} = globals->{};", out.GetFormattedName(proto), exportField));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> BoolGlobalNode::GetPinInfo()
{
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<BoolVariable>>("Value"));
	return info;
}

IsKillReplayNode::IsKillReplayNode(RenderInstance& rend, ImFlow::StyleManager& style)
	: BoolGlobalNode(name, &Globals::isKillReplay, "isKillReplay", rend, style)
{}

IsKillReplayNode::IsKillReplayNode(RenderInstance& rend, ImFlow::StyleManager& style,
	rapidjson::GenericObject<false, rapidjson::Value> obj) : IsKillReplayNode(rend, style)
{}

IsUsingControllerNode::IsUsingControllerNode(RenderInstance& rend, ImFlow::StyleManager& style)
	: BoolGlobalNode(name, &Globals::isUsingController, "isUsingController", rend, style)
{}

IsUsingControllerNode::IsUsingControllerNode(RenderInstance& rend, ImFlow::StyleManager& style,
	rapidjson::GenericObject<false, rapidjson::Value> obj) : IsUsingControllerNode(rend, style)
{}

IsAliveNode::IsAliveNode(RenderInstance& rend, ImFlow::StyleManager& style)
	: BoolGlobalNode(name, &Globals::isAlive, "isAlive", rend, style)
{}

IsAliveNode::IsAliveNode(RenderInstance& rend, ImFlow::StyleManager& style,
	rapidjson::GenericObject<false, rapidjson::Value> obj) : IsAliveNode(rend, style)
{}

IsSpectatingNode::IsSpectatingNode(RenderInstance& rend, ImFlow::StyleManager& style)
	: BoolGlobalNode(name, &Globals::isSpectating, "isSpectating", rend, style)
{}

IsSpectatingNode::IsSpectatingNode(RenderInstance& rend, ImFlow::StyleManager& style,
	rapidjson::GenericObject<false, rapidjson::Value> obj) : IsSpectatingNode(rend, style)
{}

IsMenuOpenNode::IsMenuOpenNode(RenderInstance& rend, ImFlow::StyleManager& style)
	: BoolGlobalNode(name, &Globals::isMenuOpen, "isMenuOpen", rend, style)
{}

IsMenuOpenNode::IsMenuOpenNode(RenderInstance& rend, ImFlow::StyleManager& style,
	rapidjson::GenericObject<false, rapidjson::Value> obj) : IsMenuOpenNode(rend, style)
{}

IsPhaseShiftedNode::IsPhaseShiftedNode(RenderInstance& rend, ImFlow::StyleManager& style)
	: BoolGlobalNode(name, &Globals::isPhaseShifted, "isPhaseShifted", rend, style)
{}

IsPhaseShiftedNode::IsPhaseShiftedNode(RenderInstance& rend, ImFlow::StyleManager& style,
	rapidjson::GenericObject<false, rapidjson::Value> obj) : IsPhaseShiftedNode(rend, style)
{}

ColorGlobalNode::ColorGlobalNode(const std::string& nodeName, float (Globals::* localField)[3],
	const std::string& exportField, RenderInstance& rend, ImFlow::StyleManager& style)
	: RuiBaseNode(nodeName, "Globals", GetPinInfo(), rend, style),
	nodeName(nodeName), exportField(exportField), localField(localField)
{
	std::string outName = Variable::UniqueName();
	getOut<ColorVariable>("Value")->behaviour([this, outName]() {
		const float* color = render.globals.*this->localField;
		return ColorVariable(color[0], color[1], color[2], 1.0f, outName);
		});
}

void ColorGlobalNode::draw()
{
	const float* color = render.globals.*localField;
	ImGui::ColorButton("##ColorPreview", ImVec4(color[0], color[1], color[2], 1.0f),
		ImGuiColorEditFlags_NoDragDrop, ImVec2(48.0f, 48.0f));
	ImGui::SameLine();
	ImGui::Text("R: %.3f\nG: %.3f\nB: %.3f", color[0], color[1], color[2]);
}

void ColorGlobalNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,
	rapidjson::Document::AllocatorType& allocator)
{
	obj.AddMember("Name", nodeName, allocator);
	obj.AddMember("Category", "Globals", allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void ColorGlobalNode::Export(RuiExportPrototype& proto)
{
	auto out = getOut<ColorVariable>("Value")->val();
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.identifier = out.name;
	ele.callback = [out, exportField = exportField](RuiExportPrototype& proto) {
		const std::string value = std::format(
			"Color(globals->{0}.x,globals->{0}.y,globals->{0}.z,1.0f)", exportField);
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = {};", out.GetFormattedName(proto), value));
		else
			proto.codeLines.push_back(std::format("Color {} = {};", out.GetFormattedName(proto), value));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ColorGlobalNode::GetPinInfo()
{
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<ColorVariable>>("Value"));
	return info;
}

FriendlyTeamColorNode::FriendlyTeamColorNode(RenderInstance& rend, ImFlow::StyleManager& style)
	: ColorGlobalNode(name, &Globals::friendlyTeamColor, "friendlyTeamColor", rend, style)
{}

FriendlyTeamColorNode::FriendlyTeamColorNode(RenderInstance& rend, ImFlow::StyleManager& style,
	rapidjson::GenericObject<false, rapidjson::Value> obj) : FriendlyTeamColorNode(rend, style)
{}

EnemyTeamColorNode::EnemyTeamColorNode(RenderInstance& rend, ImFlow::StyleManager& style)
	: ColorGlobalNode(name, &Globals::enemyTeamColor, "enemyTeamColor", rend, style)
{}

EnemyTeamColorNode::EnemyTeamColorNode(RenderInstance& rend, ImFlow::StyleManager& style,
	rapidjson::GenericObject<false, rapidjson::Value> obj) : EnemyTeamColorNode(rend, style)
{}

PartyTeamColorNode::PartyTeamColorNode(RenderInstance& rend, ImFlow::StyleManager& style)
	: ColorGlobalNode(name, &Globals::partyTeamColor, "partyTeamColor", rend, style)
{}

PartyTeamColorNode::PartyTeamColorNode(RenderInstance& rend, ImFlow::StyleManager& style,
	rapidjson::GenericObject<false, rapidjson::Value> obj) : PartyTeamColorNode(rend, style)
{}

void AddGlobalNodes(NodeEditor& editor) {
	editor.AddNodeType<TimeNode>();
	editor.AddNodeType<ScreenWidthNode>();
	editor.AddNodeType<ScreenHeightNode>();
	editor.AddNodeType<ADSFracNode>();
	editor.AddNodeType<LocalPlayerPosNode>();
	editor.AddNodeType<IsKillReplayNode>();
	editor.AddNodeType<IsUsingControllerNode>();
	editor.AddNodeType<IsAliveNode>();
	editor.AddNodeType<IsSpectatingNode>();
	editor.AddNodeType<IsMenuOpenNode>();
	editor.AddNodeType<IsPhaseShiftedNode>();
	editor.AddNodeType<FriendlyTeamColorNode>();
	editor.AddNodeType<EnemyTeamColorNode>();
	editor.AddNodeType<PartyTeamColorNode>();
}
