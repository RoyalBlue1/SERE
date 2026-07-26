#include "Nodes/MathNodes.h"


MathNodeConnectionType TypeInfoToConnectionType(const std::type_info& typeInfo)
{
	if (typeInfo == typeid(FloatVariable))
		return MathNodeConnectionType::Float;
	if (typeInfo == typeid(Float2Variable))
		return MathNodeConnectionType::Float2;
	if (typeInfo == typeid(Float3Variable))
		return MathNodeConnectionType::Float3;
	if (typeInfo == typeid(ColorVariable))
		return MathNodeConnectionType::Color;
	if (typeInfo == typeid(TransformSize))
		return MathNodeConnectionType::Size;
	return MathNodeConnectionType::Invalid;
}

BaseMathNode::BaseMathNode(const std::string& name,const std::string& category,std::vector<std::shared_ptr<ImFlow::PinProto>> pinInfo,const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles):
RuiBaseNode(name,category,std::move(pinInfo),rend,styles),
nodeName(name),
nodeCategory(category)
{}

void BaseMathNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator)
{
	obj.AddMember("Name",nodeName,allocator);
	obj.AddMember("Category",nodeCategory,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void BaseMathNode::UpdateInPin(const char* name,MathNodeConnectionType& lastConnectionType,std::unordered_map<std::string,std::any>& emptyVals)
{
	MathNodeConnectionType connectionType = GetConnectionRestrictions();
	if (lastConnectionType == connectionType)
		return;
	if ((lastConnectionType == MathNodeConnectionType::None&&connectionType == MathNodeConnectionType::Float) ||
		(lastConnectionType == MathNodeConnectionType::Float&& connectionType == MathNodeConnectionType::None))
	{
		lastConnectionType = connectionType;
		return;
	}


	ImFlow::Pin* connectedPin = nullptr;
	if (inPin(name)->isConnected())
		connectedPin = inPin(name)->getLink().lock()->left();
	inPin(name)->deleteLink();
	switch (lastConnectionType)
	{
	case MathNodeConnectionType::None:
	case MathNodeConnectionType::Float:
		storePin<FloatVariable>(name,emptyVals);
		break;
	case MathNodeConnectionType::Float2:
		storePin<Float2Variable>(name,emptyVals);
		break;
	case MathNodeConnectionType::Float3:
		storePin<Float3Variable>(name,emptyVals);
		break;
	case MathNodeConnectionType::Color:
		storePin<ColorVariable>(name,emptyVals);
		break;
	case MathNodeConnectionType::Size:
		storePin<TransformSize>(name,emptyVals);
		break;
	}

	switch (connectionType)
	{
	case MathNodeConnectionType::None:
	case MathNodeConnectionType::Float:
		recreatePin<FloatVariable>(name,isPinMath,emptyVals);
		break;
	case MathNodeConnectionType::Float2:
		recreatePin<Float2Variable>(name,isPinMath,emptyVals);
		break;
	case MathNodeConnectionType::Float3:
		recreatePin<Float3Variable>(name,isPinMath,emptyVals);
		break;
	case MathNodeConnectionType::Color:
		recreatePin<ColorVariable>(name,isPinMath,emptyVals);
		break;
	case MathNodeConnectionType::Size:
		recreatePin<TransformSize>(name,isPinMath,emptyVals);
		break;
	}
	if (connectedPin)
		inPin(name)->createLink(connectedPin);

	lastConnectionType = connectionType;
}

MathNodeConnectionType BaseMathNode::OutConnectionType()
{
	MathNodeConnectionType outConnection = MathNodeConnectionType::None;
	if (outPin("Res")->isConnected())
	{
		outConnection = MathNodeConnectionType::Float;
	}
	if (outPin("Vector2 Res")->isConnected())
	{
		if (outConnection != MathNodeConnectionType::None)
			return MathNodeConnectionType::Invalid;
		outConnection = MathNodeConnectionType::Float2;
	}
	if (outPin("Vector3 Res")->isConnected())
	{
		if (outConnection != MathNodeConnectionType::None)
			return MathNodeConnectionType::Invalid;
		outConnection =  MathNodeConnectionType::Float3;
	}
	if (outPin("Color Res")->isConnected())
	{
		if (outConnection != MathNodeConnectionType::None)
			return MathNodeConnectionType::Invalid;
		outConnection = MathNodeConnectionType::Color;
	}
	return outConnection;
}

void BaseMathNode::UpdateOutPinVisibility()
{
	outPin("Res")->visible(false);
	outPin("Vector2 Res")->visible(false);
	outPin("Vector3 Res")->visible(false);
	outPin("Color Res")->visible(false);
	outPin("Size Res")->visible(false);
	switch (GetConnectionRestrictions())
	{
	case MathNodeConnectionType::None:
	case MathNodeConnectionType::Invalid:
		outPin("Res")->visible(true);
		outPin("Vector2 Res")->visible(true);
		outPin("Vector3 Res")->visible(true);
		outPin("Color Res")->visible(true);
		outPin("Size Res")->visible(true);
		break;
	case MathNodeConnectionType::Float:
		outPin("Res")->visible(true);
		break;
	case MathNodeConnectionType::Float2:
		outPin("Vector2 Res")->visible(true);
		break;
	case MathNodeConnectionType::Float3:
		outPin("Vector3 Res")->visible(true);
		break;
	case MathNodeConnectionType::Color:
		outPin("Color Res")->visible(true);
		break;
	case MathNodeConnectionType::Size:
		outPin("Size Res")->visible(true);
		break;
	}
}

UnaryMathNode::UnaryMathNode(const std::string& name,const std::string& category,const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles):
	BaseMathNode(name,category,GetPinInfo(),rend,styles),
	lastConnectionType(MathNodeConnectionType::None)
{
	std::string outFloatName = Variable::UniqueName();
	std::string outFloat2Name = Variable::UniqueName();
	std::string outFloat3Name = Variable::UniqueName();
	std::string outColorName = Variable::UniqueName();
	std::string outSizeName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this, outFloatName]() {
		if (GetConnectionRestrictions() != MathNodeConnectionType::Float)
			return FloatVariable(1);

		FloatVariable in = getInVal<FloatVariable>("In");
		return FloatVariable(Operation(in.value),outFloatName);

	});
	getOut<Float2Variable>("Vector2 Res")->behaviour([this, outFloat2Name]() {
		if (GetConnectionRestrictions() != MathNodeConnectionType::Float2)
			return Float2Variable(1,1);

		Float2Variable in = getInVal<Float2Variable>("In");
		return Float2Variable(Operation(in.value.x),Operation(in.value.y),outFloat2Name);
	});
	getOut<Float3Variable>("Vector3 Res")->behaviour([this, outFloat3Name]() {
		if (GetConnectionRestrictions() != MathNodeConnectionType::Float3)
			return Float3Variable(1,1,1);

		Float3Variable in = getInVal<Float3Variable>("In");
		return Float3Variable(Operation(in.value.x),Operation(in.value.y),Operation(in.value.z),outFloat3Name);
	});
	getOut<ColorVariable>("Color Res")->behaviour([this, outColorName]() {
		if (GetConnectionRestrictions() != MathNodeConnectionType::Color)
			return ColorVariable(1,1,1,1);

		ColorVariable in = getInVal<ColorVariable>("In");
		return ColorVariable(Operation(in.value.red),Operation(in.value.green),Operation(in.value.blue),Operation(in.value.alpha),outColorName);
	});
	getOut<TransformSize>("Size Res")->behaviour([this, outSizeName]() {
		if (GetConnectionRestrictions() != MathNodeConnectionType::Float3)
			return TransformSize(_mm_set1_ps(1));

		TransformSize in = getInVal<TransformSize>("In");
		float inVal[4];
		_mm_store_ps(inVal,in.size);
		__m128 outVal = _mm_set_ps(
			Operation(inVal[3]),
			Operation(inVal[2]),
			Operation(inVal[1]),
			Operation(inVal[0])
		);
		return TransformSize(outVal,outSizeName);
	});
}




