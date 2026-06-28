#include "Nodes/MathNodes.h"


MultiplyNode::MultiplyNode(RenderInstance& rend,ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this,outName]() {

		const FloatVariable& a = getInNumeric("A");
		const FloatVariable& b = getInNumeric("B");
		std::string name = (a.IsConstant() && b.IsConstant())?"":outName;
		return FloatVariable(a.value * b.value,name);

	});

}

MultiplyNode::MultiplyNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):MultiplyNode(rend,style){}


void MultiplyNode::draw() {
	const FloatVariable& a = getInNumeric("A");
	const FloatVariable& b = getInNumeric("B");
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
	const auto& a = getInNumeric("A");
	const auto& b = getInNumeric("B");
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
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
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",isPinNumeric,FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B",isPinNumeric,FloatVariable(2.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

AdditionNode::AdditionNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this,outName]() {
		const FloatVariable& a = getInNumeric("A");
		const FloatVariable& b = getInNumeric("B");
		std::string name = (a.IsConstant() && b.IsConstant())?"":outName;
		return FloatVariable(a.value + b.value,name);
		

	});

}

AdditionNode::AdditionNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):AdditionNode(rend,style){}

void AdditionNode::draw() {
	const FloatVariable& a = getInNumeric("A");
	const FloatVariable& b = getInNumeric("B");
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
	const auto& a = getInNumeric("A");
	const auto& b = getInNumeric("B");
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
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
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",isPinNumeric,FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B",isPinNumeric,FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}


SubtractNode::SubtractNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this,outName]() {
		const FloatVariable& a = getInNumeric("A");
		const FloatVariable& b = getInNumeric("B");
		std::string name = (a.IsConstant() && b.IsConstant())?"":outName;
		return FloatVariable(a.value - b.value,name);
		

	});

}

SubtractNode::SubtractNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):SubtractNode(rend,style){}

void SubtractNode::draw() {
	const FloatVariable& a = getInNumeric("A");
	const FloatVariable& b = getInNumeric("B");
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
	const auto& a = getInNumeric("A");
	const auto& b = getInNumeric("B");
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
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
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",isPinNumeric,FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B",isPinNumeric,FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

DivideNode::DivideNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this,outName]() {
		const FloatVariable& a = getInNumeric("A");
		const FloatVariable& b = getInNumeric("B");
		std::string name = (a.IsConstant() && b.IsConstant())?"":outName;
		return FloatVariable((b.value!=0.0f)?(a.value/b.value):1.0,name);

	});

}

DivideNode::DivideNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):DivideNode(rend,style){}

void DivideNode::draw() {
	const FloatVariable& a = getInNumeric("A");
	const FloatVariable& b = getInNumeric("B");
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
	const auto& a = getInNumeric("A");
	const auto& b = getInNumeric("B");
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
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
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",isPinNumeric,FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B",isPinNumeric,FloatVariable(2.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

ModuloNode::ModuloNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this,outName]() {

		const FloatVariable& a = getInNumeric("A");
		const FloatVariable& b = getInNumeric("B");
		std::string name = (a.IsConstant() && b.IsConstant())?"":outName;
		return FloatVariable((b.value!=0.0f)?std::fmodf(a.value,b.value):1.0,name);
		

	});

}

ModuloNode::ModuloNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):ModuloNode(rend,style){}

void ModuloNode::draw() {
	const FloatVariable& a = getInNumeric("A");
	const FloatVariable& b = getInNumeric("B");
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
	const auto& a = getInNumeric("A");
	const auto& b = getInNumeric("B");
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
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
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",isPinNumeric,FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B",isPinNumeric,FloatVariable(1.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}


AbsoluteNode::AbsoluteNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this,outName]() {
		const FloatVariable& a = getInNumeric("A");
		std::string name = a.IsConstant()?"":outName;
		return FloatVariable(abs(a.value),name);
	});

}

AbsoluteNode::AbsoluteNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):AbsoluteNode(rend,style){}

void AbsoluteNode::draw() {
	const FloatVariable& a = getInNumeric("A");

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
	const auto& a = getInNumeric("A");

	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
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
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",isPinNumeric,FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

SineNode::SineNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this,outName]() {

		const FloatVariable& a = getInNumeric("A");
		std::string name = a.IsConstant()?"":outName;
		return FloatVariable(sin(a.value),name);

	});

}

SineNode::SineNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):SineNode(rend,style){}

