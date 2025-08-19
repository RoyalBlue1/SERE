#include "RuiNodeEditor/RuiExportPrototype.h"


RuiExportPrototype::RuiExportPrototype(const RenderInstance& inst):size(inst.elementWidth,inst.elementHeight) {
	name = "testRui";
	renderJobCount = 0;
}

void RuiExportPrototype::AddConstant(float f) {
	if (floatConstants.contains(f))return;
	floatConstants.emplace(f, currentDataStructSize);
	currentDataStructSize += 4;
}

void RuiExportPrototype::AddConstant(std::string s) {
	if (stringConstants.contains(s))return;
	if (currentDataStructSize % 8)currentDataStructSize += 8 - (currentDataStructSize % 8);
	stringConstants.emplace(s, currentDataStructSize);
	currentDataStructSize += 8;
}

void RuiExportPrototype::AddTransformData(uint8_t* data, size_t size) {
	for (size_t i = 0; i < size; i++) {
		transformData.push_back(data[i]);
	}
}

void RuiExportPrototype::AddRenderJobData(uint8_t* data, size_t size) {
	for (size_t i = 0; i < size; i++) {
		renderJobData.push_back(data[i]);
	}
}

void RuiExportPrototype::AddDataVariable(const FloatVariable& var) {
	if (var.IsConstant()) {
		AddConstant(var.value);
	}
	else {
		varsInDataStruct.emplace(var.name, 4);
	}
}

void RuiExportPrototype::AddDataVariable(const Float2Variable& var) {
	if (var.IsConstant()) {
		AddConstant(var.value.x);
		AddConstant(var.value.y);
	}
	else {
		varsInDataStruct.emplace(var.name, 8);
	}
}

void RuiExportPrototype::AddDataVariable(const ColorVariable& var) {
	if (var.IsConstant()) {
		AddConstant(var.value.red);
		AddConstant(var.value.green);
		AddConstant(var.value.blue);
		AddConstant(var.value.alpha);
	}
	else {
		varsInDataStruct.emplace(var.name, 16);
	}
}

void RuiExportPrototype::AddDataVariable(const AssetVariable& var) {
	varsInDataStruct.emplace(var.name, 4);
}

void RuiExportPrototype::AddDataVariable(const StringVariable& var) {
	if (var.IsConstant()) {
		AddConstant(var.value);
	}
	else {
		varsInDataStruct.emplace(var.name, 8);
	}
}

uint16_t RuiExportPrototype::GetFloatDataVariableOffset(const FloatVariable& var) {
	if (var.IsConstant())
		return GetFloatConstantOffset(var.value);
	return varOffsets[var.name];
}

Float2Offsets RuiExportPrototype::GetFloat2DataVariableOffset(const Float2Variable& var) {
	Float2Offsets res;
	if (var.IsConstant()) {
		res.x = GetFloatConstantOffset(var.value.x);
		res.y = GetFloatConstantOffset(var.value.y);
		return res;
	}
	res.x = varOffsets[var.name];
	res.y = res.x + 4;
	return res;
}

Float3Offsets RuiExportPrototype::GetFloat3DataVariableOffset(const Float3Variable& var) {
	Float3Offsets res;
	if (var.IsConstant()) {
		res.x = GetFloatConstantOffset(var.value.x);
		res.y = GetFloatConstantOffset(var.value.y);
		res.z = GetFloatConstantOffset(var.value.z);
		return res;
	}
	res.x = varOffsets[var.name];
	res.y = res.x + 4;
	res.z = res.y + 4;
	return res;
}

ColorOffsets RuiExportPrototype::GetColorDataVariableOffset(const ColorVariable& var) {
	ColorOffsets res;
	if (var.IsConstant()) {
		res.red = GetFloatConstantOffset(var.value.red);
		res.green = GetFloatConstantOffset(var.value.green);
		res.blue = GetFloatConstantOffset(var.value.blue);
		res.alpha = GetFloatConstantOffset(var.value.alpha);
		return res;
	}
	res.red = varOffsets[var.name];
	res.green = res.red + 4;
	res.blue = res.green + 4;
	res.alpha = res.blue + 4;
	return res;
}