std::vector<std::shared_ptr<ImFlow::PinProto>> UnaryMathNode::GetPinInfo()
{
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("In", isPinMath, FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float2Variable>>("Vector2 Res"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float3Variable>>("Vector3 Res"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<ColorVariable>>("Color Res"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<TransformSize>>("Size Res"));
	return info;
}

MathNodeConnectionType UnaryMathNode::GetConnectionRestrictions()
{

	MathNodeConnectionType inType = MathNodeConnectionType::None;
	if (inPin("In")->isConnected())
		inType = TypeInfoToConnectionType(inPin("In")->getLink().lock()->left()->getDataType());
	MathNodeConnectionType outType = OutConnectionType();
	if (outType == MathNodeConnectionType::None)
		return inType;
	if (inType == MathNodeConnectionType::None)
		return outType;
	if (inType == outType)
		return inType;
	return MathNodeConnectionType::Invalid;
}




bool UnaryMathNode::CanCreateLink(ImFlow::Pin* own, ImFlow::Pin* other)
{
	switch (GetConnectionRestrictions())
	{
	case MathNodeConnectionType::None:
		return true;
	case MathNodeConnectionType::Float:
		if (other->getDataType() == typeid(FloatVariable))
			return true;
		return false;
	case MathNodeConnectionType::Float2:
		if (other->getDataType() == typeid(Float2Variable))
			return true;
		return false;
	case MathNodeConnectionType::Float3:
		if (other->getDataType() == typeid(Float3Variable))
			return true;
		return false;
	case MathNodeConnectionType::Color:
		if (other->getDataType() == typeid(ColorVariable))
			return true;
		return false;
	case MathNodeConnectionType::Size:
		if (other->getDataType() == typeid(TransformSize))
			return true;
		return false;
	case MathNodeConnectionType::Invalid:
		return false;
	}
	return false;
}



void UnaryMathNode::Export(RuiExportPrototype& proto)
{

}

void UnaryMathNode::draw()
{
	UpdateInPin("In",lastConnectionType,inPinEmptyVal);
	UpdateOutPinVisibility();

}

BinaryMathNode::BinaryMathNode(const std::string& name,const std::string& category,const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& styles):
	BaseMathNode(name,category,GetPinInfo(),rend,styles),
	lastConnectionType(MathNodeConnectionType::None)
{
	std::string outFloatName = Variable::UniqueName();
	std::string outFloat2Name = Variable::UniqueName();
	std::string outFloat3Name = Variable::UniqueName();
	std::string outColorName = Variable::UniqueName();
	std::string outSizeName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this, outFloatName]() {
		if (GetConnectionRestrictions() != MathNodeConnectionType::Float)
			return FloatVariable(1);

		FloatVariable a = getInVal<FloatVariable>("A");
		FloatVariable b = getInVal<FloatVariable>("B");
		return FloatVariable(Operation(a.value,b.value),outFloatName);

	});
	getOut<Float2Variable>("Vector2 Res")->behaviour([this, outFloat2Name]() {
		if (GetConnectionRestrictions() != MathNodeConnectionType::Float2)
			return Float2Variable(1,1);

		Float2Variable a = getInVal<Float2Variable>("A");
		if (inPin("B")->isConnected() && inPin("B")->getDataType() == typeid(Float2Variable))
		{
			Float2Variable b = getInVal<Float2Variable>("B");
			return Float2Variable(Operation(a.value.x,b.value.x),Operation(a.value.y,b.value.y),outFloat2Name);
		}
		FloatVariable b = getInVal<FloatVariable>("B");
		return Float2Variable(Operation(a.value.x,b.value),Operation(a.value.y,b.value),outFloat2Name);
	});
	getOut<Float3Variable>("Vector3 Res")->behaviour([this, outFloat3Name]() {
		if (GetConnectionRestrictions() != MathNodeConnectionType::Float3)
			return Float3Variable(1,1,1);

		Float3Variable a = getInVal<Float3Variable>("A");
		if (inPin("B")->isConnected() && inPin("B")->getDataType() == typeid(Float3Variable))
		{
			Float3Variable b = getInVal<Float3Variable>("B");
			return Float3Variable(Operation(a.value.x,b.value.x),Operation(a.value.y,b.value.y),Operation(a.value.z,b.value.z),outFloat3Name);
		}
		FloatVariable b = getInVal<FloatVariable>("B");
		return Float3Variable(Operation(a.value.x,b.value),Operation(a.value.y,b.value),Operation(a.value.z,b.value));
	});
	getOut<ColorVariable>("Color Res")->behaviour([this, outColorName]() {
		if (GetConnectionRestrictions() != MathNodeConnectionType::Color)
			return ColorVariable(1,1,1,1);

		ColorVariable a = getInVal<ColorVariable>("A");
		if (inPin("B")->getDataType() == typeid(ColorVariable))
		{
			ColorVariable b = getInVal<ColorVariable>("B");
		return ColorVariable(
			Operation(a.value.red,b.value.red),
			Operation(a.value.green,b.value.green),
			Operation(a.value.blue,b.value.blue),
			Operation(a.value.alpha,b.value.alpha),
			outColorName);
		}
		FloatVariable b = getInVal<FloatVariable>("B");
		return ColorVariable(
			Operation(a.value.red,b.value),
			Operation(a.value.green,b.value),
			Operation(a.value.blue,b.value),
			Operation(a.value.alpha,b.value),
			outColorName);
	});
	getOut<TransformSize>("Size Res")->behaviour([this, outSizeName]() {
		if (GetConnectionRestrictions() != MathNodeConnectionType::Float3)
			return TransformSize(_mm_set1_ps(1));

		TransformSize a = getInVal<TransformSize>("A");
		if (inPin("B")->isConnected() && inPin("B")->getDataType() == typeid(TransformSize))
		{
			TransformSize b = getInVal<TransformSize>("B");
			float aInVal[4];
			float bInVal[4];
			_mm_store_ps(aInVal,a.size);
			_mm_store_ps(bInVal,b.size);
			__m128 outVal = _mm_set_ps(
				Operation(aInVal[3],bInVal[3]),
				Operation(aInVal[2],bInVal[2]),
				Operation(aInVal[1],bInVal[1]),
				Operation(aInVal[0],bInVal[0])

			);
			return TransformSize(outVal,outSizeName);
		}
		FloatVariable b = getInVal<FloatVariable>("B");
		float aInVal[4];

		_mm_store_ps(aInVal,a.size);
		__m128 outVal = _mm_set_ps(
			Operation(aInVal[3],b.value),
			Operation(aInVal[2],b.value),
			Operation(aInVal[1],b.value),
			Operation(aInVal[0],b.value)

		);
		return TransformSize(outVal,outSizeName);
	});
}

std::vector<std::shared_ptr<ImFlow::PinProto>> BinaryMathNode::GetPinInfo()
{
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A", isPinMath, FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B", isPinMath, FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float2Variable>>("Vector2 Res"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float3Variable>>("Vector3 Res"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<ColorVariable>>("Color Res"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<TransformSize>>("Size Res"));
	return info;
}

MathNodeConnectionType BinaryMathNode::GetConnectionRestrictions()
{

	MathNodeConnectionType aInType = MathNodeConnectionType::None;
	if (inPin("A")->isConnected())
		aInType = TypeInfoToConnectionType(inPin("A")->getLink().lock()->left()->getDataType());
	MathNodeConnectionType bInType = MathNodeConnectionType::None;
	if (inPin("B")->isConnected())
		bInType = TypeInfoToConnectionType(inPin("B")->getLink().lock()->left()->getDataType());

	MathNodeConnectionType outType = OutConnectionType();

	if (bInType == MathNodeConnectionType::None || bInType == MathNodeConnectionType::Float)
		bInType = aInType;
	if (aInType == MathNodeConnectionType::None || aInType == MathNodeConnectionType::Float)
		aInType = bInType;
	if (aInType != bInType)
		return MathNodeConnectionType::Invalid;

	if (outType == MathNodeConnectionType::None)
		return aInType;
	if (aInType == MathNodeConnectionType::None)
		return outType;
	if (aInType == outType)
		return aInType;
	return MathNodeConnectionType::Invalid;
}




bool BinaryMathNode::CanCreateLink(ImFlow::Pin* own, ImFlow::Pin* other)
{
	if (own->getName() == "B"&&other->getDataType() == typeid(FloatVariable))
		return true;

	switch (GetConnectionRestrictions())
	{
	case MathNodeConnectionType::None:
		return true;
	case MathNodeConnectionType::Float:
		if (other->getDataType() == typeid(FloatVariable))
			return true;
		return false;
	case MathNodeConnectionType::Float2:
		if (other->getDataType() == typeid(Float2Variable))
			return true;
		return false;
	case MathNodeConnectionType::Float3:
		if (other->getDataType() == typeid(Float3Variable))
			return true;
		return false;
	case MathNodeConnectionType::Color:
		if (other->getDataType() == typeid(ColorVariable))
			return true;
		return false;
	case MathNodeConnectionType::Size:
		if (other->getDataType() == typeid(TransformSize))
			return true;
		return false;
	case MathNodeConnectionType::Invalid:
		return false;
	}
	return false;
}



void BinaryMathNode::Export(RuiExportPrototype& proto)
{

}

void BinaryMathNode::draw()
{
	UpdateInPin("A",lastConnectionType,inPinEmptyVal);
	UpdateOutPinVisibility();

}


MultiplyNode::MultiplyNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style) :
BinaryMathNode(name, category,rend,style)
{}

MultiplyNode::MultiplyNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):MultiplyNode(rend,style){}

float MultiplyNode::Operation(float a,float b)
{
	return a*b;
}

std::string MultiplyNode::OperationString(std::string a,std::string b)
{
	return std::format("{} * {}",a,b);
}

AdditionNode::AdditionNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style):
BinaryMathNode(name, category,rend,style)
{}

