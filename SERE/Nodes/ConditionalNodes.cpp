

#include "Nodes/ConditionalNodes.h"
#include <type_traits>
#include <variant>

namespace {
enum class ConditionalValueType {
	Invalid,
	Int,
	Float,
	Float2,
	Float3,
	Size,
	Color
};

struct ConditionalValue {
	std::variant<IntVariable, FloatVariable, Float2Variable, Float3Variable, TransformSize, ColorVariable> value;

	ConditionalValue() :value(FloatVariable(0.f)) {}
	ConditionalValue(const IntVariable& val) :value(val) {}
	ConditionalValue(const FloatVariable& val) :value(val) {}
	ConditionalValue(const Float2Variable& val) :value(val) {}
	ConditionalValue(const Float3Variable& val) :value(val) {}
	ConditionalValue(const TransformSize& val) :value(val) {}
	ConditionalValue(const ColorVariable& val) :value(val) {}

	ConditionalValueType Type() const {
		if (std::holds_alternative<IntVariable>(value))
			return ConditionalValueType::Int;
		if (std::holds_alternative<FloatVariable>(value))
			return ConditionalValueType::Float;
		if (std::holds_alternative<Float2Variable>(value))
			return ConditionalValueType::Float2;
		if (std::holds_alternative<Float3Variable>(value))
			return ConditionalValueType::Float3;
		if (std::holds_alternative<TransformSize>(value))
			return ConditionalValueType::Size;
		return ConditionalValueType::Color;
	}

	bool IsConstant() const {
		return std::visit([](const auto& val) { return val.IsConstant(); }, value);
	}

	std::string Name() const {
		return std::visit([](const auto& val) { return val.name; }, value);
	}

