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