#pragma once
#include <map>
#include <vector>
#include <set>
#include <any>
#include <memory>
#include <unordered_map>
#include <fstream>

struct RuiExportPrototype;

#include "Imgui/ImNodeFlow.h"
#include "RuiNodeEditor/RuiVariables.h"
#include "RuiNodeEditor/Mapping.h"
#include "RuiRendering/RenderManager.h"
#include "RuiNodeEditor/RuiBaseNode.h"

template<typename T>
struct ExportElement {
	T identifier;
	std::set<T> dependencys;
	std::function<void(RuiExportPrototype&)> callback;
};


struct Float2Offsets {
	uint16_t x;
	uint16_t y;
};

struct Float3Offsets {
	uint16_t x;
	uint16_t y;
	uint16_t z;
};

struct ColorOffsets {
	uint16_t red;
	uint16_t green;
	uint16_t blue;
	uint16_t alpha;
};

struct StyleDescriptorOffsets {
	uint16_t type = 0;
	ColorOffsets color0;
	ColorOffsets color1;
	ColorOffsets color2;
	uint16_t blend = 0;
	uint16_t premul = 0;
	uint16_t val_1E = 0;//fontIndex
	uint16_t val_20 = 0;
	uint16_t val_22 = 0;
	uint16_t val_24 = 0;
	uint16_t val_26 = 0;
	uint16_t val_28 = 0;
	uint16_t val_2A = 0;
	uint16_t val_2C = 0;
	uint16_t val_2E = 0;
	uint16_t val_30 = 0;
	uint16_t val_32 = 0;
};

#pragma push(pack)
struct RuiPackageHeader_v1_t {
	uint32_t magic;
	uint16_t packageVersion;
	uint16_t ruiVersion;
	uint64_t nameOffset;
	float elementWidth;
	float elementHeight;
	float elementWidthRcp;
	float elementHeightRcp;
	uint16_t defaultValuesSize;
	uint16_t dataStructSize;
	uint16_t styleDescriptorCount;
	uint16_t unk_A4;//unused in r2
	uint16_t renderJobCount;
	uint16_t argClusterCount;
	uint16_t argCount;
	uint16_t mappingCount;
	uint16_t transformDataSize;
	uint16_t nameSize;
	uint16_t rpakPointersInDefaltDataCount;
	uint8_t pad[2];
	uint32_t argNamesSize;
	uint32_t renderJobSize;
	uint32_t mappingSize;
	uint32_t defaultStringsSize;
	uint64_t argNamesOffset;//debug only
	uint64_t argClusterOffset;
	uint64_t argumentsOffset;
	uint64_t styleDescriptorOffset;
	uint64_t renderJobOffset;
	uint64_t mappingOffset;
	uint64_t transformDataOffset;
	uint64_t defaultValuesOffset;
	uint64_t defaultStringDataOffset;
	uint64_t rpakPointersInDefaultDataOffset;
	uint64_t defaultStringsDataSize;
};
#pragma pop(pack)

struct Argument_t
{
	VariableType type;

	uint8_t unk_1;

	uint16_t dataOffset;
	uint16_t nameOffset;

	uint16_t shortHash;

	Argument_t(): type(VariableType::NONE),
		unk_1(0),
		dataOffset(0),
		nameOffset(0),
		shortHash(0)
	{}
};

struct ArgCluster_t
{
	uint16_t argIndex;
	uint16_t argCount;

	uint8_t byte_4;
	uint8_t byte_5;

	uint16_t short_6;
	uint16_t valueSize;
	uint16_t dataStructSize;
	uint16_t short_C;
	uint16_t short_E;
	uint16_t renderJobCount;
};

struct RuiExportPrototype {



	std::string name;
	Vector2 size;
	std::map<std::string,VariableType> arguments;
	std::map<std::string, size_t> varsInDataStruct;

	std::vector<ExportElement<std::string>> codeElements;
	std::vector<ExportElement<uint64_t>> transformCallbacks;
	std::vector<std::function<void(RuiExportPrototype&)>> step2Callbacks;
	uint16_t currentDataStructSize = 0;
	std::map<float, uint16_t> floatConstants;
	std::map<std::string, uint16_t> stringConstants;
	//code export
	std::vector<std::string> codeLines;


	//data struct gen
	std::map<std::string, int> varOffsets;
	std::vector<Mapping> mappings;
	std::map<uint64_t, uint16_t> transformIndices;
	std::vector<StyleDescriptorOffsets> styleDescriptor;
	std::vector<uint8_t> transformData;
	std::vector<uint8_t> renderJobData;
	std::vector<uint8_t> defaultValues;
	std::vector<uint16_t> rpakPointersInDefaultValues;
	std::stringstream defaultStrings;
	uint16_t renderJobCount;
	ArgCluster_t cluster;
	std::vector<Argument_t> exportArgs;

	RuiExportPrototype(const RenderInstance& inst);

	void AddConstant(float f);
	void AddConstant(std::string s);

	void AddTransformData(uint8_t* data, size_t size);

	void AddRenderJobData(uint8_t* data, size_t size);

	void AddDataVariable(const FloatVariable& var);
	void AddDataVariable(const Float2Variable& var);
	void AddDataVariable(const ColorVariable& var);
	void AddDataVariable(const AssetVariable& var);
	void AddDataVariable(const StringVariable& var);

	uint16_t GetFloatDataVariableOffset(const FloatVariable& var);
	Float2Offsets GetFloat2DataVariableOffset(const Float2Variable& var);
	Float3Offsets GetFloat3DataVariableOffset(const Float3Variable& var);
	ColorOffsets GetColorDataVariableOffset(const ColorVariable& var);

	uint16_t GetAssetDataVariableOffset(const AssetVariable& var);
	uint16_t GetStringDataVariableOffset(const StringVariable& var);

	uint16_t GetFloatConstantOffset(float f);
	uint16_t GetStringConstantOffset(std::string s);

	void GenerateCode();
	void GenerateTransformData();
	void GenerateRenderJobData();
	void GenerateVariables(std::map<std::string,std::any>& argValues);
	void GenerateArguments();
	void Generate(std::unordered_map<ImFlow::NodeUID, std::shared_ptr<ImFlow::BaseNode>>& nodes, RenderInstance& render);

	void WriteToFile(fs::path path);

};