#include "Nodes/MathNodes.h"


MultiplyNode::MultiplyNode(RenderInstance& rend,ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this,outName]() {

		const FloatVariable& a = getInVal<FloatVariable>("A");
		const FloatVariable& b = getInVal<FloatVariable>("B");
		std::string name = (a.IsConstant() && b.IsConstant())?"":outName;
		return FloatVariable(a.value * b.value,name);

	});

}

MultiplyNode::MultiplyNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):MultiplyNode(rend,style){}


void MultiplyNode::draw() {
	const FloatVariable& a = getInVal<FloatVariable>("A");
	const FloatVariable& b = getInVal<FloatVariable>("B");
	ImGui::Text("A %f",a.value);
	ImGui::Text("B %f",b.value);
	ImGui::Text("Res %f",a.value*b.value);

}

void MultiplyNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void MultiplyNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<FloatVariable>("Res")->val();
	const auto& a = getInVal<FloatVariable>("A");
	const auto& b = getInVal<FloatVariable>("B");
	ExportElement<std::string> ele;
	ele.dependencys = {a.name,b.name};
	ele.identifier = out.name;
	ele.callback = [out,a,b](RuiExportPrototype& proto) {
		if(proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = {} * {};",out.GetFormattedName(proto),a.GetFormattedName(proto), b.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = {} * {};",out.GetFormattedName(proto),a.GetFormattedName(proto), b.GetFormattedName(proto)));
	};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> MultiplyNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B",ImFlow::ConnectionFilter::SameType(),FloatVariable(2.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

AdditionNode::AdditionNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this,outName]() {
		const FloatVariable& a = getInVal<FloatVariable>("A");
		const FloatVariable& b = getInVal<FloatVariable>("B");
		std::string name = (a.IsConstant() && b.IsConstant())?"":outName;
		return FloatVariable(a.value + b.value,name);
		

	});

}

AdditionNode::AdditionNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):AdditionNode(rend,style){}

void AdditionNode::draw() {
	const FloatVariable& a = getInVal<FloatVariable>("A");
	const FloatVariable& b = getInVal<FloatVariable>("B");
	ImGui::Text("A %f",a.value);
	ImGui::Text("B %f",b.value);
	ImGui::Text("Res %f",a.value+b.value);

}

void AdditionNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void AdditionNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<FloatVariable>("Res")->val();
	const auto& a = getInVal<FloatVariable>("A");
	const auto& b = getInVal<FloatVariable>("B");
	ExportElement<std::string> ele;
	ele.dependencys = {a.name,b.name};
	ele.identifier = out.name;
	ele.callback = [out,a,b](RuiExportPrototype& proto) {
		if(proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = {} + {};",out.GetFormattedName(proto),a.GetFormattedName(proto), b.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = {} + {};",out.GetFormattedName(proto),a.GetFormattedName(proto), b.GetFormattedName(proto)));
	};
	proto.codeElements.push_back(ele);
}


std::vector<std::shared_ptr<ImFlow::PinProto>> AdditionNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}


SubtractNode::SubtractNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this,outName]() {
		const FloatVariable& a = getInVal<FloatVariable>("A");
		const FloatVariable& b = getInVal<FloatVariable>("B");
		std::string name = (a.IsConstant() && b.IsConstant())?"":outName;
		return FloatVariable(a.value - b.value,name);
		

	});

}

SubtractNode::SubtractNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):SubtractNode(rend,style){}

void SubtractNode::draw() {
	const FloatVariable& a = getInVal<FloatVariable>("A");
	const FloatVariable& b = getInVal<FloatVariable>("B");
	ImGui::Text("A %f",a.value);
	ImGui::Text("B %f",b.value);
	ImGui::Text("Res %f",a.value-b.value);

}

void SubtractNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void SubtractNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<FloatVariable>("Res")->val();
	const auto& a = getInVal<FloatVariable>("A");
	const auto& b = getInVal<FloatVariable>("B");
	ExportElement<std::string> ele;
	ele.dependencys = {a.name,b.name};
	ele.identifier = out.name;
	ele.callback = [out,a,b](RuiExportPrototype& proto) {
		if(proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = {} - {};",out.GetFormattedName(proto),a.GetFormattedName(proto), b.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = {} - {};",out.GetFormattedName(proto),a.GetFormattedName(proto), b.GetFormattedName(proto)));
	};
	proto.codeElements.push_back(ele);
}


std::vector<std::shared_ptr<ImFlow::PinProto>> SubtractNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

DivideNode::DivideNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this,outName]() {
		const FloatVariable& a = getInVal<FloatVariable>("A");
		const FloatVariable& b = getInVal<FloatVariable>("B");
		std::string name = (a.IsConstant() && b.IsConstant())?"":outName;
		return FloatVariable((b.value!=0.0f)?(a.value/b.value):1.0,name);

	});

}