uint16_t RuiExportPrototype::GetAssetDataVariableOffset(const AssetVariable& var) {
	return varOffsets[var.name];
}

uint16_t RuiExportPrototype::GetStringDataVariableOffset(const StringVariable& var) {
	if (var.IsConstant())
		return GetStringConstantOffset(var.value);
	return varOffsets[var.name];
}


uint16_t RuiExportPrototype::GetFloatConstantOffset(float f) {
	assert(floatConstants.contains(f) && "Float not found in Float Constants");
	return floatConstants[f];
}

uint16_t RuiExportPrototype::GetStringConstantOffset(std::string s) {
	assert(stringConstants.contains(s) && "Float not found in Float Constants");
	return stringConstants[s];
}

void RuiExportPrototype::GenerateCode() {
	std::set<std::string> existingVariables;
	for (auto& [arg, _] : arguments) {
		existingVariables.emplace(arg);
	}
	bool addedVar = true;
	while (addedVar) {
		addedVar = false;
		for (auto& ele : codeElements) {
			if (existingVariables.contains(ele.identifier))continue;
			bool dependencyMissing = false;
			for (auto& dep : ele.dependencys) {
				if (!dep.size())continue;
				if (!existingVariables.contains(dep)) {
					dependencyMissing = true;
					break;
				}
			}
			if (dependencyMissing)continue;
			ele.callback(*this);
			existingVariables.insert(ele.identifier);
			addedVar = true;
		}
	}

}

void RuiExportPrototype::GenerateTransformData() {
	bool addedVar = true;
	while (addedVar) {
		addedVar = false;
		for (auto& ele : transformCallbacks) {
			if (transformIndices.contains(ele.identifier))continue;
			bool dependencyMissing = false;
			for (auto& dep : ele.dependencys) {
				if (!transformIndices.contains(dep)) {
					dependencyMissing = true;
					break;
				}
			}
			if (dependencyMissing)continue;
			ele.callback(*this);
			addedVar = true;
		}
	}
}

void RuiExportPrototype::GenerateRenderJobData() {
	for (auto& callback : step2Callbacks) {
		callback(*this);
	}
}

uint32_t calculateShortHash(const char* name, uint32_t mul, uint32_t add) {
	uint32_t i = 0;
	uint32_t hash = 0;
	while (name[i]) {
		hash *= mul;
		hash += add + name[i];
		i++;
	}
	hash = hash ^ (hash >> 4);
	return hash;
	//shorthash = (unsigned short)(hash>>4);
	//posistion in array = (cluster.argIndex + (hash & (cluster.argCount -1)));
}

void RuiExportPrototype::GenerateArguments() {

	cluster.argCount = 1;
	cluster.argIndex = 0;
	cluster.short_6 = 0;
	cluster.dataStructSize = currentDataStructSize;
	cluster.short_C = 1;
	cluster.short_E = 0;
	cluster.renderJobCount = renderJobCount;
	while (cluster.argCount < arguments.size())cluster.argCount *= 2;

	for (int add = 0; add < 256; add++) {
		bool success = true;
		for (int mul = 1; mul < 256; mul++) {
			success = true;
			std::vector<bool> argSlots(cluster.argCount, false);
			for (auto& [name,type] : arguments) {
				uint32_t argIndex = calculateShortHash(name.c_str(), mul, add) & (cluster.argCount - 1);
				if (argSlots[argIndex]) {
					success = false;
					break;
				}
				argSlots[argIndex] = true;
			}
			if (success) {
				cluster.byte_4 = mul;
				cluster.byte_5 = add;
				break;
			}
		}
		if(success)break;
	}
	exportArgs.resize(cluster.argCount);
	for (auto& [name, type] : arguments) {
		uint32_t hash = calculateShortHash(name.c_str(),cluster.byte_4,cluster.byte_5);
		uint32_t index = hash & (cluster.argCount -1);
		exportArgs[index].type = type;
		exportArgs[index].dataOffset = varOffsets[name];
		exportArgs[index].nameOffset = 0;
		exportArgs[index].shortHash = hash >> 4;
		
	}
}