AdditionNode::AdditionNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):AdditionNode(rend,style){}

float AdditionNode::Operation(float a,float b)
{
	return a+b;
}

std::string AdditionNode::OperationString(std::string a,std::string b)
{
	return std::format("{} + {}",a,b);
}

SubtractNode::SubtractNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style):
BinaryMathNode(name, category,rend,style)
{}
SubtractNode::SubtractNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):SubtractNode(rend,style){}

float SubtractNode::Operation(float a,float b)
{
	return a-b;
}

std::string SubtractNode::OperationString(std::string a,std::string b)
{
	return std::format("{} - {}",a,b);
}

DivideNode::DivideNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style):
BinaryMathNode(name, category,rend,style)
{}


DivideNode::DivideNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):DivideNode(rend,style){}

float DivideNode::Operation(float a,float b)
{
	return a/b;
}

std::string DivideNode::OperationString(std::string a,std::string b)
{
	return std::format("{} / {}",a,b);
}

ModuloNode::ModuloNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style):
BinaryMathNode(name, category,rend,style)
{}


ModuloNode::ModuloNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):ModuloNode(rend,style){}

float ModuloNode::Operation(float a,float b)
{
	return std::fmodf(a,b);
}

std::string ModuloNode::OperationString(std::string a,std::string b)
{
	return std::format("std::fmodf({}, {})",a,b);
}

