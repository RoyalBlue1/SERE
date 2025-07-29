#include "Nodes/MathNodes.h"


MultiplyNode::MultiplyNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this,styles.pinStyles);
	}
	getOut<FloatVariable>("Res")->behaviour([this]() {

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

std::vector<std::shared_ptr<ImFlow::PinProto>> MultiplyNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B",ImFlow::ConnectionFilter::SameType(),FloatVariable(2.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

AdditionNode::AdditionNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this,styles.pinStyles);
	}
	getOut<FloatVariable>("Res")->behaviour([this]() {
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

std::vector<std::shared_ptr<ImFlow::PinProto>> AdditionNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}


SubtractNode::SubtractNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this,styles.pinStyles);
	}
	getOut<FloatVariable>("Res")->behaviour([this]() {
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

std::vector<std::shared_ptr<ImFlow::PinProto>> SubtractNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

DivideNode::DivideNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	style = styles.GetNodeStyle(category);
	errorStyle = styles.errorNode;
	setStyle(style);
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this,styles.pinStyles);
	}
	getOut<FloatVariable>("Res")->behaviour([this]() {
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

std::vector<std::shared_ptr<ImFlow::PinProto>> DivideNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B",ImFlow::ConnectionFilter::SameType(),FloatVariable(2.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

ModuloNode::ModuloNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	style = styles.GetNodeStyle(category);
	errorStyle = styles.errorNode;
	setStyle(style);
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this,styles.pinStyles);
	}
	getOut<FloatVariable>("Res")->behaviour([this]() {

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

std::vector<std::shared_ptr<ImFlow::PinProto>> ModuloNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B",ImFlow::ConnectionFilter::SameType(),FloatVariable(1.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}


AbsoluteNode::AbsoluteNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this,styles.pinStyles);
	}
	getOut<FloatVariable>("Res")->behaviour([this]() {
		const FloatVariable& a = getInVal<FloatVariable>("A");
		return FloatVariable(abs(a.value),a.isConstant);
	});

}

void AbsoluteNode::draw() {
	const FloatVariable& a = getInVal<FloatVariable>("A");

	ImGui::Text("A %f",a.value);
	ImGui::Text("Res %f", abs(a.value));

}


std::vector<std::shared_ptr<ImFlow::PinProto>> AbsoluteNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

SineNode::SineNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this,styles.pinStyles);
	}
	getOut<FloatVariable>("Res")->behaviour([this]() {

		const FloatVariable& a = getInVal<FloatVariable>("A");
		return FloatVariable(sin(a.value),a.isConstant);

	});

}

void SineNode::draw() {
	const FloatVariable& a = getInVal<FloatVariable>("A");

	ImGui::Text("A %f",a.value);
	ImGui::Text("Res %f", sin(a.value));

}

std::vector<std::shared_ptr<ImFlow::PinProto>> SineNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

ExponentNode::ExponentNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this,styles.pinStyles);
	}
	getOut<FloatVariable>("Res")->behaviour([this]() {
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


std::vector<std::shared_ptr<ImFlow::PinProto>> ExponentNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

MappingNode::MappingNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this,styles.pinStyles);
	}
	getOut<FloatVariable>("Res")->behaviour([this]() {
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


std::vector<std::shared_ptr<ImFlow::PinProto>> MappingNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
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