void RuiExportPrototype::GenerateVariables(std::map<std::string,std::any>& argValues) {
	defaultValues.resize(currentDataStructSize);
	for (auto& [value, offset] : floatConstants) {
		*(float*)&defaultValues[offset] = value;
	}
	for (auto& [value, offset] : stringConstants) {
		*(float*)&defaultValues[offset] = defaultStrings.str().size();
		rpakPointersInDefaultValues.push_back(offset);
		defaultStrings.write(value.c_str(),value.size());
		defaultStrings.put(0);
	}
	for (auto& [name, type] : arguments) {
		size_t size;
		switch (type) {
		case VariableType::STRING:
		case VariableType::ASSET:
		case VariableType::IMAGE:
		{
			if (((type == VariableType::ASSET) || (type == VariableType::STRING)) && (currentDataStructSize % 8)) {
				currentDataStructSize += 8 - (currentDataStructSize % 8);
			}
			size = 8;
			defaultValues.resize(currentDataStructSize + size);
			*(size_t*)&defaultValues[currentDataStructSize] = defaultStrings.str().size();
			rpakPointersInDefaultValues.push_back(currentDataStructSize);
			std::string val = std::any_cast<std::string>(argValues[name]);
			defaultStrings.write(val.c_str(), val.size());
			defaultStrings.put(0);
		}
		break;
		case VariableType::INT:
		case VariableType::BOOL:
			size = 4;
			*(int*)&defaultValues[currentDataStructSize] = std::any_cast<int>(argValues[name]);
			break;
		case VariableType::FLOAT:
		case VariableType::GAMETIME:
		case VariableType::FLOAT_UNK:
			size = 4;
			*(float*)&defaultValues[currentDataStructSize] = std::any_cast<float>(argValues[name]);
			break;
		case VariableType::FLOAT2:
			size = 8;
			*(Vector2*)&defaultValues[currentDataStructSize] = std::any_cast<Vector2>(argValues[name]);
			break;
		case VariableType::FLOAT3:
			size = 12;
			*(Vector3*)&defaultValues[currentDataStructSize] = std::any_cast<Vector3>(argValues[name]);
			break;
		case VariableType::COLOR_ALPHA:
			size = 16;
			*(Color*)&defaultValues[currentDataStructSize] = std::any_cast<Color>(argValues[name]);
			break;
		default:
			size = 0;
			break;
		}

		varOffsets.emplace(name, currentDataStructSize);
		currentDataStructSize += size;
	}


	for (auto& [name, size] : varsInDataStruct) {
		if ((size == 8) && (currentDataStructSize % 8)) {
			currentDataStructSize += 8 - (currentDataStructSize % 8);
		}
		varOffsets.emplace(name, currentDataStructSize);
		currentDataStructSize += size;
	}


}

void RuiExportPrototype::Generate(std::unordered_map<ImFlow::NodeUID, std::shared_ptr<ImFlow::BaseNode>>& nodes, RenderInstance& render) {
	for (int i = 0; i < 3; i++)
		transformIndices.emplace(render.transformResults[i].hash, i);
	for (auto& [uid, node] : nodes) {
		std::dynamic_pointer_cast<RuiBaseNode>(node)->Export(*this);
	}
	GenerateVariables(render.arguments);
	GenerateTransformData();
	GenerateRenderJobData();
	GenerateCode();
	for (auto& line : codeLines) {
		printf("%s\n", line.c_str());
	}
}


