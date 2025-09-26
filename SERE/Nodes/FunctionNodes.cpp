#include "FunctionNodes.h"
#include <format>
#include "Imgui/imgui_stdlib.h"


ToUpperNode::ToUpperNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	std::string outName = Variable::UniqueName();
	getOut<StringVariable>("Out")->behaviour([this,outName]() {
		std::string out = getInVal<StringVariable>("In").value;
		std::transform(out.begin(),out.end(),out.begin(),::toupper);
		return StringVariable(out,outName);
	});

}

ToUpperNode::ToUpperNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):ToUpperNode(rend,style) {

}

void ToUpperNode::draw() {

	std::string out = getInVal<StringVariable>("In").value;
	std::transform(out.begin(),out.end(),out.begin(),::toupper);
	ImGui::Text("Out: \"%s\"",out.c_str());


}

void ToUpperNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void ToUpperNode::Export(RuiExportPrototype& proto) {
	auto out = getOut<StringVariable>("Out")->val();
	auto in = getInVal<StringVariable>("In");
	ExportElement<std::string> ele;
	ele.identifier = out.name;
	ele.dependencys.insert(in.UniqueName());
	ele.callback = [out,in](RuiExportPrototype& proto) {
		if(proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = funcs->ToUpper(inst,{});",out.GetFormattedName(proto),in.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("const char* {} = funcs->ToUpper(inst,{});",out.GetFormattedName(proto),in.GetFormattedName(proto)));
	};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ToUpperNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<StringVariable>>("In",ImFlow::ConnectionFilter::SameType(),StringVariable("")));
	info.push_back(std::make_shared<ImFlow::OutPinProto<StringVariable>>("Out"));
	return info;
}

LocalizeNode::LocalizeNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	std::string outName = Variable::UniqueName();
	getOut<StringVariable>("Out")->behaviour([this,outName]() {
		std::string out = getInVal<StringVariable>("In").value;
		std::transform(out.begin(),out.end(),out.begin(),::toupper);
		return StringVariable(out,outName);
	});

}

LocalizeNode::LocalizeNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):LocalizeNode(rend,style) {

}


std::string LocalizeNode::Format(int valId, std::string options) {
	std::string id = std::format("Val {}",valId);
	if(inPin(id)->getLink().expired())return "***";
	try {
		if (inPin(id)->getLink().lock()->left()->getDataType() == typeid(IntVariable)) {
			int val = getInVal<IntVariable>(id).value;
			return std::vformat(options, std::make_format_args(val));
		}
		else if (inPin(id)->getLink().lock()->left()->getDataType() == typeid(BoolVariable)) {
			bool val = getInVal<BoolVariable>(id).value;
			return std::vformat(options, std::make_format_args(val));
		}
		else if (inPin(id)->getLink().lock()->left()->getDataType() == typeid(FloatVariable)) {
			float val = getInVal<FloatVariable>(id).value;
			return std::vformat(options, std::make_format_args(val));
		}
		else if (inPin(id)->getLink().lock()->left()->getDataType() == typeid(Float2Variable)) {
			Vector2 val = getInVal<Float2Variable>(id).value;
			return std::format("<{}, {}>",
				std::vformat(options, std::make_format_args(val.x)),
				std::vformat(options, std::make_format_args(val.y)));
		}
		else if (inPin(id)->getLink().lock()->left()->getDataType() == typeid(Float3Variable)) {
			Vector3 val = getInVal<Float3Variable>(id).value;
			return std::format("<{}, {}, {}>",
				std::vformat(options, std::make_format_args(val.x)),
				std::vformat(options, std::make_format_args(val.y)),
				std::vformat(options, std::make_format_args(val.z)));
		}
		else if (inPin(id)->getLink().lock()->left()->getDataType() == typeid(ColorVariable)) {
			Color val = getInVal<ColorVariable>(id).value;
			return std::format("<{}, {}, {}, {}>",
				std::vformat(options, std::make_format_args(val.red)),
				std::vformat(options, std::make_format_args(val.green)),
				std::vformat(options, std::make_format_args(val.blue)),
				std::vformat(options, std::make_format_args(val.alpha)));
		}
		else if (inPin(id)->getLink().lock()->left()->getDataType() == typeid(StringVariable)) {
			std::string val = getInVal<StringVariable>(id).value;
			return std::vformat(options, std::make_format_args(val));
		}
		else if (inPin(id)->getLink().lock()->left()->getDataType() == typeid(AssetVariable)) {
			uint32_t val = getInVal<AssetVariable>(id).hash;
			return std::vformat(options, std::make_format_args(val));
		}
	}
	catch (std::format_error& e) {
		return "***";
	}
	return "***";
}