AbsoluteNode::AbsoluteNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style):
UnaryMathNode(name, category,rend,style)
{}

AbsoluteNode::AbsoluteNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):AbsoluteNode(rend,style){}

float AbsoluteNode::Operation(float a)
{
	return std::abs(a);
}

std::string AbsoluteNode::OperationString(std::string a)
{
	return std::format("std::abs({})",a);
}

SineNode::SineNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style):
UnaryMathNode(name, category,rend,style)
{}

SineNode::SineNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):SineNode(rend,style){}

float SineNode::Operation(float a)
{
	return std::sinf(a);
}

std::string SineNode::OperationString(std::string a)
{
	return std::format("std::sinf({})",a);
}

ExponentNode::ExponentNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style):
BinaryMathNode(name, category,rend,style)
{}

ExponentNode::ExponentNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):ExponentNode(rend,style){}

float ExponentNode::Operation(float a,float b)
{
	return std::pow(a,b);
}

std::string ExponentNode::OperationString(std::string a,std::string b)
{
	return std::format("std::pow({},{})",a,b);
}

MappingNode::MappingNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this,outName]() {
		const FloatVariable& a = getInNumeric("A");
		return FloatVariable(map.MapVar(a.value), outName);
	});

}

MappingNode::MappingNode(const std::shared_ptr<RenderInstance>& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):MappingNode(rend,style){
	if(obj.HasMember("CubicSpline")&&obj["CubicSpline"].IsBool())
		map.cubicSpline = obj["CubicSpline"].GetBool();
	if(obj.HasMember("ControlPoints")&&obj["ControlPoints"].IsArray()) {
		auto points = obj["ControlPoints"].GetArray();
		if(points.Size() >= 2) {
			map.controlPoints.clear();
			for (auto itr = points.Begin(); itr != points.End(); itr++) {
				if(!itr->IsObject())continue;
				auto point = itr->GetObject();
				if(!(point.HasMember("X")&&point["X"].IsNumber()))continue;
				if(!(point.HasMember("Y")&&point["Y"].IsNumber()))continue;
				float dir = 0.f;
				if(point.HasMember("Dir")&&point["Dir"].IsNumber())
					dir = point["Dir"].GetFloat();
				map.AddControlPoint(point["X"].GetFloat(), point["Y"].GetFloat(), dir);
			}
			if(map.controlPoints.size() < 2)
				map = Mapping();
		}
	}
}

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
	obj.AddMember("CubicSpline",map.cubicSpline,allocator);
	rapidjson::GenericValue<rapidjson::UTF8<>> controlPoints;
	controlPoints.SetArray();
	for(auto& point:map.controlPoints) {
		rapidjson::GenericValue<rapidjson::UTF8<>> pointObj;
		pointObj.SetObject();
		pointObj.AddMember("X",point.x,allocator);
		pointObj.AddMember("Y",point.y,allocator);
		pointObj.AddMember("Dir",point.dir,allocator);
		controlPoints.PushBack(pointObj,allocator);
	}
	obj.AddMember("ControlPoints",controlPoints,allocator);
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
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> MappingNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A",isPinNumeric,FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

