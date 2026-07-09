#include "Nodes/MathNodes.h"

namespace {
enum class MathBinaryOp {
	Add,
	Subtract,
	Multiply,
	Divide,
	Modulo,
	Exponent
};

enum class MathUnaryOp {
	Absolute,
	Sine,
	Tangent,
	Cosine,
	SquareRoot,
	Round,
	Floor,
	Ceil,
	Truncate
};

const char* OpSymbol(MathBinaryOp op) {
	switch (op) {
	case MathBinaryOp::Add: return "+";
	case MathBinaryOp::Subtract: return "-";
	case MathBinaryOp::Multiply: return "*";
	case MathBinaryOp::Divide: return "/";
	case MathBinaryOp::Modulo: return "%";
	case MathBinaryOp::Exponent: return "^";
	}
	return "?";
}

int ComponentCount(MathVariableType type) {
	switch (type) {
	case MathVariableType::FLOAT: return 1;
	case MathVariableType::FLOAT2: return 2;
	case MathVariableType::FLOAT3: return 3;
	case MathVariableType::SIZE: return 4;
	}
	return 1;
}

float ComponentAt(const MathVariable& value, int index) {
	if (std::holds_alternative<FloatVariable>(value.value))
		return std::get<FloatVariable>(value.value).value;
	if (std::holds_alternative<Float2Variable>(value.value)) {
		const Vector2& val = std::get<Float2Variable>(value.value).value;
		return index == 0 ? val.x : val.y;
	}
	if (std::holds_alternative<Float3Variable>(value.value)) {
		const Vector3& val = std::get<Float3Variable>(value.value).value;
		if (index == 0)
			return val.x;
		if (index == 1)
			return val.y;
		return val.z;
	}
	const TransformSize& val = std::get<TransformSize>(value.value);
	float size[4];
	_mm_storeu_ps(size, val.size);
	return size[index];
}

MathVariableType ResultType(const MathVariable& a, const MathVariable& b, bool& valid) {
	MathVariableType aType = a.Type();
	MathVariableType bType = b.Type();
	valid = true;
	if (aType == bType)
		return aType;
	if (aType == MathVariableType::FLOAT)
		return bType;
	if (bType == MathVariableType::FLOAT)
		return aType;
	valid = false;
	return MathVariableType::FLOAT;
}

float ApplyComponent(MathBinaryOp op, float a, float b) {
	switch (op) {
	case MathBinaryOp::Add: return a + b;
	case MathBinaryOp::Subtract: return a - b;
	case MathBinaryOp::Multiply: return a * b;
	case MathBinaryOp::Divide: return b == 0.f ? 0.f : a / b;
	case MathBinaryOp::Modulo: return b == 0.f ? 0.f : std::fmodf(a, b);
	case MathBinaryOp::Exponent: return std::pow(a, b);
	}
	return 0.f;
}

MathVariable EvalBinary(MathBinaryOp op, const MathVariable& a, const MathVariable& b, const std::string& outName, bool* valid = nullptr, bool* divideByZero = nullptr) {
	bool isValid = false;
	MathVariableType type = ResultType(a, b, isValid);
	if (valid)
		*valid = isValid;
	if (!isValid)
		return MathVariable(FloatVariable(0.f));

	int count = ComponentCount(type);
	std::string name = (a.IsConstant() && b.IsConstant()) ? "" : outName;
	bool hasZeroDivisor = false;
	float result[4] = {};
	for (int i = 0; i < count; i++) {
		float right = ComponentAt(b, ComponentCount(b.Type()) == 1 ? 0 : i);
		hasZeroDivisor |= (op == MathBinaryOp::Divide || op == MathBinaryOp::Modulo) && right == 0.f;
		result[i] = ApplyComponent(op, ComponentAt(a, ComponentCount(a.Type()) == 1 ? 0 : i), right);
	}
	if (divideByZero)
		*divideByZero = hasZeroDivisor;

	switch (type) {
	case MathVariableType::FLOAT:
		return MathVariable(FloatVariable(result[0], name));
	case MathVariableType::FLOAT2:
		return MathVariable(Float2Variable(result[0], result[1], name));
	case MathVariableType::FLOAT3:
		return MathVariable(Float3Variable(result[0], result[1], result[2], name));
	case MathVariableType::SIZE:
		return MathVariable(TransformSize(_mm_set_ps(result[3], result[2], result[1], result[0]), name));
	}
	return MathVariable(FloatVariable(0.f));
}

FloatVariable AsFloatOutput(const MathVariable& value) {
	if (std::holds_alternative<FloatVariable>(value.value))
		return std::get<FloatVariable>(value.value);
	return FloatVariable(0.f);
}

Float2Variable AsFloat2Output(const MathVariable& value) {
	if (std::holds_alternative<Float2Variable>(value.value))
		return std::get<Float2Variable>(value.value);
	if (std::holds_alternative<FloatVariable>(value.value)) {
		const FloatVariable& scalar = std::get<FloatVariable>(value.value);
		return Float2Variable(scalar.value, scalar.value, scalar.name);
	}
	return Float2Variable(0.f, 0.f);
}

Float3Variable AsFloat3Output(const MathVariable& value) {
	if (std::holds_alternative<Float3Variable>(value.value))
		return std::get<Float3Variable>(value.value);
	if (std::holds_alternative<FloatVariable>(value.value)) {
		const FloatVariable& scalar = std::get<FloatVariable>(value.value);
		return Float3Variable(scalar.value, scalar.value, scalar.value, scalar.name);
	}
	return Float3Variable(0.f, 0.f, 0.f);
}

TransformSize AsSizeOutput(const MathVariable& value) {
	if (std::holds_alternative<TransformSize>(value.value))
		return std::get<TransformSize>(value.value);
	if (std::holds_alternative<FloatVariable>(value.value)) {
		const FloatVariable& scalar = std::get<FloatVariable>(value.value);
		return TransformSize(_mm_set1_ps(scalar.value), scalar.name);
	}
	return TransformSize(_mm_setzero_ps());
}

void DrawMathValue(const char* label, const MathVariable& value) {
	switch (value.Type()) {
	case MathVariableType::FLOAT:
		ImGui::Text("%s %f", label, std::get<FloatVariable>(value.value).value);
		break;
	case MathVariableType::FLOAT2:
	{
		const Vector2& val = std::get<Float2Variable>(value.value).value;
		ImGui::Text("%s %f, %f", label, val.x, val.y);
		break;
	}
	case MathVariableType::FLOAT3:
	{
		const Vector3& val = std::get<Float3Variable>(value.value).value;
		ImGui::Text("%s %f, %f, %f", label, val.x, val.y, val.z);
		break;
	}
	case MathVariableType::SIZE:
	{
		float size[4];
		_mm_storeu_ps(size, std::get<TransformSize>(value.value).size);
		ImGui::Text("%s %f, %f, %f, %f", label, size[0], size[1], size[2], size[3]);
		break;
	}
	}
}

bool CanUseMathOutput(MathVariableType resultType, MathVariableType outputType) {
	if (resultType == MathVariableType::FLOAT)
		return true;
	return resultType == outputType;
}

void UpdateDynamicMathOutputVisibility(ImFlow::BaseNode* node, MathVariableType resultType, bool valid) {
	ImFlow::Pin* scalar = node->outPin("Res");
	ImFlow::Pin* vector2 = node->outPin("Vector2 Res");
	ImFlow::Pin* vector3 = node->outPin("Vector3 Res");
	ImFlow::Pin* size = node->outPin("Size Res");
	bool allowScalar = valid && CanUseMathOutput(resultType, MathVariableType::FLOAT);
	bool allowVector2 = valid && CanUseMathOutput(resultType, MathVariableType::FLOAT2);
	bool allowVector3 = valid && CanUseMathOutput(resultType, MathVariableType::FLOAT3);
	bool allowSize = valid && CanUseMathOutput(resultType, MathVariableType::SIZE);
	bool hasSelectedOutput = (allowScalar && scalar->isConnected()) ||
		(allowVector2 && vector2->isConnected()) ||
		(allowVector3 && vector3->isConnected()) ||
		(allowSize && size->isConnected());

	scalar->visible(allowScalar && (!hasSelectedOutput || scalar->isConnected()));
	vector2->visible(allowVector2 && (!hasSelectedOutput || vector2->isConnected()));
	vector3->visible(allowVector3 && (!hasSelectedOutput || vector3->isConnected()));
	size->visible(allowSize && (!hasSelectedOutput || size->isConnected()));
}

std::string ComponentExpression(const MathVariable& value, int index, RuiExportPrototype& proto) {
	if (value.Type() == MathVariableType::FLOAT)
		return value.GetFormattedName(proto);
	if (value.IsConstant())
		return std::format("{}", ComponentAt(value, index));

	if (value.Type() == MathVariableType::SIZE)
		return std::format("{}.m128_f32[{}]", value.GetFormattedName(proto), index);

	const char* component = index == 0 ? "x" : (index == 1 ? "y" : "z");
	return std::format("{}.{}", value.GetFormattedName(proto), component);
}

const char* ExportTypeName(MathVariableType type) {
	switch (type) {
	case MathVariableType::FLOAT: return "float";
	case MathVariableType::FLOAT2: return "Vector2";
	case MathVariableType::FLOAT3: return "Vector3";
	case MathVariableType::SIZE: return "__m128";
	}
	return "float";
}

std::string BuildComponentExpression(MathBinaryOp op, const std::string& a, const std::string& b) {
	switch (op) {
	case MathBinaryOp::Modulo:
		return std::format("std::fmodf({}, {})", a, b);
	case MathBinaryOp::Exponent:
		return std::format("std::pow({}, {})", a, b);
	default:
		return std::format("{} {} {}", a, OpSymbol(op), b);
	}
}

std::string BuildBinaryExpression(MathBinaryOp op, MathVariableType outType, const MathVariable& a, const MathVariable& b, RuiExportPrototype& proto) {
	int count = ComponentCount(outType);
	if (count == 1)
		return BuildComponentExpression(op, ComponentExpression(a, 0, proto), ComponentExpression(b, 0, proto));

	std::string components[4];
	for (int i = 0; i < count; i++) {
		int aIndex = ComponentCount(a.Type()) == 1 ? 0 : i;
		int bIndex = ComponentCount(b.Type()) == 1 ? 0 : i;
		components[i] = BuildComponentExpression(op, ComponentExpression(a, aIndex, proto), ComponentExpression(b, bIndex, proto));
	}
	if (outType == MathVariableType::FLOAT2)
		return std::format("Vector2({}, {})", components[0], components[1]);
	if (outType == MathVariableType::FLOAT3)
		return std::format("Vector3({}, {}, {})", components[0], components[1], components[2]);
	return std::format("_mm_set_ps({}, {}, {}, {})", components[3], components[2], components[1], components[0]);
}

std::string DivideByZeroGuard(const MathVariable& b, RuiExportPrototype& proto) {
	int count = ComponentCount(b.Type());
	if (count == 1)
		return std::format("{} == 0", ComponentExpression(b, 0, proto));

	std::string guard;
	for (int i = 0; i < count; i++) {
		if (i)
			guard += " || ";
		guard += std::format("{} == 0", ComponentExpression(b, i, proto));
	}
	return guard;
}

template<typename T>
void PushBinaryExport(RuiExportPrototype& proto, const T& out, MathVariableType outType, const MathVariable& a, const MathVariable& b, MathBinaryOp op) {
	if (out.IsConstant())
		return;

	ExportElement<std::string> ele;
	ele.dependencys = { a.Name(), b.Name() };
	ele.identifier = out.name;
	ele.callback = [out, outType, a, b, op](RuiExportPrototype& proto) {
		const char* typeName = ExportTypeName(outType);
		std::string assignPrefix = proto.varsInDataStruct.contains(out.name) ? "" : std::format("{} ", typeName);
		if (op == MathBinaryOp::Divide || op == MathBinaryOp::Modulo) {
			proto.codeLines.push_back(std::format(
				"if({}){{\n\tfuncs->SetErrorWithReason(inst,\"{} by zero\");\n\treturn;\n}}\n{}{} = {};",
				DivideByZeroGuard(b, proto),
				op == MathBinaryOp::Divide ? "Divide" : "Modulo",
				assignPrefix,
				out.GetFormattedName(proto),
				BuildBinaryExpression(op, outType, a, b, proto)));
		}
		else {
			proto.codeLines.push_back(std::format("{}{} = {};", assignPrefix, out.GetFormattedName(proto), BuildBinaryExpression(op, outType, a, b, proto)));
		}
	};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> BinaryMathPinInfo(float defaultB) {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<MathVariable>>("A", isPinMath, MathVariable(FloatVariable(0.f))));
	info.push_back(std::make_shared<ImFlow::InPinProto<MathVariable>>("B", isPinMath, MathVariable(FloatVariable(defaultB))));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float2Variable>>("Vector2 Res"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float3Variable>>("Vector3 Res"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<TransformSize>>("Size Res"));
	return info;
}

float ApplyUnary(MathUnaryOp op, float value) {
	switch (op) {
	case MathUnaryOp::Absolute: return abs(value);
	case MathUnaryOp::Sine: return sin(value);
	case MathUnaryOp::Tangent: return tan(value);
	case MathUnaryOp::Cosine: return cos(value);
	case MathUnaryOp::SquareRoot: return sqrt(value);
	case MathUnaryOp::Round: return round(value);
	case MathUnaryOp::Floor: return floor(value);
	case MathUnaryOp::Ceil: return ceil(value);
	case MathUnaryOp::Truncate: return trunc(value);
	}
	return value;
}

const char* UnaryFunctionName(MathUnaryOp op) {
	switch (op) {
	case MathUnaryOp::Absolute: return "abs";
	case MathUnaryOp::Sine: return "sin";
	case MathUnaryOp::Tangent: return "tan";
	case MathUnaryOp::Cosine: return "cos";
	case MathUnaryOp::SquareRoot: return "sqrt";
	case MathUnaryOp::Round: return "round";
	case MathUnaryOp::Floor: return "floor";
	case MathUnaryOp::Ceil: return "ceil";
	case MathUnaryOp::Truncate: return "trunc";
	}
	return "";
}

MathVariable EvalUnary(MathUnaryOp op, const MathVariable& in, const std::string& outName) {
	std::string name = in.IsConstant() ? "" : outName;
	switch (in.Type()) {
	case MathVariableType::FLOAT:
		return MathVariable(FloatVariable(ApplyUnary(op, ComponentAt(in, 0)), name));
	case MathVariableType::FLOAT2:
		return MathVariable(Float2Variable(ApplyUnary(op, ComponentAt(in, 0)), ApplyUnary(op, ComponentAt(in, 1)), name));
	case MathVariableType::FLOAT3:
		return MathVariable(Float3Variable(ApplyUnary(op, ComponentAt(in, 0)), ApplyUnary(op, ComponentAt(in, 1)), ApplyUnary(op, ComponentAt(in, 2)), name));
	case MathVariableType::SIZE:
		return MathVariable(TransformSize(_mm_set_ps(ApplyUnary(op, ComponentAt(in, 3)), ApplyUnary(op, ComponentAt(in, 2)), ApplyUnary(op, ComponentAt(in, 1)), ApplyUnary(op, ComponentAt(in, 0))), name));
	}
	return MathVariable(FloatVariable(0.f));
}

std::string BuildUnaryExpression(MathUnaryOp op, MathVariableType outType, const MathVariable& in, RuiExportPrototype& proto) {
	int count = ComponentCount(outType);
	std::string components[4];
	for (int i = 0; i < count; i++)
		components[i] = std::format("{}({})", UnaryFunctionName(op), ComponentExpression(in, i, proto));

	if (outType == MathVariableType::FLOAT)
		return components[0];
	if (outType == MathVariableType::FLOAT2)
		return std::format("Vector2({}, {})", components[0], components[1]);
	if (outType == MathVariableType::FLOAT3)
		return std::format("Vector3({}, {}, {})", components[0], components[1], components[2]);
	return std::format("_mm_set_ps({}, {}, {}, {})", components[3], components[2], components[1], components[0]);
}

template<typename T>
void PushUnaryExport(RuiExportPrototype& proto, const T& out, MathVariableType outType, const MathVariable& in, MathUnaryOp op) {
	if (out.IsConstant())
		return;

	ExportElement<std::string> ele;
	ele.dependencys = { in.Name() };
	ele.identifier = out.name;
	ele.callback = [out, outType, in, op](RuiExportPrototype& proto) {
		const char* typeName = ExportTypeName(outType);
		std::string assignPrefix = proto.varsInDataStruct.contains(out.name) ? "" : std::format("{} ", typeName);
		proto.codeLines.push_back(std::format("{}{} = {};", assignPrefix, out.GetFormattedName(proto), BuildUnaryExpression(op, outType, in, proto)));
	};
	proto.codeElements.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> UnaryMathPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<MathVariable>>("A", isPinMath, MathVariable(FloatVariable(0.f))));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Res"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float2Variable>>("Vector2 Res"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float3Variable>>("Vector3 Res"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<TransformSize>>("Size Res"));
	return info;
}
}

