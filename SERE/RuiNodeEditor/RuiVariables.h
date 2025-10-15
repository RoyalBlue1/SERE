#pragma once 

#include <string>
#include <format>
#include "Util.h"
#include "ImageAtlas.h"






enum class VariableType : uint8_t {
	NONE = 0x0,
	STRING = 0x1,
	ASSET = 0x2,
	BOOL = 0x3,
	INT = 0x4,
	FLOAT = 0x5,
	FLOAT2 = 0x6,
	FLOAT3 = 0x7,
	COLOR_ALPHA = 0x8,
	GAMETIME = 0x9,
	FLOAT_UNK = 0xA,
	IMAGE = 0xB,
	ASSET_HANDLE = 0xFF
};


struct Variable;
struct IntVariable;
struct BoolVariable;
struct FloatVariable;
struct Float2Variable;
struct Float3Variable;
struct ColorVariable;
struct StringVariable;
struct AssetVariable;

#include "RuiNodeEditor/RuiExportPrototype.h"

struct Variable {
	bool IsConstant() const {
		return name.size() == 0;
	}
	std::string name;
	virtual std::string Literal() const = 0;

	std::string GetFormattedName(RuiExportPrototype& proto) const;
	Variable(std::string name_) :name(name_) {}
	static std::string UniqueName() {
		return std::format("var_{:X}", randomInt64());
	}
};

struct IntVariable :Variable {
	int value;
	IntVariable(int val, std::string name = "") :Variable(name), value(val) {

	}
	IntVariable() :Variable(""), value(0) {};

	std::string Literal() const override {
		return std::format("{}", value);
	}
};

struct BoolVariable :Variable {
	int value;
	BoolVariable(bool val, std::string name = "") :Variable(name), value(val) {

	}
	BoolVariable() :Variable(""), value(false) {};
	std::string Literal() const override {
		return std::format("{}", value);
	}
};

struct FloatVariable :Variable {
	float value;
	FloatVariable(float val, std::string name = "") :Variable(name), value(val) {

	}
	FloatVariable() :Variable(""), value(1.f) {};

	std::string Literal() const override {
		return std::format("{}", value);
	}
};

struct Float2Variable :Variable {
	Vector2 value;
	Float2Variable(Vector2 val, std::string name = "") :Variable(name), value(val) {

	}
	Float2Variable(float x, float y, std::string name = "") :Variable(name), value(x, y) {

	}
	Float2Variable() :Variable(""), value(1.f, 1.f) {};

	std::string Literal() const override {
		return std::format("Vector2({},{})", value.x, value.y);
	}
};

struct Float3Variable :Variable {
	Vector3 value;
	Float3Variable(Vector3 val, std::string name = "") :Variable(name), value(val) {

	}
	Float3Variable(float x, float y, float z, std::string name = "") :Variable(name), value(x, y, z) {

	}
	Float3Variable() :Variable(""), value(1.f, 1.f, 1.f) {};

	std::string Literal() const override {
		return std::format("Vector3({},{},{})", value.x, value.y, value.z);
	}
};

struct ColorVariable :Variable {
	Color value;
	ColorVariable(Color val, std::string name = "") :Variable(name), value(val) {

	}
	ColorVariable(float r, float g, float b, float a, std::string name = "") :Variable(name), value(r, g, b, a) {

	}
	ColorVariable() :Variable(""), value(1.f, 1.f, 1.f, 1.f) {};

	std::string Literal() const override {
		return std::format("Color({},{},{},{})", value.red, value.green, value.blue, value.alpha);
	}
};

struct StringVariable :Variable {
	std::string value;
	StringVariable(std::string val, std::string name = "") :Variable(name), value(val) {

	}
	StringVariable(const char* val, std::string name = "") :Variable(name), value(val) {

	}
	StringVariable() :Variable(""), value("") {}

	std::string Literal() const override {
		return std::format("\"{}\"", value);
	}
};

struct AssetVariable :Variable {
	uint32_t hash;
	AssetVariable(std::string val, std::string name = "") :Variable(name) {
		hash = loadAsset(val.c_str());
	}
	AssetVariable(uint32_t val, std::string name = "") :Variable(name), hash(val) {

	}
	AssetVariable() :Variable(""), hash(INVALID_ASSET) {}

	std::string Literal() const override {
		return "";//Asset will never be literal
	}
};

struct TransformSize :Variable {

	TransformSize() :Variable("") {
		size = _mm_set1_ps(128.f);
	}
	TransformSize(__m128 size_, std::string name = "") :Variable(name) {
		size = size_;
	}
	__m128 size;
	std::string Literal() const override {
		return std::format("_mm_set_ps({},{},{},{})", size.m128_f32[3], size.m128_f32[2], size.m128_f32[1], size.m128_f32[0]);
	}
};