TangentNode::TangentNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style) :
UnaryMathNode(name, category,rend,style)
{}

TangentNode::TangentNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :TangentNode(rend, style) {}

float TangentNode::Operation(float a)
{
	return std::tanf(a);
}

std::string TangentNode::OperationString(std::string a)
{
	return std::format("std::tanf({})",a);
}

CosineNode::CosineNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style) :
UnaryMathNode(name, category,rend,style)
{}

CosineNode::CosineNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :CosineNode(rend, style) {}

float CosineNode::Operation(float a)
{
	return std::cosf(a);
}

std::string CosineNode::OperationString(std::string a)
{
	return std::format("std::cosf({})",a);
}

SquareRootNode::SquareRootNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style) :
UnaryMathNode(name, category,rend,style)
{}

SquareRootNode::SquareRootNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :SquareRootNode(rend, style) {}

float SquareRootNode::Operation(float a)
{
	return std::sqrtf(a);
}

std::string SquareRootNode::OperationString(std::string a)
{
	return std::format("std::sqrtf({})",a);
}

RoundNode::RoundNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style) :
UnaryMathNode(name, category,rend,style)
{}

RoundNode::RoundNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :RoundNode(rend, style) {}

float RoundNode::Operation(float a)
{
	return std::roundf(a);
}