#define CONFIGURE_BINARY_NODE(NodeTypeName, OpName) \
	std::string outFloatName = Variable::UniqueName(); \
	std::string outFloat2Name = Variable::UniqueName(); \
	std::string outFloat3Name = Variable::UniqueName(); \
	std::string outSizeName = Variable::UniqueName(); \
	getOut<FloatVariable>("Res")->behaviour([this, outFloatName]() { \
		bool valid = false; \
		MathVariable res = EvalBinary(OpName, getInMath("A"), getInMath("B"), outFloatName, &valid); \
		if (valid) \
			return AsFloatOutput(res); \
		return FloatVariable(0.f); \
	}); \
	getOut<Float2Variable>("Vector2 Res")->behaviour([this, outFloat2Name]() { \
		bool valid = false; \
		MathVariable res = EvalBinary(OpName, getInMath("A"), getInMath("B"), outFloat2Name, &valid); \
		if (valid) \
			return AsFloat2Output(res); \
		return Float2Variable(0.f, 0.f); \
	}); \
	getOut<Float3Variable>("Vector3 Res")->behaviour([this, outFloat3Name]() { \
		bool valid = false; \
		MathVariable res = EvalBinary(OpName, getInMath("A"), getInMath("B"), outFloat3Name, &valid); \
		if (valid) \
			return AsFloat3Output(res); \
		return Float3Variable(0.f, 0.f, 0.f); \
	}); \
	getOut<TransformSize>("Size Res")->behaviour([this, outSizeName]() { \
		bool valid = false; \
		MathVariable res = EvalBinary(OpName, getInMath("A"), getInMath("B"), outSizeName, &valid); \
		if (valid) \
			return AsSizeOutput(res); \
		return TransformSize(_mm_setzero_ps()); \
	});