	std::string GetFormattedName(RuiExportPrototype& proto) const {
		return std::visit([&proto](const auto& val) { return val.GetFormattedName(proto); }, value);
	}
};

ConditionalValueType TypeFromInfo(const std::type_info& type) {
	if (type == typeid(IntVariable))
		return ConditionalValueType::Int;
	if (type == typeid(FloatVariable))
		return ConditionalValueType::Float;
	if (type == typeid(Float2Variable))
		return ConditionalValueType::Float2;
	if (type == typeid(Float3Variable))
		return ConditionalValueType::Float3;
	if (type == typeid(TransformSize))
		return ConditionalValueType::Size;
	if (type == typeid(ColorVariable))
		return ConditionalValueType::Color;
	if (type == typeid(ConditionalValue))
		return ConditionalValueType::Float;
	return ConditionalValueType::Invalid;
}

bool isPinConditionalValue(const std::type_info& out, const std::type_info& in) {
	return TypeFromInfo(out) != ConditionalValueType::Invalid;
}

ConditionalValue DefaultConditionalValue(ConditionalValueType type, bool trueBranch) {
	float value = trueBranch ? 1.f : 0.f;
	switch (type) {
	case ConditionalValueType::Int:
		return IntVariable(trueBranch ? 1 : 0);
	case ConditionalValueType::Float:
		return FloatVariable(value);
	case ConditionalValueType::Float2:
		return Float2Variable(value, value);
	case ConditionalValueType::Float3:
		return Float3Variable(value, value, value);
	case ConditionalValueType::Size:
		return TransformSize(_mm_set1_ps(value));
	case ConditionalValueType::Color:
		return trueBranch ? ColorVariable(1.f, 1.f, 1.f, 1.f) : ColorVariable(0.f, 0.f, 0.f, 0.f);
	default:
		return FloatVariable(value);
	}
}

ConditionalValue GetConditionalInput(ImFlow::BaseNode* node, const char* id, ConditionalValueType defaultType = ConditionalValueType::Invalid) {
	ImFlow::Pin* pin = node->inPin(id);
	if (!pin->isConnected()) {
		if (defaultType != ConditionalValueType::Invalid)
			return DefaultConditionalValue(defaultType, id[0] == 'B');
		return node->getInVal<ConditionalValue>(id);
	}

	std::any value = pin->getLink().lock()->left()->valueAny();
	if (value.type() == typeid(IntVariable))
		return std::any_cast<IntVariable>(value);
	if (value.type() == typeid(FloatVariable))
		return std::any_cast<FloatVariable>(value);
	if (value.type() == typeid(Float2Variable))
		return std::any_cast<Float2Variable>(value);
	if (value.type() == typeid(Float3Variable))
		return std::any_cast<Float3Variable>(value);
	if (value.type() == typeid(TransformSize))
		return std::any_cast<TransformSize>(value);
	if (value.type() == typeid(ColorVariable))
		return std::any_cast<ColorVariable>(value);
	if (value.type() == typeid(ConditionalValue))
		return std::any_cast<ConditionalValue>(value);

	return node->getInVal<ConditionalValue>(id);
}

ConditionalValueType ConnectedInputType(ImFlow::BaseNode* node, const char* id) {
	ImFlow::Pin* pin = node->inPin(id);
	if (!pin->isConnected())
		return ConditionalValueType::Invalid;
	return TypeFromInfo(pin->getLink().lock()->left()->getDataType());
}

ConditionalValueType ConnectedOutputType(ImFlow::BaseNode* node) {
	if (node->outPin("Int Res")->isConnected())
		return ConditionalValueType::Int;
	if (node->outPin("Float Res")->isConnected())
		return ConditionalValueType::Float;
	if (node->outPin("Vector2 Res")->isConnected())
		return ConditionalValueType::Float2;
	if (node->outPin("Vector3 Res")->isConnected())
		return ConditionalValueType::Float3;
	if (node->outPin("Size Res")->isConnected())
		return ConditionalValueType::Size;
	if (node->outPin("Color Res")->isConnected())
		return ConditionalValueType::Color;
	return ConditionalValueType::Invalid;
}

ConditionalValueType DefaultInputType(ImFlow::BaseNode* node) {
	ConditionalValueType outputType = ConnectedOutputType(node);
	if (outputType != ConditionalValueType::Invalid)
		return outputType;

	ConditionalValueType bType = ConnectedInputType(node, "B");
	ConditionalValueType cType = ConnectedInputType(node, "C");
	if (bType != ConditionalValueType::Invalid && cType == ConditionalValueType::Invalid)
		return bType;
	if (cType != ConditionalValueType::Invalid && bType == ConditionalValueType::Invalid)
		return cType;

	return ConditionalValueType::Invalid;
}

ConditionalValueType ExpectedInputType(ImFlow::BaseNode* node, const std::string& pinName) {
	ConditionalValueType outputType = ConnectedOutputType(node);
	if (outputType != ConditionalValueType::Invalid)
		return outputType;

	if (pinName == "B")
		return ConnectedInputType(node, "C");
	if (pinName == "C")
		return ConnectedInputType(node, "B");

	return ConditionalValueType::Invalid;
}

bool IsConditionalOutputName(const std::string& pinName) {
	return pinName == "Int Res" ||
		pinName == "Float Res" ||
		pinName == "Vector2 Res" ||
		pinName == "Vector3 Res" ||
		pinName == "Size Res" ||
		pinName == "Color Res";
}

const char* ConditionalTypeName(ConditionalValueType type) {
	switch (type) {
	case ConditionalValueType::Int: return "Integer";
	case ConditionalValueType::Float: return "Float";
	case ConditionalValueType::Float2: return "Vector2";
	case ConditionalValueType::Float3: return "Vector3";
	case ConditionalValueType::Size: return "Size";
	case ConditionalValueType::Color: return "Color";
	default: return "Invalid";
	}
}

const char* ConditionalExportTypeName(ConditionalValueType type) {
	switch (type) {
	case ConditionalValueType::Int: return "int";
	case ConditionalValueType::Float: return "float";
	case ConditionalValueType::Float2: return "Vector2";
	case ConditionalValueType::Float3: return "Vector3";
	case ConditionalValueType::Size: return "__m128";
	case ConditionalValueType::Color: return "Color";
	default: return "float";
	}
}

template<typename T>
ConditionalValue WithName(const T& value, const std::string& name) {
	T named = value;
	named.name = name;
	return ConditionalValue(named);
}

ConditionalValue SelectConditionalValue(const BoolVariable& condition, const ConditionalValue& trueValue, const ConditionalValue& falseValue, const std::string& outName, bool* valid = nullptr) {
	bool isValid = trueValue.Type() == falseValue.Type();
	if (valid)
		*valid = isValid;
	if (!isValid)
		return ConditionalValue();

	std::string name = (condition.IsConstant() && trueValue.IsConstant() && falseValue.IsConstant()) ? "" : outName;
	return std::visit([&](const auto& trueTypedValue) -> ConditionalValue {
		using ValueT = std::decay_t<decltype(trueTypedValue)>;
		const auto& falseTypedValue = std::get<ValueT>(falseValue.value);
		return WithName(condition.value ? trueTypedValue : falseTypedValue, name);
		}, trueValue.value);
}

void DrawConditionalValue(const char* label, const ConditionalValue& value) {
	switch (value.Type()) {
	case ConditionalValueType::Int:
		ImGui::Text("%s %d", label, std::get<IntVariable>(value.value).value);
		break;
	case ConditionalValueType::Float:
		ImGui::Text("%s %f", label, std::get<FloatVariable>(value.value).value);
		break;
	case ConditionalValueType::Float2:
	{
		const Vector2& val = std::get<Float2Variable>(value.value).value;
		ImGui::Text("%s %f, %f", label, val.x, val.y);
		break;
	}
	case ConditionalValueType::Float3:
	{
		const Vector3& val = std::get<Float3Variable>(value.value).value;
		ImGui::Text("%s %f, %f, %f", label, val.x, val.y, val.z);
		break;
	}
	case ConditionalValueType::Size:
	{
		float size[4];
		_mm_storeu_ps(size, std::get<TransformSize>(value.value).size);
		ImGui::Text("%s %f, %f, %f, %f", label, size[0], size[1], size[2], size[3]);
		break;
	}
	case ConditionalValueType::Color:
	{
		const Color& val = std::get<ColorVariable>(value.value).value;
		ImGui::Text("%s %f, %f, %f, %f", label, val.red, val.green, val.blue, val.alpha);
		break;
	}
	default:
		ImGui::Text("%s Invalid", label);
		break;
	}
}

void UpdateConditionalOutputVisibility(ImFlow::BaseNode* node, ConditionalValueType type, bool valid, ConditionalValueType selectedOutputType = ConditionalValueType::Invalid, bool showAllOutputs = false) {
	ImFlow::Pin* intOut = node->outPin("Int Res");
	ImFlow::Pin* floatOut = node->outPin("Float Res");
	ImFlow::Pin* vector2Out = node->outPin("Vector2 Res");
	ImFlow::Pin* vector3Out = node->outPin("Vector3 Res");
	ImFlow::Pin* sizeOut = node->outPin("Size Res");
	ImFlow::Pin* colorOut = node->outPin("Color Res");

	if (showAllOutputs) {
		intOut->visible(true);
		floatOut->visible(true);
		vector2Out->visible(true);
		vector3Out->visible(true);
		sizeOut->visible(true);
		colorOut->visible(true);
		return;
	}

	bool hasSelectedOutput = selectedOutputType != ConditionalValueType::Invalid;
	ConditionalValueType displayType = hasSelectedOutput ? selectedOutputType : type;

	bool allowInt = (hasSelectedOutput || valid) && displayType == ConditionalValueType::Int;
	bool allowFloat = (hasSelectedOutput || valid) && displayType == ConditionalValueType::Float;
	bool allowVector2 = (hasSelectedOutput || valid) && displayType == ConditionalValueType::Float2;
	bool allowVector3 = (hasSelectedOutput || valid) && displayType == ConditionalValueType::Float3;
	bool allowSize = (hasSelectedOutput || valid) && displayType == ConditionalValueType::Size;
	bool allowColor = (hasSelectedOutput || valid) && displayType == ConditionalValueType::Color;

	bool hasSelectedVisibleOutput = (allowInt && intOut->isConnected()) ||
		(allowFloat && floatOut->isConnected()) ||
		(allowVector2 && vector2Out->isConnected()) ||
		(allowVector3 && vector3Out->isConnected()) ||
		(allowSize && sizeOut->isConnected()) ||
		(allowColor && colorOut->isConnected());

	intOut->visible(allowInt && (!hasSelectedVisibleOutput || intOut->isConnected()));
	floatOut->visible(allowFloat && (!hasSelectedVisibleOutput || floatOut->isConnected()));
	vector2Out->visible(allowVector2 && (!hasSelectedVisibleOutput || vector2Out->isConnected()));
	vector3Out->visible(allowVector3 && (!hasSelectedVisibleOutput || vector3Out->isConnected()));
	sizeOut->visible(allowSize && (!hasSelectedVisibleOutput || sizeOut->isConnected()));
	colorOut->visible(allowColor && (!hasSelectedVisibleOutput || colorOut->isConnected()));
}

template<typename T>
void PushConditionalExport(RuiExportPrototype& proto, const T& out, const BoolVariable& condition, const ConditionalValue& trueValue, const ConditionalValue& falseValue, ConditionalValueType type) {
	if (out.IsConstant())
		return;

	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = "ConditionalValueNode";
#endif
	ele.dependencys = { condition.name,trueValue.Name(),falseValue.Name() };
	ele.identifier = out.name;
	ele.callback = [out, condition, trueValue, falseValue, type](RuiExportPrototype& proto) {
		std::string outName = out.GetFormattedName(proto);
		std::string declaration = proto.varsInDataStruct.contains(out.name) ? "" : std::format("{} {};\n", ConditionalExportTypeName(type), outName);
		proto.codeLines.push_back(std::format(
			"{}{} = {}?{}:{};",
			declaration,
			outName,
			condition.GetFormattedName(proto),
			trueValue.GetFormattedName(proto),
			falseValue.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}
}

GreaterNode::GreaterNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<BoolVariable>("Res")->behaviour([this, outName]() {

		const FloatVariable& a = getInNumeric("A");
		const FloatVariable& b = getInNumeric("B");
		std::string name = (a.IsConstant() && b.IsConstant()) ? "" : outName;

		return BoolVariable(a.value > b.value, name);

		});

}

GreaterNode::GreaterNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :GreaterNode(rend, style) {}


void GreaterNode::draw() {
	const FloatVariable& a = getInNumeric("A");
	const FloatVariable& b = getInNumeric("B");
	ImGui::Text("A %f", a.value);
	ImGui::Text("B %f", b.value);
	if (a.value > b.value)
		ImGui::Text("Res True");
	else
		ImGui::Text("Res False");
}

void GreaterNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void GreaterNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<BoolVariable>("Res")->val();
	const auto& a = getInNumeric("A");
	const auto& b = getInNumeric("B");
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.dependencys = { a.name,b.name };
	ele.identifier = out.name;
	ele.callback = [out, a, b](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = {} > {};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("bool {} = {} > {};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> GreaterNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A", isPinNumeric, FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B", isPinNumeric, FloatVariable(2.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<BoolVariable>>("Res"));
	return info;
}

LessNode::LessNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<BoolVariable>("Res")->behaviour([this, outName]() {

		const FloatVariable& a = getInNumeric("A");
		const FloatVariable& b = getInNumeric("B");
		std::string name = (a.IsConstant() && b.IsConstant()) ? "" : outName;

		return BoolVariable(a.value < b.value, name);

		});

}

LessNode::LessNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :LessNode(rend, style) {}


void LessNode::draw() {
	const FloatVariable& a = getInNumeric("A");
	const FloatVariable& b = getInNumeric("B");
	ImGui::Text("A %f", a.value);
	ImGui::Text("B %f", b.value);
	if (a.value < b.value)
		ImGui::Text("Res True");
	else
		ImGui::Text("Res False");
}

void LessNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void LessNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<BoolVariable>("Res")->val();
	const auto& a = getInNumeric("A");
	const auto& b = getInNumeric("B");
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.dependencys = { a.name,b.name };
	ele.identifier = out.name;
	ele.callback = [out, a, b](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = {} < {};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("bool {} = {} < {};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> LessNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A", isPinNumeric, FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B", isPinNumeric, FloatVariable(2.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<BoolVariable>>("Res"));
	return info;
}

ConditionalFloatNode::ConditionalFloatNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this, outName]() {

		const BoolVariable& a = getInVal<BoolVariable>("A");
		const FloatVariable& b = getInNumeric("B");
		const FloatVariable& c = getInNumeric("C");

		std::string name = (a.IsConstant() && b.IsConstant() && c.IsConstant()) ? "" : outName;

		if (a.value)
			return FloatVariable(b.value, name);
		else
			return FloatVariable(c.value, name);

		});

}

