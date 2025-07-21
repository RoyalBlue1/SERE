#include "Nodes/MathNodes.h"


MultiplyNode::MultiplyNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Multiply");
	setStyle(styles.mathNode);
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

AdditionNode::AdditionNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Add");
	setStyle(styles.mathNode);
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


SubtractNode::SubtractNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Subtract");
	setStyle(styles.mathNode);
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

DivideNode::DivideNode(RenderInstance& prot,NodeStyles& style):proto(prot),styles(style) {

	setTitle("Divide");
	setStyle(styles.mathNode);
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
		setStyle(styles.mathNode);
		ImGui::Text("Res %f", a.value / b.value);
	}
	else
	{
		setStyle(styles.errorNode);
		ImGui::Text("Divide by 0");
	}

}

ModuloNode::ModuloNode(RenderInstance& prot,NodeStyles& style):proto(prot),styles(style) {

	setTitle("Modulo");
	setStyle(styles.mathNode);
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
		setStyle(styles.mathNode);
		ImGui::Text("Res %f", std::fmodf(a.value,b.value));
	}
	else
	{
		setStyle(styles.errorNode);
		ImGui::Text("Divide by 0");
	}

}


AbsoluteNode::AbsoluteNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Absolute");
	setStyle(styles.mathNode);
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

SineNode::SineNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Sine");
	setStyle(styles.mathNode);
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

ExponentNode::ExponentNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Exponent");
	setStyle(styles.mathNode);
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