DivideNode::DivideNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):DivideNode(rend,style){}

void DivideNode::draw() {
	const FloatVariable& a = getInVal<FloatVariable>("A");
	const FloatVariable& b = getInVal<FloatVariable>("B");
	ImGui::Text("A %f",a.value);
	ImGui::Text("B %f",b.value);
	if(b.value!=0.f)
	{
		setStyle(styles.GetNodeStyle(category));
		ImGui::Text("Res %f", a.value / b.value);
	}
	else
	{
		setStyle(styles.GetErrorStyle());
		ImGui::Text("Divide by 0");
	}

}

void DivideNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void DivideNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<FloatVariable>("Res")->val();
	const auto& a = getInVal<FloatVariable>("A");
	const auto& b = getInVal<FloatVariable>("B");
	ExportElement<std::string> ele;
	ele.dependencys = {a.name,b.name};
	ele.identifier = out.name;
	ele.callback = [out,a,b](RuiExportPrototype& proto) {
		if(proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("if({} == 0){{funcs->SetErrorWithReason(inst,\"Divide by zero\");\nreturn;\n}}\n{} = {} / {};",b.GetFormattedName(proto),out.GetFormattedName(proto),a.GetFormattedName(proto), b.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("if({} == 0){{funcs->SetErrorWithReason(inst,\"Divide by zero\");\nreturn;\n}}\n float {} = {} / {};",b.GetFormattedName(proto),out.GetFormattedName(proto),a.GetFormattedName(proto), b.GetFormattedName(proto)));
	};
	proto.codeElements.push_back(ele);
}


std::vector<std::shared_ptr<ImFlow::PinProto>> DivideNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B",ImFlow::ConnectionFilter::SameType(),FloatVariable(2.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

ModuloNode::ModuloNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this,outName]() {

		const FloatVariable& a = getInVal<FloatVariable>("A");
		const FloatVariable& b = getInVal<FloatVariable>("B");
		std::string name = (a.IsConstant() && b.IsConstant())?"":outName;
		return FloatVariable((b.value!=0.0f)?std::fmodf(a.value,b.value):1.0,name);
		

	});

}

ModuloNode::ModuloNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):ModuloNode(rend,style){}

void ModuloNode::draw() {
	const FloatVariable& a = getInVal<FloatVariable>("A");
	const FloatVariable& b = getInVal<FloatVariable>("B");
	ImGui::Text("A %f",a.value);
	ImGui::Text("B %f",b.value);
	if(b.value!=0.f)
	{
		setStyle(styles.GetNodeStyle(category));
		ImGui::Text("Res %f", std::fmodf(a.value,b.value));
	}
	else
	{
		setStyle(styles.GetErrorStyle());
		ImGui::Text("Divide by 0");
	}

}

void ModuloNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void ModuloNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<FloatVariable>("Res")->val();
	const auto& a = getInVal<FloatVariable>("A");
	const auto& b = getInVal<FloatVariable>("B");
	ExportElement<std::string> ele;
	ele.dependencys = {a.name,b.name};
	ele.identifier = out.name;
	ele.callback = [out,a,b](RuiExportPrototype& proto) {
		if(proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("if({} == 0){{\n\tfuncs->SetErrorWithReason(inst,\"Modulo with zero\");\n\treturn;\n}}\n{} = std::fmodf({},{});",b.GetFormattedName(proto),out.GetFormattedName(proto),a.GetFormattedName(proto), b.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("if({} == 0){{\n\tfuncs->SetErrorWithReason(inst,\"Modulo with zero\");\n\treturn;\n}}\nfloat {} = std::fmodf({},{});",b.GetFormattedName(proto),out.GetFormattedName(proto),a.GetFormattedName(proto), b.GetFormattedName(proto)));
	};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ModuloNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B",ImFlow::ConnectionFilter::SameType(),FloatVariable(1.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}


AbsoluteNode::AbsoluteNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this,outName]() {
		const FloatVariable& a = getInVal<FloatVariable>("A");
		std::string name = a.IsConstant()?"":outName;
		return FloatVariable(abs(a.value),name);
	});

}

AbsoluteNode::AbsoluteNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):AbsoluteNode(rend,style){}

void AbsoluteNode::draw() {
	const FloatVariable& a = getInVal<FloatVariable>("A");

	ImGui::Text("A %f",a.value);
	ImGui::Text("Res %f", abs(a.value));

}

void AbsoluteNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void AbsoluteNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<FloatVariable>("Res")->val();
	const auto& a = getInVal<FloatVariable>("A");

	ExportElement<std::string> ele;
	ele.dependencys = {a.name};
	ele.identifier = out.name;
	ele.callback = [out,a](RuiExportPrototype& proto) {
		if(proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = abs( {});",out.GetFormattedName(proto),a.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = abs( {});",out.GetFormattedName(proto),a.GetFormattedName(proto)));

	};
	proto.codeElements.push_back(ele);
}


std::vector<std::shared_ptr<ImFlow::PinProto>> AbsoluteNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

SineNode::SineNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this,outName]() {

		const FloatVariable& a = getInVal<FloatVariable>("A");
		std::string name = a.IsConstant()?"":outName;
		return FloatVariable(sin(a.value),name);

	});

}

SineNode::SineNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):SineNode(rend,style){}

void SineNode::draw() {
	const FloatVariable& a = getInVal<FloatVariable>("A");

	ImGui::Text("A %f",a.value);
	ImGui::Text("Res %f", sin(a.value));

}

void SineNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void SineNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<FloatVariable>("Res")->val();
	const auto& a = getInVal<FloatVariable>("A");

	ExportElement<std::string> ele;
	ele.dependencys = {a.name};
	ele.identifier = out.name;
	ele.callback = [out,a](RuiExportPrototype& proto) {
		if(proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = sin( {});",out.GetFormattedName(proto),a.GetFormattedName(proto)));
		else	
			proto.codeLines.push_back(std::format("float {} = sin( {});",out.GetFormattedName(proto),a.GetFormattedName(proto)));
	};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> SineNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

ExponentNode::ExponentNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this,outName]() {
		const FloatVariable& a = getInVal<FloatVariable>("A");
		const FloatVariable& b = getInVal<FloatVariable>("B");
		std::string name = (a.IsConstant() && b.IsConstant())?"":outName;

		return FloatVariable(std::pow(a.value,b.value), name);

	});

}

ExponentNode::ExponentNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):ExponentNode(rend,style){}

void ExponentNode::draw() {
	const FloatVariable& a = getInVal<FloatVariable>("A");
	const FloatVariable& b = getInVal<FloatVariable>("B");
	ImGui::Text("A %f",a.value);
	ImGui::Text("B %f",b.value);
	ImGui::Text("Res %f",std::pow(a.value, b.value));

}

void ExponentNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void ExponentNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<FloatVariable>("Res")->val();
	const auto& a = getInVal<FloatVariable>("A");
	const auto& b = getInVal<FloatVariable>("B");
	ExportElement<std::string> ele;
	ele.dependencys = {a.name,b.name};
	ele.identifier = out.name;
	ele.callback = [out,a,b](RuiExportPrototype& proto) {
		if(proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = std::pow({},{});",out.GetFormattedName(proto),a.GetFormattedName(proto), b.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = std::pow({},{});",out.GetFormattedName(proto),a.GetFormattedName(proto), b.GetFormattedName(proto)));
	};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ExponentNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

MappingNode::MappingNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this,outName]() {
		const FloatVariable& a = getInVal<FloatVariable>("A");
		return FloatVariable(map.MapVar(a.value), outName);
	});

}

MappingNode::MappingNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):MappingNode(rend,style){}

void MappingNode::draw() {
	const FloatVariable& a = getInVal<FloatVariable>("A");
	ImGui::Text("A %f",a.value);
	ImGui::Text("Res %f",map.MapVar(a.value));
	if(ImGui::Button("Edit Mapping")) {
		ImGui::OpenPopup("Mapping Editor");
	}
	MappingCreationPopup("Mapping Editor",a.value,map);

}

void MappingNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	//TODO export mapping
	RuiBaseNode::Serialize(obj,allocator);
}

void MappingNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<FloatVariable>("Res")->val();
	const auto& a = getInVal<FloatVariable>("A");
	ExportElement<std::string> ele;
	ele.dependencys = {a.name};
	ele.identifier = out.name;
	int mappingIndex = proto.mappings.size();
	proto.mappings.push_back(map);
	ele.callback = [mappingIndex,out,a](RuiExportPrototype& proto) {
		if(proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = funcs->map_v1(inst,{},{});",out.GetFormattedName(proto),mappingIndex,a.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = funcs->map_v1(inst,{},{});",out.GetFormattedName(proto),mappingIndex,a.GetFormattedName(proto)));
	};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> MappingNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}


void AddMathNodes(NodeEditor& editor) {

	editor.AddNodeType<AdditionNode>();
	editor.AddNodeType<MultiplyNode>();
	editor.AddNodeType<SubtractNode>();
	editor.AddNodeType<DivideNode>();
	editor.AddNodeType<ModuloNode>();
	editor.AddNodeType<AbsoluteNode>();
	editor.AddNodeType<SineNode>();
	editor.AddNodeType<ExponentNode>();
	editor.AddNodeType<MappingNode>();
}