void SineNode::draw() {
	const FloatVariable& a = getInNumeric("A");

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
	const auto& a = getInNumeric("A");

	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
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
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",isPinNumeric,FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

ExponentNode::ExponentNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this,outName]() {
		const FloatVariable& a = getInNumeric("A");
		const FloatVariable& b = getInNumeric("B");
		std::string name = (a.IsConstant() && b.IsConstant())?"":outName;

		return FloatVariable(std::pow(a.value,b.value), name);

	});

}

ExponentNode::ExponentNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):ExponentNode(rend,style){}

void ExponentNode::draw() {
	const FloatVariable& a = getInNumeric("A");
	const FloatVariable& b = getInNumeric("B");
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
	const auto& a = getInNumeric("A");
	const auto& b = getInNumeric("B");
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
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
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",isPinNumeric,FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B",isPinNumeric,FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

MappingNode::MappingNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this,outName]() {
		const FloatVariable& a = getInNumeric("A");
		return FloatVariable(map.MapVar(a.value), outName);
	});

}

MappingNode::MappingNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):MappingNode(rend,style){}

void MappingNode::draw() {
	const FloatVariable& a = getInNumeric("A");
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
	const auto& a = getInNumeric("A");
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
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


	struct MappingFileStruct {
		uint32_t dataCount;
		uint16_t nestedMappingCount;
		uint16_t isCublicSpline;
	};

	MappingFileStruct mappingFileStruct{};
	auto& mapping = proto.mappings[mappingIndex];
	//mappingFileStruct.isCublicSpline = proto.mappings[mappingIndex].cubicSpline;
	proto.mappings[mappingIndex].cubicSpline ? mappingFileStruct.isCublicSpline = 1 : mappingFileStruct.isCublicSpline = 0;
	mappingFileStruct.dataCount = mapping.controlPoints.size();
	mappingFileStruct.nestedMappingCount = 1;

	const size_t mappingFloatCount = mappingFileStruct.dataCount * (mappingFileStruct.isCublicSpline ? 3 : 2);
	std::vector<float> mappingFloatData(mappingFloatCount);
	if (mappingFileStruct.isCublicSpline) {
		// x, y,spline
		for (int i = 0; i < mappingFileStruct.dataCount; i++) {
			mappingFloatData[i] = (float)mapping.controlPoints[i].x;
			mappingFloatData[mappingFileStruct.dataCount + i * 2] = (float)mapping.controlPoints[i].y;
			mappingFloatData[mappingFileStruct.dataCount + i * 2 + 1] = (float)mapping.controlPoints[i].dir;
		}

	}
	else {
		for (int i = 0; i < mappingFileStruct.dataCount; i++) {
			mappingFloatData[i] = mapping.controlPoints[i].x;
			mappingFloatData[i + mappingFileStruct.dataCount] = (float)mapping.controlPoints[i].y;
		}
	}

	for (int i = 0; i < mappingFileStruct.dataCount; i++) {
		float x = mappingFloatData[i];
		float y = mappingFloatData[mappingFileStruct.dataCount + (mappingFileStruct.isCublicSpline ? i * 2 : i)];
		std::printf("Mapping point %d: x=%f, y=%f", i, x, y);
		if (mappingFileStruct.isCublicSpline) {
			float dir = mappingFloatData[mappingFileStruct.dataCount + i * 2 + 1];
			std::printf(", dir=%f", dir);
		}
		std::printf("\n");
	}
	std::vector<uint8_t> mappingData;
	mappingData.reserve(sizeof(mappingFileStruct) + mappingFloatData.size() * sizeof(float));

	const auto* mappingHeaderBytes = reinterpret_cast<const uint8_t*>(&mappingFileStruct);
	mappingData.insert(mappingData.end(), mappingHeaderBytes, mappingHeaderBytes + sizeof(mappingFileStruct));

	const auto* mappingFloatBytes = reinterpret_cast<const uint8_t*>(mappingFloatData.data());
	mappingData.insert(mappingData.end(), mappingFloatBytes, mappingFloatBytes + mappingFloatData.size() * sizeof(float));

	proto.AddMappingData(mappingData.data(), mappingData.size());
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> MappingNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",isPinNumeric,FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

TangentNode::TangentNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this, outName]() {

		const FloatVariable& a = getInNumeric("A");
		std::string name = a.IsConstant() ? "" : outName;
		return FloatVariable(tan(a.value), name);

		});

}