#define DRAW_BINARY_NODE(OpName) \
	MathVariable a = getInMath("A"); \
	MathVariable b = getInMath("B"); \
	bool valid = false; \
	bool divideByZero = false; \
	MathVariable res = EvalBinary(OpName, a, b, "", &valid, &divideByZero); \
	UpdateDynamicMathOutputVisibility(this, res.Type(), valid); \
	DrawMathValue("A", a); \
	DrawMathValue("B", b); \
	if (!valid) { \
		setStyle(styles.GetErrorStyle()); \
		ImGui::Text("Mismatched vector sizes"); \
	} \
	else if (divideByZero) { \
		setStyle(styles.GetErrorStyle()); \
		ImGui::Text("Zero divisor"); \
		DrawMathValue("Res", res); \
	} \
	else { \
		setStyle(styles.GetNodeStyle(category)); \
		DrawMathValue("Res", res); \
	}

#define EXPORT_BINARY_NODE(OpName) \
	MathVariable a = getInMath("A"); \
	MathVariable b = getInMath("B"); \
	bool valid = false; \
	EvalBinary(OpName, a, b, "", &valid); \
	if (!valid) \
		return; \
	MathVariable res = EvalBinary(OpName, a, b, ""); \
	auto outFloat = getOut<FloatVariable>("Res"); \
	auto outFloat2 = getOut<Float2Variable>("Vector2 Res"); \
	auto outFloat3 = getOut<Float3Variable>("Vector3 Res"); \
	auto outSize = getOut<TransformSize>("Size Res"); \
	if (outFloat->isConnected() && CanUseMathOutput(res.Type(), MathVariableType::FLOAT)) \
		PushBinaryExport(proto, outFloat->val(), MathVariableType::FLOAT, a, b, OpName); \
	if (outFloat2->isConnected() && CanUseMathOutput(res.Type(), MathVariableType::FLOAT2)) \
		PushBinaryExport(proto, outFloat2->val(), MathVariableType::FLOAT2, a, b, OpName); \
	if (outFloat3->isConnected() && CanUseMathOutput(res.Type(), MathVariableType::FLOAT3)) \
		PushBinaryExport(proto, outFloat3->val(), MathVariableType::FLOAT3, a, b, OpName); \
	if (outSize->isConnected() && CanUseMathOutput(res.Type(), MathVariableType::SIZE)) \
		PushBinaryExport(proto, outSize->val(), MathVariableType::SIZE, a, b, OpName);