std::string RoundNode::OperationString(std::string a)
{
	return std::format("std::roundf({})",a);
}

FloorNode::FloorNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style) :
UnaryMathNode(name, category,rend,style)
{}

FloorNode::FloorNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :FloorNode(rend, style) {}

float FloorNode::Operation(float a)
{
	return std::floorf(a);
}

std::string FloorNode::OperationString(std::string a)
{
	return std::format("std::floorf({})",a);
}


CeilNode::CeilNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style) :
UnaryMathNode(name, category,rend,style)
{}

CeilNode::CeilNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :CeilNode(rend, style) {}

float CeilNode::Operation(float a)
{
	return std::ceilf(a);
}

std::string CeilNode::OperationString(std::string a)
{
	return std::format("std::ceilf({})",a);
}

TruncNode::TruncNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style) :
UnaryMathNode(name, category,rend,style)
{}

TruncNode::TruncNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :TruncNode(rend, style) {}

float TruncNode::Operation(float a)
{
	return std::truncf(a);
}

std::string TruncNode::OperationString(std::string a)
{
	return std::format("std::truncf({})",a);
}


ClampNode::ClampNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
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

ClampNode::ClampNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :ClampNode(rend, style) {}

void ClampNode::draw() {
	const FloatVariable& min = getInNumeric("Min");
	const FloatVariable& max = getInNumeric("Max");
	const FloatVariable& val = getInNumeric("Val");


	ImGui::Text("Min %f", min.value);
	ImGui::Text("Max %f", max.value);
	ImGui::Text("Val %f", val.value);
	if (min.value > max.value)
	{
		setStyle(styles.GetErrorStyle());
		ImGui::Text("Error");
	}
	else {
		setStyle(styles.GetNodeStyle(category));
		ImGui::Text("Res %f", std::clamp(val.value, min.value, max.value));
	}
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

MinNode::MinNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style)
{
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this, outName]() {
		const FloatVariable& a = getInNumeric("A");
		const FloatVariable& b = getInNumeric("B");
		return FloatVariable(std::min(a.value, b.value), outName);
	});
}

