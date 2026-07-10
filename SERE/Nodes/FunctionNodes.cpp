#include "FunctionNodes.h"

#include <algorithm>
#include <cctype>
#include <format>
#include <type_traits>
#include <variant>
#include "Imgui/imgui_stdlib.h"

SetNoRenderNode::SetNoRenderNode(RenderInstance& rend, ImFlow::StyleManager& style)
	: RuiBaseNode(name, category, GetPinInfo(), rend, style)
{}

SetNoRenderNode::SetNoRenderNode(RenderInstance& rend, ImFlow::StyleManager& style,
	rapidjson::GenericObject<false, rapidjson::Value> obj) : SetNoRenderNode(rend, style)
{}

void SetNoRenderNode::draw()
{
	const BoolVariable& value = getInVal<BoolVariable>("Value");
	ImGui::Text("Value: %s", value.value ? "true" : "false");
}

void SetNoRenderNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,
	rapidjson::Document::AllocatorType& allocator)
{
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void SetNoRenderNode::Export(RuiExportPrototype& proto)
{
	const BoolVariable& value = getInVal<BoolVariable>("Value");
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.dependencys = { value.name };
	ele.identifier = Variable::UniqueName();
	ele.callback = [value](RuiExportPrototype& proto) {
		proto.codeLines.push_back(std::format(
			"if ({}) {{\n\tfuncs->setNoRender(inst);\n\treturn;\n}}",
			value.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> SetNoRenderNode::GetPinInfo()
{
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<BoolVariable>>(
		"Value", ImFlow::ConnectionFilter::SameType(), BoolVariable(false)));
	return info;
}


static float getRandomFloat(float min, float max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}

RandomFloatNode::RandomFloatNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	randomFloat = getRandomFloat(0.0f, 1.0f);
	getOut<FloatVariable>("Out")->behaviour([this, outName]() {
		return FloatVariable(randomFloat, outName);
		});
}

RandomFloatNode::RandomFloatNode(RenderInstance& prot, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj) : RandomFloatNode(prot, styles)
{

}

void RandomFloatNode::draw()
{
	ImGui::Text("Value: %f", randomFloat);
}

void RandomFloatNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator)
{
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void RandomFloatNode::Export(RuiExportPrototype& proto)
{
	auto out = getOut<FloatVariable>("Out")->val();
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.identifier = out.name;
	ele.callback = [out](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = randomFloat(inst);", out.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = randomFloat(inst);", out.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> RandomFloatNode::GetPinInfo()
{
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Out"));
	return info;
}


ProjectionNode::ProjectionNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<Float2Variable>("Res")->behaviour([this, outName]() {
		return Float2Variable(Vector2(0.5f, 0.5f), name);
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
			proto.codeLines.push_back(std::format("{} = project3d(funcs,globals,inst,&{});", out.GetFormattedName(proto), pos.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("float {} = project3d(funcs,globals,inst,&{});", out.GetFormattedName(proto), pos.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ProjectionNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<Float3Variable>>("Position", ImFlow::ConnectionFilter::SameType(), Float3Variable(Vector3(0.f, 0.f, 0.f))));
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float2Variable>>("Res"));
	return info;
}

static std::string ToUpper(std::string value)
{
	std::transform(value.begin(), value.end(), value.begin(), [](unsigned char character) {
		return static_cast<char>(std::toupper(character));
		});
	return value;
}

ToUpperNode::ToUpperNode(RenderInstance& prot, ImFlow::StyleManager& styles)
	: RuiBaseNode(name, category, GetPinInfo(), prot, styles)
{
	std::string outName = Variable::UniqueName();
	getOut<StringVariable>("Out")->behaviour([this, outName]() {
		const StringVariable& input = getInVal<StringVariable>("In");
		return StringVariable(ToUpper(input.value), input.IsConstant() ? "" : outName);
		});
}

ToUpperNode::ToUpperNode(RenderInstance& prot, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj)
	: ToUpperNode(prot, styles)
{}

void ToUpperNode::draw()
{}

void ToUpperNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator)
{
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void ToUpperNode::Export(RuiExportPrototype& proto)
{
	const StringVariable& out = getOut<StringVariable>("Out")->val();
	const StringVariable& input = getInVal<StringVariable>("In");
	if (out.IsConstant())
		return;

	ExportElement<std::string> element;
#if _DEBUG
	element.sourceNodeName = typeid(*this).name();
#endif
	element.dependencys = { input.name };
	element.identifier = out.name;
	element.callback = [out, input](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = funcs->toUpper(inst, {});", out.GetFormattedName(proto), input.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("const char* {} = funcs->toUpper(inst, {});", out.GetFormattedName(proto), input.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(element);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ToUpperNode::GetPinInfo()
{
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<StringVariable>>(
		"In", ImFlow::ConnectionFilter::SameType(), StringVariable("Default Text")));
	info.push_back(std::make_shared<ImFlow::OutPinProto<StringVariable>>("Out"));
	return info;
}


using LocalizeArgument = std::variant<BoolVariable, IntVariable, FloatVariable, StringVariable>;

LocalizeArgument GetLocalizeArgument(ImFlow::BaseNode& node, const std::string& id)
{
	ImFlow::Pin* pin = node.inPin(id);
	if (!pin->isConnected())
		return node.getInVal<StringVariable>(id);

	std::any value = pin->getLink().lock()->left()->valueAny();
	if (value.type() == typeid(BoolVariable))
		return std::any_cast<BoolVariable>(value);
	if (value.type() == typeid(IntVariable))
		return std::any_cast<IntVariable>(value);
	if (value.type() == typeid(FloatVariable))
		return std::any_cast<FloatVariable>(value);
	return std::any_cast<StringVariable>(value);
}

std::string GetFormattedLocalizeArgument(const LocalizeArgument& argument, RuiExportPrototype& proto)
{
	return std::visit([&proto](const auto& value) { return value.GetFormattedName(proto); }, argument);
}

std::string GetLocalizeArgumentName(const LocalizeArgument& argument)
{
	return std::visit([](const auto& value) { return value.name; }, argument);
}

static char IntegerPresentation(const std::string& options)
{
	if (options.size() < 3 || options.front() != '{' || options.back() != '}')
		return '\0';
	for (auto it = options.rbegin() + 1; it != options.rend(); ++it) {
		if (*it == 'd' || *it == 'x' || *it == 'X' || *it == 'o' ||
			*it == 'b' || *it == 'B' || *it == 'c')
			return *it;
		if ((*it >= 'a' && *it <= 'z') || (*it >= 'A' && *it <= 'Z'))
			break;
	}
	return '\0';
}

static std::string PrintfIntegerSpecifier(const std::string& options, char presentation)
{
	std::string modifiers;
	const size_t colon = options.find(':');
	if (colon != std::string::npos && options.size() > colon + 2) {
		const size_t presentationIndex = options.size() - 2;
		for (size_t index = colon + 1; index < presentationIndex; ++index) {
			const char character = options[index];
			if ((character >= '0' && character <= '9') || character == '+' ||
				character == '-' || character == ' ' || character == '#')
				modifiers += character;
		}
	}

	std::string specifier = "%" + modifiers;
	switch (presentation) {
	case 'x': return specifier + 'x';
	case 'X': return specifier + 'X';
	case 'o': return specifier + 'o';
	case 'c': return specifier + 'c';
	default: return specifier + 'i';
	}
}

static char FloatPresentation(const std::string& options)
{
	if (options.size() < 3 || options.front() != '{' || options.back() != '}')
		return '\0';
	const char presentation = options[options.size() - 2];
	switch (presentation) {
	case 'a': case 'A': case 'e': case 'E':
	case 'f': case 'F': case 'g': case 'G':
		return presentation;
	default:
		return '\0';
	}
}

static std::string PrintfFloatSpecifier(const std::string& options)
{
	const char presentation = FloatPresentation(options);
	std::string modifiers;
	const size_t colon = options.find(':');
	if (colon != std::string::npos) {
		const size_t end = presentation == '\0' ? options.size() - 1 : options.size() - 2;
		for (size_t index = colon + 1; index < end; ++index) {
			const char character = options[index];
			if ((character >= '0' && character <= '9') || character == '.' ||
				character == '+' || character == '-' || character == ' ' || character == '#')
				modifiers += character;
		}
	}
	return "%" + modifiers + (presentation == '\0' ? 'f' : presentation);
}

LocalizeNode::LocalizeNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<StringVariable>("Out")->behaviour([this, outName]() {
		std::string out;
		Print(fmt, out);
		return StringVariable(out, outName);
		});

}

LocalizeNode::LocalizeNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :LocalizeNode(rend, style) {
	if (obj.HasMember("Format") && obj["Format"].IsString())
		fmt = obj["Format"].GetString();
}


std::string LocalizeNode::Format(int valId, std::string options) {
	std::string id = std::format("Val {}", valId);
	if (inPin(id)->getLink().expired())return "***";
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
	for (size_t i = 0; i < fmt.size(); i++) {

		if (fmt[i] == '{') {
			if ((i + 1) >= fmt.size()) {
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
				if (varId < 6)
					f = Format(varId++, options);
				out += f;
			}
		}
		else if (fmt[i] == '}') {
			if ((i + 1) < fmt.size() && fmt[i + 1] == '}') {
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
	return true;
}

int LocalizeNode::GetPrintfString(std::string& out) {

	int varId = 1;
	for (size_t i = 0; i < fmt.size(); i++) {

		if (fmt[i] == '{') {
			if ((i + 1) >= fmt.size()) {
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
				if (varId > 5) {
					out = "***";
					return -1;
				}

				ImFlow::Pin* in = inPin(std::format("Val {}", varId++));
				std::string f = "%s";
				if (!in->getLink().expired()) {
					if (in->getLink().lock()->left()->getDataType() == typeid(BoolVariable)) {
						f = "%i";
					}
					else if (in->getLink().lock()->left()->getDataType() == typeid(IntVariable)) {
						f = "%i";
					}
					else if (in->getLink().lock()->left()->getDataType() == typeid(FloatVariable)) {
						f = "%f";
					}
					else if (in->getLink().lock()->left()->getDataType() == typeid(StringVariable)) {
						f = "%s";
					}
				}
				out += f;

			}
		}
		else if (fmt[i] == '}') {
			if ((i + 1) < fmt.size() && fmt[i + 1] == '}') {
				out += '}';
				i++;
			}
			else {
				out = "***";
				return -1;
			}
		}
		else {
			if (fmt[i] == '%')
				out += "%%";
			else
				out += fmt[i];
		}
	}
	return varId - 1;
}

void LocalizeNode::draw() {

	ImGui::PushItemWidth(120.f);
	ImGui::InputText("Format", &fmt);

	std::string out = "";
	Print(fmt, out);

	ImGui::Text("\"%s\"", out.c_str());
	ImGui::PopItemWidth();

}

void LocalizeNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	obj.AddMember("Format", fmt, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void LocalizeNode::Export(RuiExportPrototype& proto) {
	auto out = getOut<StringVariable>("Out")->val();
	std::string printfString;
	const int argumentCount = GetPrintfString(printfString);
	if (argumentCount < 0)
		return;

	std::vector<LocalizeArgument> arguments;
	arguments.reserve(argumentCount);
	for (int index = 1; index <= argumentCount; ++index)
		arguments.push_back(GetLocalizeArgument(*this, std::format("Val {}", index)));

	ExportElement<std::string> ele;
	ele.identifier = out.name;
	for (const LocalizeArgument& argument : arguments)
		ele.dependencys.insert(GetLocalizeArgumentName(argument));
	ele.callback = [out, printfString, arguments](RuiExportPrototype& proto) {
		const StringVariable format(printfString);
		std::string call = std::format("funcs->Localize(inst, {}", format.GetFormattedName(proto));
		for (const LocalizeArgument& argument : arguments)
			call += std::format(", {}", GetFormattedLocalizeArgument(argument, proto));
		call += ')';

		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = {};", out.GetFormattedName(proto), call));
		else
			proto.codeLines.push_back(std::format("const char* {} = {};", out.GetFormattedName(proto), call));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> LocalizeNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	static std::function<bool(const std::type_info&, const std::type_info&)> localizeArg = [](const std::type_info& out, const std::type_info& in) {
		return out == typeid(BoolVariable) ||
			out == typeid(IntVariable) ||
			out == typeid(FloatVariable) ||
			out == typeid(StringVariable);
		};

	info.push_back(std::make_shared<ImFlow::InPinProto<StringVariable>>("Val 1", localizeArg, StringVariable("")));
	info.push_back(std::make_shared<ImFlow::InPinProto<StringVariable>>("Val 2", localizeArg, StringVariable("")));
	info.push_back(std::make_shared<ImFlow::InPinProto<StringVariable>>("Val 3", localizeArg, StringVariable("")));
	info.push_back(std::make_shared<ImFlow::InPinProto<StringVariable>>("Val 4", localizeArg, StringVariable("")));
	info.push_back(std::make_shared<ImFlow::InPinProto<StringVariable>>("Val 5", localizeArg, StringVariable("")));

	info.push_back(std::make_shared<ImFlow::OutPinProto<StringVariable>>("Out"));
	return info;
}

PrintFNode::PrintFNode(RenderInstance& rend, ImFlow::StyleManager& style)
	: RuiBaseNode(name, category, GetPinInfo(), rend, style)
{
	const std::string outName = Variable::UniqueName();
	getOut<StringVariable>("Out")->behaviour([this, outName]() {
		std::string value;
		Print(value);
		return StringVariable(value, outName);
		});
	AddArgumentPin();
}

PrintFNode::PrintFNode(RenderInstance& rend, ImFlow::StyleManager& style,
	rapidjson::GenericObject<false, rapidjson::Value> obj)
	: RuiBaseNode(name, category, GetPinInfo(), rend, style)
{
	const std::string outName = Variable::UniqueName();
	getOut<StringVariable>("Out")->behaviour([this, outName]() {
		std::string value;
		Print(value);
		return StringVariable(value, outName);
		});

	if (obj.HasMember("Format") && obj["Format"].IsString())
		fmt = obj["Format"].GetString();

	size_t savedArgumentCount = 1;
	if (obj.HasMember("ArgumentCount") && obj["ArgumentCount"].IsUint64())
		savedArgumentCount = static_cast<size_t>(obj["ArgumentCount"].GetUint64());
	for (size_t index = 0; index < savedArgumentCount; ++index)
		AddArgumentPin();
}

void PrintFNode::AddArgumentPin()
{
	static std::function<bool(const std::type_info&, const std::type_info&)> printfArgument =
		[](const std::type_info& out, const std::type_info&) {
			return out == typeid(BoolVariable) ||
				out == typeid(IntVariable) ||
				out == typeid(FloatVariable) ||
				out == typeid(StringVariable);
		};

	++argumentCount;
	auto pin = std::make_shared<ImFlow::InPinProto<StringVariable>>(
		std::format("Arg {}", argumentCount), printfArgument, StringVariable(""));
	pin->CreatePin(this, styles);
}

std::string PrintFNode::FormatArgument(size_t index, const std::string& options)
{
	const std::string id = std::format("Arg {}", index);
	if (!inPin(id)->isConnected())
		return "***";

	try {
		return std::visit([&options](const auto& value) {
			if constexpr (std::is_same_v<std::decay_t<decltype(value)>, FloatVariable>) {
				if (IntegerPresentation(options) != '\0') {
					int converted = static_cast<int>(value.value);
					return std::vformat(options, std::make_format_args(converted));
				}
			}
			return std::vformat(options, std::make_format_args(value.value));
			}, GetLocalizeArgument(*this, id));
	}
	catch (const std::format_error&) {
		return "***";
	}
}

bool PrintFNode::Print(std::string& out)
{
	size_t argumentIndex = 1;
	for (size_t index = 0; index < fmt.size(); ++index) {
		if (fmt[index] == '{') {
			if (index + 1 >= fmt.size()) {
				out = "***";
				return false;
			}
			if (fmt[index + 1] == '{') {
				out += '{';
				++index;
				continue;
			}

			std::string options;
			bool closed = false;
			for (; index < fmt.size(); ++index) {
				options += fmt[index];
				if (fmt[index] == '}') {
					closed = true;
					break;
				}
			}
			if (!closed || argumentIndex > argumentCount) {
				out = "***";
				return false;
			}
			out += FormatArgument(argumentIndex++, options);
		}
		else if (fmt[index] == '}') {
			if (index + 1 < fmt.size() && fmt[index + 1] == '}') {
				out += '}';
				++index;
			}
			else {
				out = "***";
				return false;
			}
		}
		else {
			out += fmt[index];
		}
	}
	return true;
}

int PrintFNode::GetPrintfString(std::string& out, std::vector<bool>* floatArgumentsAsInt)
{
	if (floatArgumentsAsInt)
		floatArgumentsAsInt->clear();
	size_t argumentIndex = 1;
	for (size_t index = 0; index < fmt.size(); ++index) {
		if (fmt[index] == '{') {
			if (index + 1 >= fmt.size())
				return -1;
			if (fmt[index + 1] == '{') {
				out += '{';
				++index;
				continue;
			}

			std::string options;
			bool closed = false;
			for (; index < fmt.size(); ++index) {
				options += fmt[index];
				if (fmt[index] == '}') {
					closed = true;
					break;
				}
			}
			if (!closed || argumentIndex > argumentCount)
				return -1;

			ImFlow::Pin* input = inPin(std::format("Arg {}", argumentIndex++));
			const char integerPresentation = IntegerPresentation(options);
			const bool floatAsInt = input->isConnected() &&
				input->getLink().lock()->left()->getDataType() == typeid(FloatVariable) &&
				integerPresentation != '\0';
			if (floatArgumentsAsInt)
				floatArgumentsAsInt->push_back(floatAsInt);
			if (!input->isConnected() || input->getLink().lock()->left()->getDataType() == typeid(StringVariable))
				out += "%s";
			else if (integerPresentation != '\0')
				out += PrintfIntegerSpecifier(options, integerPresentation);
			else if (input->getLink().lock()->left()->getDataType() == typeid(FloatVariable))
				out += PrintfFloatSpecifier(options);
			else
				out += "%i";
		}
		else if (fmt[index] == '}') {
			if (index + 1 >= fmt.size() || fmt[index + 1] != '}')
				return -1;
			out += '}';
			++index;
		}
		else if (fmt[index] == '%') {
			out += "%%";
		}
		else {
			out += fmt[index];
		}
	}
	return static_cast<int>(argumentIndex - 1);
}

void PrintFNode::draw()
{
	ImGui::PushItemWidth(160.f);
	ImGui::InputText("Format", &fmt);
	ImGui::PopItemWidth();

	const bool canRemove = argumentCount > 0 &&
		!inPin(std::format("Arg {}", argumentCount))->isConnected();
	ImGui::BeginDisabled(!canRemove);
	if (ImGui::SmallButton("-")) {
		const std::string id = std::format("Arg {}", argumentCount);
		dropIN(id);
		--argumentCount;
	}
	ImGui::EndDisabled();
	ImGui::SameLine();
	if (ImGui::SmallButton("+"))
		AddArgumentPin();

	std::string preview;
	Print(preview);
	ImGui::Text("\"%s\"", preview.c_str());
}

void PrintFNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj,
	rapidjson::Document::AllocatorType& allocator)
{
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	obj.AddMember("Format", fmt, allocator);
	obj.AddMember("ArgumentCount", static_cast<uint64_t>(argumentCount), allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void PrintFNode::Export(RuiExportPrototype& proto)
{
	const StringVariable out = getOut<StringVariable>("Out")->val();
	std::string printfString;
	std::vector<bool> floatArgumentsAsInt;
	const int usedArgumentCount = GetPrintfString(printfString, &floatArgumentsAsInt);
	if (usedArgumentCount < 0)
		return;

	std::vector<LocalizeArgument> arguments;
	arguments.reserve(usedArgumentCount);
	for (int index = 1; index <= usedArgumentCount; ++index)
		arguments.push_back(GetLocalizeArgument(*this, std::format("Arg {}", index)));

	ExportElement<std::string> element;
#if _DEBUG
	element.sourceNodeName = typeid(*this).name();
#endif
	element.identifier = out.name;
	for (const LocalizeArgument& argument : arguments)
		element.dependencys.insert(GetLocalizeArgumentName(argument));
	element.callback = [out, printfString, arguments, floatArgumentsAsInt](RuiExportPrototype& proto) {
		const StringVariable format(printfString);
		std::string call = std::format("funcs->printf(inst, {}", format.GetFormattedName(proto));
		for (size_t index = 0; index < arguments.size(); ++index) {
			std::string value = GetFormattedLocalizeArgument(arguments[index], proto);
			if (floatArgumentsAsInt[index])
				value = std::format("static_cast<int>({})", value);
			call += std::format(", {}", value);
		}
		call += ')';
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = {};", out.GetFormattedName(proto), call));
		else
			proto.codeLines.push_back(std::format("const char* {} = {};", out.GetFormattedName(proto), call));
		};
	proto.codeElements.push_back(element);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> PrintFNode::GetPinInfo()
{
	return { std::make_shared<ImFlow::OutPinProto<StringVariable>>("Out") };
}


void AddFunctionNodes(NodeEditor& editor)
{
	editor.AddNodeType<SetNoRenderNode>();
	editor.AddNodeType<RandomFloatNode>();
	editor.AddNodeType<ProjectionNode>();
	editor.AddNodeType<ToUpperNode>();
	editor.AddNodeType<LocalizeNode>();
	editor.AddNodeType<PrintFNode>();

}