#define CONFIGURE_UNARY_NODE(OpName) \
	std::string outFloatName = Variable::UniqueName(); \
	std::string outFloat2Name = Variable::UniqueName(); \
	std::string outFloat3Name = Variable::UniqueName(); \
	std::string outSizeName = Variable::UniqueName(); \
	getOut<FloatVariable>("Res")->behaviour([this, outFloatName]() { \
		MathVariable res = EvalUnary(OpName, getInMath("A"), outFloatName); \
		return AsFloatOutput(res); \
	}); \
	getOut<Float2Variable>("Vector2 Res")->behaviour([this, outFloat2Name]() { \
		MathVariable res = EvalUnary(OpName, getInMath("A"), outFloat2Name); \
		return AsFloat2Output(res); \
	}); \
	getOut<Float3Variable>("Vector3 Res")->behaviour([this, outFloat3Name]() { \
		MathVariable res = EvalUnary(OpName, getInMath("A"), outFloat3Name); \
		return AsFloat3Output(res); \
	}); \
	getOut<TransformSize>("Size Res")->behaviour([this, outSizeName]() { \
		MathVariable res = EvalUnary(OpName, getInMath("A"), outSizeName); \
		return AsSizeOutput(res); \
	});

#define DRAW_UNARY_NODE(OpName) \
	MathVariable a = getInMath("A"); \
	MathVariable res = EvalUnary(OpName, a, ""); \
	UpdateDynamicMathOutputVisibility(this, res.Type(), true); \
	setStyle(styles.GetNodeStyle(category)); \
	DrawMathValue("A", a); \
	DrawMathValue("Res", res);

