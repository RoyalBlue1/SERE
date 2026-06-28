#include "RuiNodeEditor/RuiBaseNode.h"
#include "RuiNodeEditor/RuiExportPrototype.h"

std::string Variable::GetFormattedName(RuiExportPrototype& proto) const {
	if(IsConstant())
		return Literal();
	bool isInData = false;
	if (proto.arguments.contains(name)) {
		isInData = true;
	}
	if (proto.varsInDataStruct.contains(name)) {
		isInData = true;
	}
	if(isInData)
		return std::format("data->{}",name);
	return name;
}

FloatVariable RuiBaseNode::getInNumeric(const char* id) {
	bool isConnected = inPin(id)->isConnected();
	if(isConnected&&inPin(id)->getLink().lock()->left()->getDataType() == typeid(IntVariable))
	{
		IntVariable val = getInVal<IntVariable>(id);
		return FloatVariable(val.value,val.name);
	}
	else
		return getInVal<FloatVariable>(id);

}

MathVariable RuiBaseNode::getInMath(const char* id) {
	if (!inPin(id)->isConnected())
		return getInVal<MathVariable>(id);

	std::any value = inPin(id)->getLink().lock()->left()->valueAny();
	if (value.type() == typeid(IntVariable)) {
		IntVariable val = std::any_cast<IntVariable>(value);
		return MathVariable(FloatVariable((float)val.value, val.name));
	}
	if (value.type() == typeid(FloatVariable))
		return MathVariable(std::any_cast<FloatVariable>(value));
	if (value.type() == typeid(Float2Variable))
		return MathVariable(std::any_cast<Float2Variable>(value));
	if (value.type() == typeid(Float3Variable))
		return MathVariable(std::any_cast<Float3Variable>(value));
	if (value.type() == typeid(MathVariable))
		return std::any_cast<MathVariable>(value);
	return getInVal<MathVariable>(id);
}

bool isPinNumeric(const std::type_info& out, const std::type_info& in) {
	return (out == typeid(IntVariable))||(out == typeid(FloatVariable));
}

bool isPinMath(const std::type_info& out, const std::type_info& in) {
	return out == typeid(IntVariable) ||
		out == typeid(FloatVariable) ||
		out == typeid(Float2Variable) ||
		out == typeid(Float3Variable) ||
		out == typeid(MathVariable);
}
