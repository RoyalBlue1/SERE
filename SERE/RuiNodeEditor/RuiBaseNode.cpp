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
	float val;
	if(inPin(id)->isConnected()&&inPin(id)->getLink().lock()->left()->getDataType() == typeid(IntVariable))
	{
		IntVariable val = getInVal<IntVariable>(id).value;
		return FloatVariable(val.value,val.name);
	}
	else
		return getInVal<FloatVariable>(id).value;

}

bool isPinNumeric(const std::type_info& out, const std::type_info& in) {
	return (out == typeid(IntVariable))||(out == typeid(FloatVariable));
}