#define EXPORT_UNARY_NODE(OpName) \
	MathVariable a = getInMath("A"); \
	MathVariable res = EvalUnary(OpName, a, ""); \
	auto outFloat = getOut<FloatVariable>("Res"); \
	auto outFloat2 = getOut<Float2Variable>("Vector2 Res"); \
	auto outFloat3 = getOut<Float3Variable>("Vector3 Res"); \
	auto outSize = getOut<TransformSize>("Size Res"); \
	if (outFloat->isConnected() && CanUseMathOutput(res.Type(), MathVariableType::FLOAT)) \
		PushUnaryExport(proto, outFloat->val(), MathVariableType::FLOAT, a, OpName); \
	if (outFloat2->isConnected() && CanUseMathOutput(res.Type(), MathVariableType::FLOAT2)) \
		PushUnaryExport(proto, outFloat2->val(), MathVariableType::FLOAT2, a, OpName); \
	if (outFloat3->isConnected() && CanUseMathOutput(res.Type(), MathVariableType::FLOAT3)) \
		PushUnaryExport(proto, outFloat3->val(), MathVariableType::FLOAT3, a, OpName); \
	if (outSize->isConnected() && CanUseMathOutput(res.Type(), MathVariableType::SIZE)) \
		PushUnaryExport(proto, outSize->val(), MathVariableType::SIZE, a, OpName);