ConditionalFloatNode::ConditionalFloatNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :ConditionalFloatNode(rend, style) {}


void ConditionalFloatNode::draw() {
	const BoolVariable& a = getInVal<BoolVariable>("A");

	const FloatVariable& b = getInNumeric("B");
	const FloatVariable& c = getInNumeric("C");

	if (a.value)
		ImGui::Text("In True");
	else
		ImGui::Text("In False");
	ImGui::Text("True %f", b.value);
	ImGui::Text("False %f", c.value);

}

void ConditionalFloatNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void ConditionalFloatNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<FloatVariable>("Res")->val();
	const auto& a = getInVal<BoolVariable>("A");
	const auto& b = getInNumeric("B");
	const auto& c = getInNumeric("C");

	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.dependencys = { a.name,b.name, c.name };
	ele.identifier = out.name;
	ele.callback = [out, a, b, c](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
		{
			proto.codeLines.push_back(std::format("{} = {}?{}:{};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto), c.GetFormattedName(proto)));

		}
		else
			proto.codeLines.push_back(std::format("float {} = {}?{}:{};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto), c.GetFormattedName(proto)));

		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ConditionalFloatNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<BoolVariable>>("A", ImFlow::ConnectionFilter::SameType(), BoolVariable(false)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B", isPinNumeric, FloatVariable(1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("C", isPinNumeric, FloatVariable(2.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	return info;
}

ConditionalValueNode::ConditionalValueNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outIntName = Variable::UniqueName();
	std::string outFloatName = Variable::UniqueName();
	std::string outFloat2Name = Variable::UniqueName();
	std::string outFloat3Name = Variable::UniqueName();
	std::string outSizeName = Variable::UniqueName();
	std::string outColorName = Variable::UniqueName();

	getOut<IntVariable>("Int Res")->behaviour([this, outIntName]() {
		bool valid = false;
		ConditionalValue res = SelectConditionalValue(getInVal<BoolVariable>("A"), GetConditionalInput(this, "B", ConditionalValueType::Int), GetConditionalInput(this, "C", ConditionalValueType::Int), outIntName, &valid);
		if (valid && res.Type() == ConditionalValueType::Int)
			return std::get<IntVariable>(res.value);
		return IntVariable(0);
		});

	getOut<FloatVariable>("Float Res")->behaviour([this, outFloatName]() {
		bool valid = false;
		ConditionalValue res = SelectConditionalValue(getInVal<BoolVariable>("A"), GetConditionalInput(this, "B", ConditionalValueType::Float), GetConditionalInput(this, "C", ConditionalValueType::Float), outFloatName, &valid);
		if (valid && res.Type() == ConditionalValueType::Float)
			return std::get<FloatVariable>(res.value);
		return FloatVariable(0.f);
		});

	getOut<Float2Variable>("Vector2 Res")->behaviour([this, outFloat2Name]() {
		bool valid = false;
		ConditionalValue res = SelectConditionalValue(getInVal<BoolVariable>("A"), GetConditionalInput(this, "B", ConditionalValueType::Float2), GetConditionalInput(this, "C", ConditionalValueType::Float2), outFloat2Name, &valid);
		if (valid && res.Type() == ConditionalValueType::Float2)
			return std::get<Float2Variable>(res.value);
		return Float2Variable(0.f, 0.f);
		});

	getOut<Float3Variable>("Vector3 Res")->behaviour([this, outFloat3Name]() {
		bool valid = false;
		ConditionalValue res = SelectConditionalValue(getInVal<BoolVariable>("A"), GetConditionalInput(this, "B", ConditionalValueType::Float3), GetConditionalInput(this, "C", ConditionalValueType::Float3), outFloat3Name, &valid);
		if (valid && res.Type() == ConditionalValueType::Float3)
			return std::get<Float3Variable>(res.value);
		return Float3Variable(0.f, 0.f, 0.f);
		});

	getOut<TransformSize>("Size Res")->behaviour([this, outSizeName]() {
		bool valid = false;
		ConditionalValue res = SelectConditionalValue(getInVal<BoolVariable>("A"), GetConditionalInput(this, "B", ConditionalValueType::Size), GetConditionalInput(this, "C", ConditionalValueType::Size), outSizeName, &valid);
		if (valid && res.Type() == ConditionalValueType::Size)
			return std::get<TransformSize>(res.value);
		return TransformSize(_mm_setzero_ps());
		});

	getOut<ColorVariable>("Color Res")->behaviour([this, outColorName]() {
		bool valid = false;
		ConditionalValue res = SelectConditionalValue(getInVal<BoolVariable>("A"), GetConditionalInput(this, "B", ConditionalValueType::Color), GetConditionalInput(this, "C", ConditionalValueType::Color), outColorName, &valid);
		if (valid && res.Type() == ConditionalValueType::Color)
			return std::get<ColorVariable>(res.value);
		return ColorVariable(1.f, 1.f, 1.f, 1.f);
		});
}

ConditionalValueNode::ConditionalValueNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :ConditionalValueNode(rend, style) {}

bool ConditionalValueNode::CanCreateLink(ImFlow::Pin* pin, ImFlow::Pin* other) {
	const std::string& pinName = pin->getName();
	if (pinName == "B" || pinName == "C") {
		ConditionalValueType incomingType = TypeFromInfo(other->getDataType());
		if (incomingType == ConditionalValueType::Invalid)
			return false;

		ConditionalValueType expectedType = ExpectedInputType(this, pinName);
		return expectedType == ConditionalValueType::Invalid || incomingType == expectedType;
	}

	if (IsConditionalOutputName(pinName)) {
		ConditionalValueType outputType = TypeFromInfo(pin->getDataType());
		if (outputType == ConditionalValueType::Invalid)
			return false;

		ConditionalValueType bType = ConnectedInputType(this, "B");
		ConditionalValueType cType = ConnectedInputType(this, "C");

		if (bType != ConditionalValueType::Invalid && bType != outputType)
			return false;
		if (cType != ConditionalValueType::Invalid && cType != outputType)
			return false;
	}

	return true;
}

void ConditionalValueNode::draw() {
	const BoolVariable& condition = getInVal<BoolVariable>("A");
	ConditionalValueType selectedOutputType = ConnectedOutputType(this);
	ConditionalValueType defaultType = DefaultInputType(this);
	ConditionalValueType bType = ConnectedInputType(this, "B");
	ConditionalValueType cType = ConnectedInputType(this, "C");
	bool showAllOutputs = selectedOutputType == ConditionalValueType::Invalid &&
		bType == ConditionalValueType::Invalid &&
		cType == ConditionalValueType::Invalid;
	ConditionalValue trueValue = GetConditionalInput(this, "B", defaultType);
	ConditionalValue falseValue = GetConditionalInput(this, "C", defaultType);
	bool valid = false;
	ConditionalValue res = SelectConditionalValue(condition, trueValue, falseValue, "", &valid);
	if (valid && selectedOutputType != ConditionalValueType::Invalid && res.Type() != selectedOutputType)
		valid = false;

	UpdateConditionalOutputVisibility(this, valid ? res.Type() : ConditionalValueType::Invalid, valid, selectedOutputType, showAllOutputs);
	if (!valid) {
		setStyle(styles.GetErrorStyle());
		if (selectedOutputType != ConditionalValueType::Invalid)
			ImGui::Text("B/C must be %s", ConditionalTypeName(selectedOutputType));
		else
			ImGui::Text("B/C type mismatch");
	}
	else {
		setStyle(styles.GetNodeStyle(category));
		ImGui::Text("Type %s", ConditionalTypeName(res.Type()));
	}

	ImGui::Text("A %s", condition.value ? "True" : "False");
	DrawConditionalValue("True", trueValue);
	DrawConditionalValue("False", falseValue);
	if (valid)
		DrawConditionalValue("Res", res);
}

void ConditionalValueNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void ConditionalValueNode::Export(RuiExportPrototype& proto) {
	const BoolVariable& condition = getInVal<BoolVariable>("A");
	ConditionalValueType selectedOutputType = ConnectedOutputType(this);
	ConditionalValueType defaultType = DefaultInputType(this);
	ConditionalValue trueValue = GetConditionalInput(this, "B", defaultType);
	ConditionalValue falseValue = GetConditionalInput(this, "C", defaultType);
	bool valid = false;
	ConditionalValue res = SelectConditionalValue(condition, trueValue, falseValue, "", &valid);
	if (valid && selectedOutputType != ConditionalValueType::Invalid && res.Type() != selectedOutputType)
		valid = false;
	if (!valid)
		return;

	switch (res.Type()) {
	case ConditionalValueType::Int:
		if (getOut<IntVariable>("Int Res")->isConnected())
			PushConditionalExport(proto, getOut<IntVariable>("Int Res")->val(), condition, trueValue, falseValue, res.Type());
		break;
	case ConditionalValueType::Float:
		if (getOut<FloatVariable>("Float Res")->isConnected())
			PushConditionalExport(proto, getOut<FloatVariable>("Float Res")->val(), condition, trueValue, falseValue, res.Type());
		break;
	case ConditionalValueType::Float2:
		if (getOut<Float2Variable>("Vector2 Res")->isConnected())
			PushConditionalExport(proto, getOut<Float2Variable>("Vector2 Res")->val(), condition, trueValue, falseValue, res.Type());
		break;
	case ConditionalValueType::Float3:
		if (getOut<Float3Variable>("Vector3 Res")->isConnected())
			PushConditionalExport(proto, getOut<Float3Variable>("Vector3 Res")->val(), condition, trueValue, falseValue, res.Type());
		break;
	case ConditionalValueType::Size:
		if (getOut<TransformSize>("Size Res")->isConnected())
			PushConditionalExport(proto, getOut<TransformSize>("Size Res")->val(), condition, trueValue, falseValue, res.Type());
		break;
	case ConditionalValueType::Color:
		if (getOut<ColorVariable>("Color Res")->isConnected())
			PushConditionalExport(proto, getOut<ColorVariable>("Color Res")->val(), condition, trueValue, falseValue, res.Type());
		break;
	default:
		break;
	}
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ConditionalValueNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<BoolVariable>>("A", ImFlow::ConnectionFilter::SameType(), BoolVariable(false)));
	info.push_back(std::make_shared<ImFlow::InPinProto<ConditionalValue>>("B", isPinConditionalValue, ConditionalValue(FloatVariable(1.f))));
	info.push_back(std::make_shared<ImFlow::InPinProto<ConditionalValue>>("C", isPinConditionalValue, ConditionalValue(FloatVariable(0.f))));
	info.push_back(std::make_shared<ImFlow::OutPinProto<IntVariable>>("Int Res"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Float Res"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float2Variable>>("Vector2 Res"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float3Variable>>("Vector3 Res"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<TransformSize>>("Size Res"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<ColorVariable>>("Color Res"));
	return info;
}

EqualFloatNode::EqualFloatNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<BoolVariable>("Res")->behaviour([this, outName]() {

		const FloatVariable& a = getInNumeric("A");
		const FloatVariable& b = getInNumeric("B");
		std::string name = (a.IsConstant() && b.IsConstant()) ? "" : outName;

		return BoolVariable(a.value == b.value, name);

		});

}

EqualFloatNode::EqualFloatNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :EqualFloatNode(rend, style) {}


void EqualFloatNode::draw() {
	const FloatVariable& a = getInNumeric("A");
	const FloatVariable& b = getInNumeric("B");
	ImGui::Text("A %f", a.value);
	ImGui::Text("B %f", b.value);
	if (a.value == b.value)
		ImGui::Text("Res True");
	else
		ImGui::Text("Res False");
}

void EqualFloatNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void EqualFloatNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<BoolVariable>("Res")->val();
	const auto& a = getInNumeric("A");
	const auto& b = getInNumeric("B");
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.dependencys = { a.name,b.name };
	ele.identifier = out.name;
	ele.callback = [out, a, b](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = {} == {};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("bool {} = {} == {};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> EqualFloatNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("A", isPinNumeric, FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("B", isPinNumeric, FloatVariable(2.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<BoolVariable>>("Res"));
	return info;
}



NotGateNode::NotGateNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<BoolVariable>("Res")->behaviour([this, outName]() {

		const BoolVariable& a = getInVal<BoolVariable>("A");
		std::string name = (a.IsConstant()) ? "" : outName;

		return BoolVariable(!a.value, name);

		});

}

NotGateNode::NotGateNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :NotGateNode(rend, style) {}


void NotGateNode::draw() {
	const BoolVariable& a = getInVal<BoolVariable>("A");

	if (!a.value)
	{
		ImGui::Text("A False");
		ImGui::Text("Res True");
	}
	else
	{
		ImGui::Text("A True");
		ImGui::Text("Res False");
	}
}
void NotGateNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void NotGateNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<BoolVariable>("Res")->val();
	const auto& a = getInVal<BoolVariable>("A");
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.dependencys = { a.name };
	ele.identifier = out.name;
	ele.callback = [out, a](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = !{};", out.GetFormattedName(proto), a.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("bool {} = !{};", out.GetFormattedName(proto), a.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> NotGateNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<BoolVariable>>("A", ImFlow::ConnectionFilter::SameType(), BoolVariable(false)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<BoolVariable>>("Res"));
	return info;
}

AndGateNode::AndGateNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<BoolVariable>("Res")->behaviour([this, outName]() {

		const BoolVariable& a = getInVal<BoolVariable>("A");
		const BoolVariable& b = getInVal<BoolVariable>("B");
		std::string name = (a.IsConstant() && b.IsConstant()) ? "" : outName;

		return BoolVariable(a.value == b.value, name);

		});

}

AndGateNode::AndGateNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :AndGateNode(rend, style) {}


void AndGateNode::draw() {
	const BoolVariable& a = getInVal<BoolVariable>("A");
	const BoolVariable& b = getInVal<BoolVariable>("B");
	ImGui::Text("A %s", a.value ? "True" : "False");
	ImGui::Text("A %s", b.value ? "True" : "False");

	if (a.value == b.value)
		ImGui::Text("Res True");
	else
		ImGui::Text("Res False");
}

void AndGateNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void AndGateNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<BoolVariable>("Res")->val();
	const auto& a = getInVal<BoolVariable>("A");
	const auto& b = getInVal<BoolVariable>("B");
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.dependencys = { a.name,b.name };
	ele.identifier = out.name;
	ele.callback = [out, a, b](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = {} == {};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("bool {} = {} == {};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> AndGateNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<BoolVariable>>("A", ImFlow::ConnectionFilter::SameType(), BoolVariable(false)));
	info.push_back(std::make_shared<ImFlow::InPinProto<BoolVariable>>("B", ImFlow::ConnectionFilter::SameType(), BoolVariable(false)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<BoolVariable>>("Res"));
	return info;
}

OrGateNode::OrGateNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<BoolVariable>("Res")->behaviour([this, outName]() {

		const BoolVariable& a = getInVal<BoolVariable>("A");
		const BoolVariable& b = getInVal<BoolVariable>("B");
		std::string name = (a.IsConstant() && b.IsConstant()) ? "" : outName;

		return BoolVariable(a.value || b.value, name);

		});

}

OrGateNode::OrGateNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :OrGateNode(rend, style) {}


void OrGateNode::draw() {
	const BoolVariable& a = getInVal<BoolVariable>("A");
	const BoolVariable& b = getInVal<BoolVariable>("B");
	ImGui::Text("A %s", a.value ? "True" : "False");
	ImGui::Text("A %s", b.value ? "True" : "False");

	if (a.value || b.value)
		ImGui::Text("Res True");
	else
		ImGui::Text("Res False");
}

void OrGateNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void OrGateNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<BoolVariable>("Res")->val();
	const auto& a = getInVal<BoolVariable>("A");
	const auto& b = getInVal<BoolVariable>("B");
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.dependencys = { a.name,b.name };
	ele.identifier = out.name;
	ele.callback = [out, a, b](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} = {} || {};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("bool {} = {} || {};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> OrGateNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<BoolVariable>>("A", ImFlow::ConnectionFilter::SameType(), BoolVariable(false)));
	info.push_back(std::make_shared<ImFlow::InPinProto<BoolVariable>>("B", ImFlow::ConnectionFilter::SameType(), BoolVariable(false)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<BoolVariable>>("Res"));
	return info;
}

EqualStringNode::EqualStringNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<BoolVariable>("Res")->behaviour([this, outName]() {

		const StringVariable& a = getInVal<StringVariable>("A");
		const StringVariable& b = getInVal<StringVariable>("B");
		std::string name = (a.IsConstant() && b.IsConstant()) ? "" : outName;

		return BoolVariable(a.value == b.value, name);

		});

}

EqualStringNode::EqualStringNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :EqualStringNode(rend, style) {}


void EqualStringNode::draw() {
	const StringVariable& a = getInVal<StringVariable>("A");
	const StringVariable& b = getInVal<StringVariable>("B");
	ImGui::Text("A %s", a.value.c_str());
	ImGui::Text("B %s", b.value.c_str());
	if (a.value == b.value)
		ImGui::Text("Res True");
	else
		ImGui::Text("Res False");
}

void EqualStringNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void EqualStringNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<BoolVariable>("Res")->val();
	const auto& a = getInVal<StringVariable>("A");
	const auto& b = getInVal<StringVariable>("B");
	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.dependencys = { a.name,b.name };
	ele.identifier = out.name;
	ele.callback = [out, a, b](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
			proto.codeLines.push_back(std::format("{} !strcmp({}, {});", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		else
			proto.codeLines.push_back(std::format("bool {} = !strcmp({}, {});", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto)));
		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> EqualStringNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<StringVariable>>("A", ImFlow::ConnectionFilter::SameType(), StringVariable("Default String")));
	info.push_back(std::make_shared<ImFlow::InPinProto<StringVariable>>("B", ImFlow::ConnectionFilter::SameType(), StringVariable("Default String")));
	info.push_back(std::make_shared<ImFlow::OutPinProto<BoolVariable>>("Res"));
	return info;
}


ConditionalStringNode::ConditionalStringNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	std::string outName = Variable::UniqueName();
	getOut<StringVariable>("Res")->behaviour([this, outName]() {

		const BoolVariable& a = getInVal<BoolVariable>("A");
		const StringVariable& b = getInVal<StringVariable>("B");
		const StringVariable& c = getInVal<StringVariable>("C");

		std::string name = (a.IsConstant() && b.IsConstant() && c.IsConstant()) ? "" : outName;

		if (a.value)
			return StringVariable(b.value, name);
		else
			return StringVariable(c.value, name);

		});

}

ConditionalStringNode::ConditionalStringNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :ConditionalStringNode(rend, style) {}


void ConditionalStringNode::draw() {
	const BoolVariable& a = getInVal<BoolVariable>("A");

	const StringVariable& b = getInVal<StringVariable>("B");
	const StringVariable& c = getInVal<StringVariable>("C");

	if (a.value)
		ImGui::Text("In True");
	else
		ImGui::Text("In False");
	ImGui::Text("True %s", b.value.c_str());
	ImGui::Text("False %s", c.value.c_str());

}

void ConditionalStringNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void ConditionalStringNode::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<StringVariable>("Res")->val();
	const auto& a = getInVal<BoolVariable>("A");
	const auto& b = getInVal<StringVariable>("B");
	const auto& c = getInVal<StringVariable>("C");

	ExportElement<std::string> ele;
#if _DEBUG
	ele.sourceNodeName = typeid(*this).name();
#endif
	ele.dependencys = { a.name,b.name, c.name };
	ele.identifier = out.name;
	ele.callback = [out, a, b, c](RuiExportPrototype& proto) {
		if (proto.varsInDataStruct.contains(out.name))
		{
			proto.codeLines.push_back(std::format("{} = {}?{}:{};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto), c.GetFormattedName(proto)));

		}
		else
			proto.codeLines.push_back(std::format("const char* {} = {}?{}:{};", out.GetFormattedName(proto), a.GetFormattedName(proto), b.GetFormattedName(proto), c.GetFormattedName(proto)));

		};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ConditionalStringNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<BoolVariable>>("A", ImFlow::ConnectionFilter::SameType(), BoolVariable(false)));
	info.push_back(std::make_shared<ImFlow::InPinProto<StringVariable>>("B", ImFlow::ConnectionFilter::SameType(), StringVariable("Default Text")));
	info.push_back(std::make_shared<ImFlow::InPinProto<StringVariable>>("C", ImFlow::ConnectionFilter::SameType(), StringVariable("Default Test")));
	info.push_back(std::make_shared<ImFlow::OutPinProto<StringVariable>>("Res"));
	return info;
}

void AddConditionalNodes(NodeEditor& editor) {

	editor.AddNodeType<GreaterNode>();
	editor.AddNodeType<LessNode>();
	editor.AddNodeType<ConditionalFloatNode>();
	editor.AddNodeType<ConditionalValueNode>();
	editor.AddNodeType<EqualFloatNode>();
	editor.AddNodeType<AndGateNode>();
	editor.AddNodeType<NotGateNode>();
	editor.AddNodeType<OrGateNode>();
	editor.AddNodeType<EqualStringNode>();
	editor.AddNodeType<ConditionalStringNode>();

}