TangentNode::TangentNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :TangentNode(rend, style) {}

void TangentNode::draw() {
	const FloatVariable& a = getInNumeric("A");

	ImGui::Text("A %f", a.value);
	ImGui::Text("Res %f", tan(a.value));

}

void TangentNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void TangentNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<FloatVariable>("Res")->val();
	const auto& a = getInNumeric("A");

	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.dependencys = { a.name };
	ele.identifier = out.name;
	ele.callback = [out, a](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = tan( {});", out.GetFormattedName(proto), a.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = tan( {});", out.GetFormattedName(proto), a.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> TangentNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A", isPinNumeric, FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

CosineNode::CosineNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this, outName]() {

		const FloatVariable& a = getInNumeric("A");
		std::string name = a.IsConstant() ? "" : outName;
		return FloatVariable(cos(a.value), name);

		});

}

CosineNode::CosineNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :CosineNode(rend, style) {}

void CosineNode::draw() {
	const FloatVariable& a = getInNumeric("A");

	ImGui::Text("A %f", a.value);
	ImGui::Text("Res %f", cos(a.value));

}

void CosineNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void CosineNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<FloatVariable>("Res")->val();
	const auto& a = getInNumeric("A");

	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.dependencys = { a.name };
	ele.identifier = out.name;
	ele.callback = [out, a](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = cos( {});", out.GetFormattedName(proto), a.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = cos( {});", out.GetFormattedName(proto), a.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> CosineNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A", isPinNumeric, FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

SquareRootNode::SquareRootNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this, outName]() {

		const FloatVariable& a = getInNumeric("A");
		std::string name = a.IsConstant() ? "" : outName;
		return FloatVariable(cos(a.value), name);

		});

}

SquareRootNode::SquareRootNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :SquareRootNode(rend, style) {}

void SquareRootNode::draw() {
	const FloatVariable& a = getInNumeric("A");

	ImGui::Text("A %f", a.value);
	ImGui::Text("Res %f", sqrt(a.value));

}

void SquareRootNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void SquareRootNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<FloatVariable>("Res")->val();
	const auto& a = getInNumeric("A");

	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.dependencys = { a.name };
	ele.identifier = out.name;
	ele.callback = [out, a](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = sqrt( {});", out.GetFormattedName(proto), a.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = sqrt( {});", out.GetFormattedName(proto), a.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> SquareRootNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A", isPinNumeric, FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

RoundNode::RoundNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this, outName]() {

		const FloatVariable& a = getInNumeric("A");
		std::string name = a.IsConstant() ? "" : outName;
		return FloatVariable(round(a.value), name);

		});

}

RoundNode::RoundNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :RoundNode(rend, style) {}

void RoundNode::draw() {
	const FloatVariable& a = getInNumeric("A");

	ImGui::Text("A %f", a.value);
	ImGui::Text("Res %f", round(a.value));

}

void RoundNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void RoundNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<FloatVariable>("Res")->val();
	const auto& a = getInNumeric("A");

	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.dependencys = { a.name };
	ele.identifier = out.name;
	ele.callback = [out, a](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = round( {});", out.GetFormattedName(proto), a.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = round( {});", out.GetFormattedName(proto), a.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> RoundNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A", isPinNumeric, FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

FloorNode::FloorNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this, outName]() {

		const FloatVariable& a = getInNumeric("A");
		std::string name = a.IsConstant() ? "" : outName;
		return FloatVariable(floor(a.value), name);

		});

}

FloorNode::FloorNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :FloorNode(rend, style) {}

void FloorNode::draw() {
	const FloatVariable& a = getInNumeric("A");

	ImGui::Text("A %f", a.value);
	ImGui::Text("Res %f", floor(a.value));

}

void FloorNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void FloorNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<FloatVariable>("Res")->val();
	const auto& a = getInNumeric("A");

	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.dependencys = { a.name };
	ele.identifier = out.name;
	ele.callback = [out, a](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = floor( {});", out.GetFormattedName(proto), a.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = floor( {});", out.GetFormattedName(proto), a.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> FloorNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A", isPinNumeric, FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

CeilNode::CeilNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this, outName]() {

		const FloatVariable& a = getInNumeric("A");
		std::string name = a.IsConstant() ? "" : outName;
		return FloatVariable(ceil(a.value), name);

		});

}

CeilNode::CeilNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :CeilNode(rend, style) {}