MultiplyNode::MultiplyNode(RenderInstance& rend,ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	CONFIGURE_BINARY_NODE(MultiplyNode, MathBinaryOp::Multiply);
}

MultiplyNode::MultiplyNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):MultiplyNode(rend,style){}

void MultiplyNode::draw() {
	DRAW_BINARY_NODE(MathBinaryOp::Multiply);
}

void MultiplyNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void MultiplyNode::Export(RuiExportPrototype& proto) {
	EXPORT_BINARY_NODE(MathBinaryOp::Multiply);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> MultiplyNode::GetPinInfo() {
	return BinaryMathPinInfo(2.f);
}

AdditionNode::AdditionNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	CONFIGURE_BINARY_NODE(AdditionNode, MathBinaryOp::Add);
}

AdditionNode::AdditionNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):AdditionNode(rend,style){}

void AdditionNode::draw() {
	DRAW_BINARY_NODE(MathBinaryOp::Add);
}

void AdditionNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void AdditionNode::Export(RuiExportPrototype& proto) {
	EXPORT_BINARY_NODE(MathBinaryOp::Add);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> AdditionNode::GetPinInfo() {
	return BinaryMathPinInfo(0.f);
}

SubtractNode::SubtractNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	CONFIGURE_BINARY_NODE(SubtractNode, MathBinaryOp::Subtract);
}