void RuiExportPrototype::WriteToFile(fs::path path) {
	std::ofstream file(path);
	if(!file.good())return;

	RuiPackageHeader_v1_t pkgHdr;
	pkgHdr.magic = 'R' | 'U' << 8 | 'I' << 16 | 'P' << 24;
	pkgHdr.packageVersion = 1;
	pkgHdr.ruiVersion = 30;
	pkgHdr.elementWidth = size.x;
	pkgHdr.elementWidthRcp = NRReciprocal(size.x);
	pkgHdr.elementHeight = size.y;
	pkgHdr.elementHeightRcp = NRReciprocal(size.y);
	pkgHdr.nameOffset = sizeof(pkgHdr);
	pkgHdr.nameSize = name.size() + 1;
	pkgHdr.defaultValuesSize = defaultValues.size();
	pkgHdr.dataStructSize = currentDataStructSize;
	pkgHdr.defaultStringsDataSize = defaultStrings.str().size();
	pkgHdr.styleDescriptorCount = styleDescriptor.size();
	pkgHdr.renderJobSize = renderJobData.size();
	pkgHdr.transformDataSize = transformData.size();
	pkgHdr.rpakPointersInDefaltDataCount = rpakPointersInDefaultValues.size();
	pkgHdr.mappingCount = mappings.size();
	pkgHdr.mappingSize = 0;
	pkgHdr.renderJobCount = renderJobCount;
	pkgHdr.argClusterCount = 1;
	pkgHdr.argCount = cluster.argCount;
	pkgHdr.unk_A4 = 0;
	pkgHdr.argNamesSize = 0;

	pkgHdr.defaultValuesOffset = pkgHdr.nameOffset + pkgHdr.nameSize;
	pkgHdr.defaultStringDataOffset = pkgHdr.defaultValuesOffset + pkgHdr.defaultValuesSize;
	pkgHdr.rpakPointersInDefaultDataOffset = pkgHdr.defaultStringDataOffset + pkgHdr.defaultStringsDataSize;
	pkgHdr.styleDescriptorOffset = pkgHdr.rpakPointersInDefaultDataOffset + pkgHdr.rpakPointersInDefaltDataCount * sizeof(uint16_t);
	pkgHdr.renderJobOffset = pkgHdr.styleDescriptorOffset + pkgHdr.styleDescriptorCount * sizeof(StyleDescriptorOffsets);
	pkgHdr.transformDataOffset = pkgHdr.renderJobOffset + pkgHdr.renderJobSize;
	pkgHdr.argumentsOffset = pkgHdr.transformDataOffset + pkgHdr.transformDataSize;
	pkgHdr.argClusterOffset = pkgHdr.argumentsOffset + pkgHdr.argCount * sizeof(Argument_t);
	pkgHdr.argNamesOffset = pkgHdr.argClusterOffset + pkgHdr.argClusterCount * sizeof(ArgCluster_t);
	pkgHdr.mappingOffset = pkgHdr.argNamesOffset + pkgHdr.argNamesSize;		

	file.write((char*)&pkgHdr,sizeof(pkgHdr));
	file.write(name.c_str(),name.size());
	file.put(0);
	file.write((char*)defaultValues.data(),defaultValues.size());
	file.write((char*)defaultStrings.str().c_str(),defaultStrings.str().size());
	file.write((char*)rpakPointersInDefaultValues.data(),rpakPointersInDefaultValues.size()*sizeof(uint16_t));
	file.write((char*)styleDescriptor.data(),styleDescriptor.size()*sizeof(StyleDescriptorOffsets));
	file.write((char*)renderJobData.data(),renderJobData.size());
	file.write((char*)transformData.data(),transformData.size());
	file.write((char*)exportArgs.data(),exportArgs.size()*sizeof(Argument_t));
	file.write((char*)&cluster,sizeof(cluster));
	file.close();
}