void CeilNode::draw() {
	const FloatVariable& a = getInNumeric("A");

	ImGui::Text("A %f", a.value);
	ImGui::Text("Res %f", ceil(a.value));

}

void CeilNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void CeilNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<FloatVariable>("Res")->val();
	const auto& a = getInNumeric("A");

	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.dependencys = { a.name };
	ele.identifier = out.name;
	ele.callback = [out, a](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = ceil( {});", out.GetFormattedName(proto), a.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = ceil( {});", out.GetFormattedName(proto), a.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> CeilNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A", isPinNumeric, FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

TruncNode::TruncNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this, outName]() {

		const FloatVariable& a = getInNumeric("A");
		std::string name = a.IsConstant() ? "" : outName;
		return FloatVariable(trunc(a.value), name);

		});

}

TruncNode::TruncNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :TruncNode(rend, style) {}

void TruncNode::draw() {
	const FloatVariable& a = getInNumeric("A");

	ImGui::Text("A %f", a.value);
	ImGui::Text("Res %f", trunc(a.value));

}

void TruncNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void TruncNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<FloatVariable>("Res")->val();
	const auto& a = getInNumeric("A");

	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.dependencys = { a.name };
	ele.identifier = out.name;
	ele.callback = [out, a](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = trunc( {});", out.GetFormattedName(proto), a.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = trunc( {});", out.GetFormattedName(proto), a.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> TruncNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A", isPinNumeric, FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

ClampNode::ClampNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this, outName]() {

		const FloatVariable& min = getInNumeric("Min");
		const FloatVariable& max = getInNumeric("Max");
		const FloatVariable& val = getInNumeric("Val");
		std::string name = (min.IsConstant()&&max.IsConstant()&&val.IsConstant()) ? "" : outName;
		if(min.value>max.value)
			return FloatVariable(0.f,name);
		return FloatVariable(std::clamp(val.value,min.value,max.value), name);

	});

}

ClampNode::ClampNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :ClampNode(rend, style) {}

void ClampNode::draw() {
	const FloatVariable& min = getInNumeric("Min");
	const FloatVariable& max = getInNumeric("Max");
	const FloatVariable& val = getInNumeric("Val");


	ImGui::Text("Min %f", min.value);
	ImGui::Text("Max %f", max.value);
	ImGui::Text("Val %f", val.value);
	if (min.value > max.value)
		ImGui::Text("Error");
	else
		ImGui::Text("Res %f", std::clamp(val.value,min.value,max.value));

}

void ClampNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void ClampNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<FloatVariable>("Res")->val();
	const FloatVariable& min = getInNumeric("Min");
	const FloatVariable& max = getInNumeric("Max");
	const FloatVariable& val = getInNumeric("Val");

	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.dependencys = { min.name,max.name,val.name };
	ele.identifier = out.name;
	ele.callback = [out, min,max,val](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = std::clamp( (float){}, (float){}, (float){});", out.GetFormattedName(proto), val.GetFormattedName(proto),min.GetFormattedName(proto),max.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = std::clamp( (float){}, (float){}, (float){});", out.GetFormattedName(proto), val.GetFormattedName(proto),min.GetFormattedName(proto),max.GetFormattedName(proto)));
	};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ClampNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Min", isPinNumeric, FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Max", isPinNumeric, FloatVariable(1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Val", isPinNumeric, FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

ProjectionNode::ProjectionNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<Float2Variable>("Res")->behaviour([this, outName]() {

		return Float2Variable(Vector2(0.5f,0.5f), name);

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
			proto.codeLines.push_back(std::format("{} = project3d(funcs,globals,inst,{});", out.GetFormattedName(proto), pos.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = project3d(funcs,globals,inst,{});", out.GetFormattedName(proto), pos.GetFormattedName(proto)));
	};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ProjectionNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<Float3Variable>>("Position",ImFlow::ConnectionFilter::SameType() , Float3Variable(Vector3(0.f,0.f,0.f))));
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float2Variable>>("Res"));
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
	editor.AddNodeType<TangentNode>();
	editor.AddNodeType<CosineNode>();
	editor.AddNodeType<SquareRootNode>();
	editor.AddNodeType<RoundNode>();
	editor.AddNodeType<FloorNode>();
	editor.AddNodeType<CeilNode>();
	editor.AddNodeType<TruncNode>();
	editor.AddNodeType<ClampNode>();
	editor.AddNodeType<ProjectionNode>();
}