SubtractNode::SubtractNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):SubtractNode(rend,style){}

void SubtractNode::draw() {
	DRAW_BINARY_NODE(MathBinaryOp::Subtract);
}

void SubtractNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void SubtractNode::Export(RuiExportPrototype& proto) {
	EXPORT_BINARY_NODE(MathBinaryOp::Subtract);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> SubtractNode::GetPinInfo() {
	return BinaryMathPinInfo(0.f);
}

DivideNode::DivideNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	CONFIGURE_BINARY_NODE(DivideNode, MathBinaryOp::Divide);
}

DivideNode::DivideNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):DivideNode(rend,style){}

void DivideNode::draw() {
	DRAW_BINARY_NODE(MathBinaryOp::Divide);
}

void DivideNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void DivideNode::Export(RuiExportPrototype& proto) {
	EXPORT_BINARY_NODE(MathBinaryOp::Divide);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> DivideNode::GetPinInfo() {
	return BinaryMathPinInfo(2.f);
}

ModuloNode::ModuloNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	CONFIGURE_BINARY_NODE(ModuloNode, MathBinaryOp::Modulo);
}

ModuloNode::ModuloNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):ModuloNode(rend,style){}

void ModuloNode::draw() {
	DRAW_BINARY_NODE(MathBinaryOp::Modulo);
}

void ModuloNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void ModuloNode::Export(RuiExportPrototype& proto) {
	EXPORT_BINARY_NODE(MathBinaryOp::Modulo);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ModuloNode::GetPinInfo() {
	return BinaryMathPinInfo(1.f);
}


AbsoluteNode::AbsoluteNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	CONFIGURE_UNARY_NODE(MathUnaryOp::Absolute);
}

AbsoluteNode::AbsoluteNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):AbsoluteNode(rend,style){}

void AbsoluteNode::draw() {
	DRAW_UNARY_NODE(MathUnaryOp::Absolute);
}

void AbsoluteNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void AbsoluteNode::Export(RuiExportPrototype& proto) {
	EXPORT_UNARY_NODE(MathUnaryOp::Absolute);
}


std::vector<std::shared_ptr<ImFlow::PinProto>> AbsoluteNode::GetPinInfo() {
	return UnaryMathPinInfo();
}

SineNode::SineNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	CONFIGURE_UNARY_NODE(MathUnaryOp::Sine);
}

SineNode::SineNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):SineNode(rend,style){}

void SineNode::draw() {
	DRAW_UNARY_NODE(MathUnaryOp::Sine);
}

void SineNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void SineNode::Export(RuiExportPrototype& proto) {
	EXPORT_UNARY_NODE(MathUnaryOp::Sine);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> SineNode::GetPinInfo() {
	return UnaryMathPinInfo();
}

ExponentNode::ExponentNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	CONFIGURE_BINARY_NODE(ExponentNode, MathBinaryOp::Exponent);
}

ExponentNode::ExponentNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):ExponentNode(rend,style){}

void ExponentNode::draw() {
	DRAW_BINARY_NODE(MathBinaryOp::Exponent);
}

void ExponentNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void ExponentNode::Export(RuiExportPrototype& proto) {
	EXPORT_BINARY_NODE(MathBinaryOp::Exponent);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> ExponentNode::GetPinInfo() {
	return BinaryMathPinInfo(0.f);
}

MappingNode::MappingNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	std::string outName = Variable::UniqueName();
	getOut<FloatVariable>("Res")->behaviour([this,outName]() {
		const FloatVariable& a = getInNumeric("A");
		return FloatVariable(map.MapVar(a.value), outName);
	});

}