MinNode::MinNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :MinNode(rend, style) {}

void MinNode::draw()
{
	
}

void MinNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator)
{
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void MinNode::Export(RuiExportPrototype& proto)
{
	const auto& out = getOut<FloatVariable>("Res")->val();
	const FloatVariable& a = getInNumeric("A");
	const FloatVariable& b = getInNumeric("B");
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.dependencys = { a.name,b.name };
	ele.identifier = out.name;
	ele.callback = [out, a, b](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = std::min( (float){}, (float){});", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = std::min( (float){}, (float){});", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> MinNode::GetPinInfo()
{
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A", isPinNumeric, FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B", isPinNumeric, FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

MaxNode::MaxNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style)
{
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this, outName]() {
		const FloatVariable& a = getInNumeric("A");
		const FloatVariable& b = getInNumeric("B");
		return FloatVariable(std::max(a.value, b.value), outName);
	});
}

MaxNode::MaxNode(const std::shared_ptr<RenderInstance>& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :MaxNode(rend, style) {}

void MaxNode::draw()
{}

void MaxNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator)
{
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void MaxNode::Export(RuiExportPrototype& proto)
{
	const auto& out = getOut<FloatVariable>("Res")->val();
	const FloatVariable& a = getInNumeric("A");
	const FloatVariable& b = getInNumeric("B");
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.dependencys = { a.name,b.name };
	ele.identifier = out.name;
	ele.callback = [out, a, b](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = std::max( (float){}, (float){});", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = std::max( (float){}, (float){});", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> MaxNode::GetPinInfo()
{
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A", isPinNumeric, FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B", isPinNumeric, FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

void AddMathNodes(const std::unique_ptr<NodeEditor>& editor) {
	editor->AddNodeType<AdditionNode>();
	editor->AddNodeType<MultiplyNode>();
	editor->AddNodeType<SubtractNode>();
	editor->AddNodeType<DivideNode>();
	editor->AddNodeType<ModuloNode>();
	editor->AddNodeType<AbsoluteNode>();
	editor->AddNodeType<SineNode>();
	editor->AddNodeType<ExponentNode>();
	editor->AddNodeType<MappingNode>();
	editor->AddNodeType<TangentNode>();
	editor->AddNodeType<CosineNode>();
	editor->AddNodeType<SquareRootNode>();
	editor->AddNodeType<RoundNode>();
	editor->AddNodeType<FloorNode>();
	editor->AddNodeType<CeilNode>();
	editor->AddNodeType<TruncNode>();
	editor->AddNodeType<ClampNode>();
	editor->AddNodeType<MinNode>();
	editor->AddNodeType<MaxNode>();
}