bool LocalizeNode::Print(const std::string& fmt, std::string& out) {

	int varId = 1;
	for (size_t i = 0;i<fmt.size();i++) {

		if (fmt[i] == '{') {
			if((i+1)>=fmt.size()){
				out = "***";
				return false;
			}
			if (fmt[i + 1] == '{') {
				out += '{';
				i++;
			}
			else {
				std::string options = "";
				bool validOptions = false;
				for (i; i < fmt.size(); i++) {
					options += fmt[i];
					if (fmt[i] == '}') {
						validOptions = true;
						break;
					}
				}
				if (!validOptions) {
					out = "***";
					return false;
				}
				std::string f = "***";
				if(varId<6)
				 f = Format(varId++,options);
				out += f;
			}
		}
		else if (fmt[i] == '}') {
			if (fmt[i+1] == '}') {
				out += '}';
				i++;
			}
			else {
				out = "***";
				return false;
			}
		}
		else {
			out += fmt[i];
		}
	}
}

int LocalizeNode::GetPrintfString(std::string& out) {

	int varId = 1;
	for (size_t i = 0;i<fmt.size();i++) {

		if (fmt[i] == '{') {
			if((i+1)>=fmt.size()){
				out = "***";
				return -1;
			}
			if (fmt[i + 1] == '{') {
				out += '{';
				i++;
			}
			else {
				std::string options = "";
				bool validOptions = false;
				for (i; i < fmt.size(); i++) {
					options += fmt[i];
					if (fmt[i] == '}') {
						validOptions = true;
						break;
					}
				}
				if (!validOptions) {
					out = "***";
					return -1;
				}
				ImFlow::Pin* in = inPin(std::format("Var {}",varId++));
				std::string f = "%s";
				if (!in->getLink().expired()) {
					if (in->getLink().lock()->left()->getDataType() == typeid(BoolVariable)) {
						f = "%s";
					}else if (in->getLink().lock()->left()->getDataType() == typeid(IntVariable)) {
						f = "%i";
					}else if (in->getLink().lock()->left()->getDataType() == typeid(FloatVariable)) {
						f = "%f";
					}else if (in->getLink().lock()->left()->getDataType() == typeid(StringVariable)) {
						f = "%s";
					}
				}
				out += f;

			}
		}
		else if (fmt[i] == '}') {
			if (fmt[i+1] == '}') {
				out += '}';
				i++;
			}
			else {
				out = "***";
				return -1;
			}
		}
		else {
			out += fmt[i];
		}
	}
	return varId - 1;
}

void LocalizeNode::draw() {

	ImGui::PushItemWidth(120.f);
	ImGui::InputText("Format",&fmt);

	std::string out = "";
	Print(fmt,out);

	ImGui::Text("\"%s\"",out.c_str());
	ImGui::PopItemWidth();

}

void LocalizeNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void LocalizeNode::Export(RuiExportPrototype& proto) {
	auto out = getOut<StringVariable>("Out")->val();
	auto in = getInVal<StringVariable>("In");
	ExportElement<std::string> ele;
	ele.identifier = out.name;
	ele.dependencys.insert(in.UniqueName());
	ele.callback = [out,in](RuiExportPrototype& proto) {
		if(proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = funcs->Localize(inst,{});",out.GetFormattedName(proto),in.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("const char* {} = funcs->Localize(inst,{});",out.GetFormattedName(proto),in.GetFormattedName(proto)));
	};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> LocalizeNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	static std::function<bool(const std::type_info&, const std::type_info&)> localizeArg = [](const std::type_info& out, const std::type_info& in){ 
		return out == typeid(BoolVariable) ||
			out == typeid(IntVariable) ||
			out == typeid(FloatVariable) ||
			out == typeid(StringVariable); 
	}; 

	info.push_back(std::make_shared<ImFlow::InPinProto<StringVariable>>("Val 1",localizeArg,StringVariable("")));
	info.push_back(std::make_shared<ImFlow::InPinProto<StringVariable>>("Val 2",localizeArg,StringVariable("")));
	info.push_back(std::make_shared<ImFlow::InPinProto<StringVariable>>("Val 3",localizeArg,StringVariable("")));
	info.push_back(std::make_shared<ImFlow::InPinProto<StringVariable>>("Val 4",localizeArg,StringVariable("")));
	info.push_back(std::make_shared<ImFlow::InPinProto<StringVariable>>("Val 5",localizeArg,StringVariable("")));

	info.push_back(std::make_shared<ImFlow::OutPinProto<StringVariable>>("Out"));
	return info;
}

void AddFunctionNodes(NodeEditor& editor) {
	editor.AddNodeType<ToUpperNode>();
	editor.AddNodeType<LocalizeNode>();
}