MappingNode::MappingNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):MappingNode(rend,style){
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

TangentNode::TangentNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	CONFIGURE_UNARY_NODE(MathUnaryOp::Tangent);
}

TangentNode::TangentNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :TangentNode(rend, style) {}

void TangentNode::draw() {
	DRAW_UNARY_NODE(MathUnaryOp::Tangent);
}

void TangentNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void TangentNode::Export(RuiExportPrototype& proto) {
	EXPORT_UNARY_NODE(MathUnaryOp::Tangent);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> TangentNode::GetPinInfo() {
	return UnaryMathPinInfo();
}

CosineNode::CosineNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	CONFIGURE_UNARY_NODE(MathUnaryOp::Cosine);
}

CosineNode::CosineNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :CosineNode(rend, style) {}

void CosineNode::draw() {
	DRAW_UNARY_NODE(MathUnaryOp::Cosine);
}

void CosineNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void CosineNode::Export(RuiExportPrototype& proto) {
	EXPORT_UNARY_NODE(MathUnaryOp::Cosine);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> CosineNode::GetPinInfo() {
	return UnaryMathPinInfo();
}

SquareRootNode::SquareRootNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	CONFIGURE_UNARY_NODE(MathUnaryOp::SquareRoot);
}

SquareRootNode::SquareRootNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :SquareRootNode(rend, style) {}

void SquareRootNode::draw() {
	DRAW_UNARY_NODE(MathUnaryOp::SquareRoot);
}

void SquareRootNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void SquareRootNode::Export(RuiExportPrototype& proto) {
	EXPORT_UNARY_NODE(MathUnaryOp::SquareRoot);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> SquareRootNode::GetPinInfo() {
	return UnaryMathPinInfo();
}

RoundNode::RoundNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	CONFIGURE_UNARY_NODE(MathUnaryOp::Round);
}

RoundNode::RoundNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :RoundNode(rend, style) {}

void RoundNode::draw() {
	DRAW_UNARY_NODE(MathUnaryOp::Round);
}

void RoundNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void RoundNode::Export(RuiExportPrototype& proto) {
	EXPORT_UNARY_NODE(MathUnaryOp::Round);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> RoundNode::GetPinInfo() {
	return UnaryMathPinInfo();
}

FloorNode::FloorNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	CONFIGURE_UNARY_NODE(MathUnaryOp::Floor);
}

FloorNode::FloorNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :FloorNode(rend, style) {}

void FloorNode::draw() {
	DRAW_UNARY_NODE(MathUnaryOp::Floor);
}

void FloorNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void FloorNode::Export(RuiExportPrototype& proto) {
	EXPORT_UNARY_NODE(MathUnaryOp::Floor);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> FloorNode::GetPinInfo() {
	return UnaryMathPinInfo();
}

CeilNode::CeilNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	CONFIGURE_UNARY_NODE(MathUnaryOp::Ceil);
}

CeilNode::CeilNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :CeilNode(rend, style) {}

void CeilNode::draw() {
	DRAW_UNARY_NODE(MathUnaryOp::Ceil);
}

void CeilNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void CeilNode::Export(RuiExportPrototype& proto) {
	EXPORT_UNARY_NODE(MathUnaryOp::Ceil);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> CeilNode::GetPinInfo() {
	return UnaryMathPinInfo();
}

TruncNode::TruncNode(RenderInstance& rend, ImFlow::StyleManager& style) :RuiBaseNode(name, category, GetPinInfo(), rend, style) {
	CONFIGURE_UNARY_NODE(MathUnaryOp::Truncate);
}

TruncNode::TruncNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :TruncNode(rend, style) {}

void TruncNode::draw() {
	DRAW_UNARY_NODE(MathUnaryOp::Truncate);
}

void TruncNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name", name, allocator);
	obj.AddMember("Category", category, allocator);
	RuiBaseNode::Serialize(obj, allocator);
}

void TruncNode::Export(RuiExportPrototype& proto) {
	EXPORT_UNARY_NODE(MathUnaryOp::Truncate);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> TruncNode::GetPinInfo() {
	return UnaryMathPinInfo();
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
