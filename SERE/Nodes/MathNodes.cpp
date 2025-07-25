#include "Nodes/MathNodes.h"


MultiplyNode::MultiplyNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addIN<FloatVariable>("A",FloatVariable(0.f), ImFlow::ConnectionFilter::SameType(), styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("B",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addOUT<FloatVariable>("Res",styles.floatVariable)->behaviour([this]() {

		const FloatVariable& a = getInVal<FloatVariable>("A");
		const FloatVariable& b = getInVal<FloatVariable>("B");
		bool isConstant = a.isConstant&&b.isConstant;
		return FloatVariable(a.value * b.value,isConstant);

	});

}

void MultiplyNode::draw() {
	const FloatVariable& a = getInVal<FloatVariable>("A");
	const FloatVariable& b = getInVal<FloatVariable>("B");
	ImGui::Text("A %f",a.value);
	ImGui::Text("B %f",b.value);
	ImGui::Text("Res %f",a.value*b.value);

}

std::vector<PinInfo> MultiplyNode::GetPinInfo() {
	return {};
}

AdditionNode::AdditionNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addIN<FloatVariable>("A",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("B",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addOUT<FloatVariable>("Res",styles.floatVariable)->behaviour([this]() {
		const FloatVariable& a = getInVal<FloatVariable>("A");
		const FloatVariable& b = getInVal<FloatVariable>("B");
		bool isConstant = a.isConstant&&b.isConstant;
		return FloatVariable(a.value + b.value,isConstant);
		

	});

}

void AdditionNode::draw() {
	const FloatVariable& a = getInVal<FloatVariable>("A");
	const FloatVariable& b = getInVal<FloatVariable>("B");
	ImGui::Text("A %f",a.value);
	ImGui::Text("B %f",b.value);
	ImGui::Text("Res %f",a.value+b.value);

}

std::vector<PinInfo> AdditionNode::GetPinInfo() {
	return {};
}


SubtractNode::SubtractNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addIN<FloatVariable>("A",FloatVariable(0.f), ImFlow::ConnectionFilter::SameType(), styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("B",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addOUT<FloatVariable>("Res",styles.floatVariable)->behaviour([this]() {
		const FloatVariable& a = getInVal<FloatVariable>("A");
		const FloatVariable& b = getInVal<FloatVariable>("B");
		bool isConstant = a.isConstant&&b.isConstant;
		return FloatVariable(a.value - b.value,isConstant);
		

	});

}

void SubtractNode::draw() {
	const FloatVariable& a = getInVal<FloatVariable>("A");
	const FloatVariable& b = getInVal<FloatVariable>("B");
	ImGui::Text("A %f",a.value);
	ImGui::Text("B %f",b.value);
	ImGui::Text("Res %f",a.value-b.value);

}

std::vector<PinInfo> SubtractNode::GetPinInfo() {
	return {};
}

DivideNode::DivideNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	style = styles.GetNodeStyle(category);
	errorStyle = styles.errorNode;
	setStyle(style);
	ImFlow::BaseNode::addIN<FloatVariable>("A",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("B",FloatVariable(2.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addOUT<FloatVariable>("Res",styles.floatVariable)->behaviour([this]() {
		const FloatVariable& a = getInVal<FloatVariable>("A");
		const FloatVariable& b = getInVal<FloatVariable>("B");
		bool isConstant = a.isConstant&&b.isConstant;
		return FloatVariable((b.value!=0.0f)?(a.value/b.value):1.0,isConstant);

	});

}

void DivideNode::draw() {
	const FloatVariable& a = getInVal<FloatVariable>("A");
	const FloatVariable& b = getInVal<FloatVariable>("B");
	ImGui::Text("A %f",a.value);
	ImGui::Text("B %f",b.value);
	if(b.value!=0.f)
	{
		setStyle(style);
		ImGui::Text("Res %f", a.value / b.value);
	}
	else
	{
		setStyle(errorStyle);
		ImGui::Text("Divide by 0");
	}

}

std::vector<PinInfo> DivideNode::GetPinInfo() {
	return {};
}

ModuloNode::ModuloNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	style = styles.GetNodeStyle(category);
	errorStyle = styles.errorNode;
	setStyle(style);
	ImFlow::BaseNode::addIN<FloatVariable>("A",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("B",FloatVariable(1.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addOUT<FloatVariable>("Res",styles.floatVariable)->behaviour([this]() {

		const FloatVariable& a = getInVal<FloatVariable>("A");
		const FloatVariable& b = getInVal<FloatVariable>("B");
		bool isConstant = a.isConstant&&b.isConstant;
		return FloatVariable((b.value!=0.0f)?std::fmodf(a.value,b.value):1.0,isConstant);
		

	});

}

void ModuloNode::draw() {
	const FloatVariable& a = getInVal<FloatVariable>("A");
	const FloatVariable& b = getInVal<FloatVariable>("B");
	ImGui::Text("A %f",a.value);
	ImGui::Text("B %f",b.value);
	if(b.value!=0.f)
	{
		setStyle(style);
		ImGui::Text("Res %f", std::fmodf(a.value,b.value));
	}
	else
	{
		setStyle(errorStyle);
		ImGui::Text("Divide by 0");
	}

}

std::vector<PinInfo> ModuloNode::GetPinInfo() {
	return {};
}


AbsoluteNode::AbsoluteNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addIN<FloatVariable>("A",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);

	ImFlow::BaseNode::addOUT<FloatVariable>("Res",styles.floatVariable)->behaviour([this]() {
		const FloatVariable& a = getInVal<FloatVariable>("A");
		return FloatVariable(abs(a.value),a.isConstant);
	});

}

void AbsoluteNode::draw() {
	const FloatVariable& a = getInVal<FloatVariable>("A");

	ImGui::Text("A %f",a.value);
	ImGui::Text("Res %f", abs(a.value));

}


std::vector<PinInfo> AbsoluteNode::GetPinInfo() {
	return {};
}

SineNode::SineNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addIN<FloatVariable>("A",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);

	ImFlow::BaseNode::addOUT<FloatVariable>("Res",styles.floatVariable)->behaviour([this]() {

		const FloatVariable& a = getInVal<FloatVariable>("A");
		return FloatVariable(sin(a.value),a.isConstant);

	});

}

void SineNode::draw() {
	const FloatVariable& a = getInVal<FloatVariable>("A");

	ImGui::Text("A %f",a.value);
	ImGui::Text("Res %f", sin(a.value));

}

std::vector<PinInfo> SineNode::GetPinInfo() {
	return {};
}

ExponentNode::ExponentNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addIN<FloatVariable>("A",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("B",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addOUT<FloatVariable>("Res",styles.floatVariable)->behaviour([this]() {
		const FloatVariable& a = getInVal<FloatVariable>("A");
		const FloatVariable& b = getInVal<FloatVariable>("B");
		bool isConstant = a.isConstant&&b.isConstant;

		return FloatVariable(std::pow(a.value,b.value), isConstant);

	});

}

void ExponentNode::draw() {
	const FloatVariable& a = getInVal<FloatVariable>("A");
	const FloatVariable& b = getInVal<FloatVariable>("B");
	ImGui::Text("A %f",a.value);
	ImGui::Text("B %f",b.value);
	ImGui::Text("Res %f",std::pow(a.value, b.value));

}


std::vector<PinInfo> ExponentNode::GetPinInfo() {
	return {};
}

MappingNode::MappingNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addIN<FloatVariable>("A",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addOUT<FloatVariable>("Res",styles.floatVariable)->behaviour([this]() {
		const FloatVariable& a = getInVal<FloatVariable>("A");
		return FloatVariable(map.MapVar(a.value), true);
	});

}

void MappingNode::draw() {
	const FloatVariable& a = getInVal<FloatVariable>("A");
	ImGui::Text("A %f",a.value);
	ImGui::Text("Res %f",map.MapVar(a.value));
	if(ImGui::Button("Edit Mapping")) {
		ImGui::OpenPopup("Mapping Editor");
	}
	MappingCreationPopup("Mapping Editor",a.value,map);

}


std::vector<PinInfo> MappingNode::GetPinInfo() {
	return {};
}


void AddMathNodes(NodeEditor& editor) {

	editor.AddNodeType<MultiplyNode>();
	editor.AddNodeType<AdditionNode>();
	editor.AddNodeType<SubtractNode>();
	editor.AddNodeType<DivideNode>();
	editor.AddNodeType<ModuloNode>();
	editor.AddNodeType<AbsoluteNode>();
	editor.AddNodeType<SineNode>();
	editor.AddNodeType<ExponentNode>();
	editor.AddNodeType<MappingNode>();
}