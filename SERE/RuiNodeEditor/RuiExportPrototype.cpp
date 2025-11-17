#include "RuiNodeEditor/RuiExportPrototype.h"


RuiExportPrototype::RuiExportPrototype(const RenderInstance& inst,const std::string& name):size(inst.elementWidth,inst.elementHeight),name(name) {
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
		varsInDataStruct.emplace(var.name, VariableType::FLOAT);
	}
}

void RuiExportPrototype::AddDataVariable(const Float2Variable& var) {
	if (var.IsConstant()) {
		AddConstant(var.value.x);
		AddConstant(var.value.y);
	}
	else {
		varsInDataStruct.emplace(var.name, VariableType::FLOAT2);
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
		varsInDataStruct.emplace(var.name, VariableType::COLOR_ALPHA);
	}
}

void RuiExportPrototype::AddDataVariable(const AssetVariable& var) {
	varsInDataStruct.emplace(var.name, VariableType::ASSET_HANDLE);
}

void RuiExportPrototype::AddDataVariable(const StringVariable& var) {
	if (var.IsConstant()) {
		AddConstant(var.value);
	}
	else {
		varsInDataStruct.emplace(var.name, VariableType::STRING);
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

	codeLines.push_back(std::format("extern \"C\" __declspec(dllexport) void {}(RuiFunctions_t* funcs,RuiGlobals* globals,RuiInstance* inst,{}_data* data){{",name,name));
	codeLines.push_back("__m128* transformSize = funcs->GetTransformSize(inst);");
	size_t codeElementsHit = 0;
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
			codeElementsHit++;
		}
	}
	if (codeElementsHit < codeElements.size()) {
		printf("Not all code has been exported should be %llu was %llu\n",codeElements.size(),codeElementsHit);
		for (auto& ele : codeElements) {
			if (existingVariables.find(ele.identifier) == existingVariables.end()) {
				printf("could not add %s missing",ele.identifier.c_str());
				for (auto& dep : ele.dependencys) {
					if (existingVariables.find(dep) == existingVariables.end()) {
						printf(" %s",dep.c_str());
					}
				}
				printf("\n");
			}
		}
		printf("Error done");
	}
		

	codeLines.push_back(std::format("funcs->executeTransform(inst,{});",transformData.size()));
	codeLines.push_back("}");

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
	std::sort(renderJobs.begin(), renderJobs.end(), [](ExportRenderJob& a, ExportRenderJob& b) {
		return a.layer<b.layer;
	});
	for (auto& job : renderJobs) {
		job.func(*this);
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
			if (currentDataStructSize % 8) {
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
			defaultValues.resize(currentDataStructSize + size);
			*(int*)&defaultValues[currentDataStructSize] = std::any_cast<int>(argValues[name]);
			break;
		case VariableType::FLOAT:
		case VariableType::GAMETIME:
		case VariableType::FLOAT_UNK:
			size = 4;
			defaultValues.resize(currentDataStructSize + size);
			*(float*)&defaultValues[currentDataStructSize] = std::any_cast<float>(argValues[name]);
			break;
		case VariableType::FLOAT2:
			size = 8;
			defaultValues.resize(currentDataStructSize + size);
			*(Vector2*)&defaultValues[currentDataStructSize] = std::any_cast<Vector2>(argValues[name]);
			break;
		case VariableType::FLOAT3:
			size = 12;
			defaultValues.resize(currentDataStructSize + size);
			*(Vector3*)&defaultValues[currentDataStructSize] = std::any_cast<Vector3>(argValues[name]);
			break;
		case VariableType::COLOR_ALPHA:
			size = 16;
			defaultValues.resize(currentDataStructSize + size);
			*(Color*)&defaultValues[currentDataStructSize] = std::any_cast<Color>(argValues[name]);
			break;
		default:
			size = 0;
			break;
		}

		varOffsets.emplace(name, currentDataStructSize);
		currentDataStructSize += (uint16_t)size;
	}


	for (auto& [name, type] : varsInDataStruct) {
		if(varOffsets.contains(name))
			continue;
		if ((type == VariableType::STRING) && (currentDataStructSize % 8)) {
			currentDataStructSize += 8 - (currentDataStructSize % 8);
		}
		varOffsets.emplace(name, currentDataStructSize);
		size_t size;
		switch (type) {
		case VariableType::INT:
		case VariableType::BOOL:
		case VariableType::FLOAT:
		case VariableType::GAMETIME:
		case VariableType::FLOAT_UNK:
		case VariableType::ASSET_HANDLE:
			size = 4;
			break;
		case VariableType::FLOAT2:
		case VariableType::STRING:
			size = 8;
			break;
		case VariableType::FLOAT3:
			size = 12;
			break;
		case VariableType::COLOR_ALPHA:
			size = 16;
			break;
		default:
			size = 0;
			break;
		}
		currentDataStructSize += size;
	}


}

bool RuiExportPrototype::GenerateCodeStruct() {
	codeLines.push_back(std::format("struct {}_data{{",name));
	
	if (varOffsets.size() == 0) {
		codeLines.push_back(std::format("_BYTE constants[{}];",currentDataStructSize));
	}
	else {

		size_t currentOffset = 0;
		size_t addedVars = 0;
		while (addedVars < varOffsets.size()) {
			std::string lowestVar;
			for (auto& [name, offset] : varOffsets) {
				if (offset >= currentOffset) {
					lowestVar = name;
					break;
				}
			}
			for (auto& [name, offset] : varOffsets) {
				if (offset < varOffsets[lowestVar] && offset >= currentOffset)
					lowestVar = name;
			}
			VariableType type = VariableType::NONE;
			if(arguments.contains(lowestVar))
				type = arguments[lowestVar];
			else if(varsInDataStruct.contains(lowestVar))
				type = varsInDataStruct[lowestVar];
			if(type == VariableType::NONE)
				return false;
			if (currentOffset < varOffsets[lowestVar]) {
				codeLines.push_back(std::format("_BYTE pad_{}[{}];",lowestVar,varOffsets[lowestVar]-currentOffset));
				currentOffset = varOffsets[lowestVar];
			}
			switch (type) {
			case VariableType::BOOL:
			case VariableType::INT:
				codeLines.push_back(std::format("int {};",lowestVar));
				currentOffset +=4;
				break;
			case VariableType::FLOAT:
			case VariableType::GAMETIME:
			case VariableType::FLOAT_UNK:
				codeLines.push_back(std::format("float {};",lowestVar));
				currentOffset +=4;
				break;
			case VariableType::FLOAT2:
				codeLines.push_back(std::format("Vector2 {};",lowestVar));
				currentOffset +=8;
				break;
			case VariableType::FLOAT3:
				codeLines.push_back(std::format("Vector3 {};",lowestVar));
				currentOffset +=12;
				break;
			case VariableType::COLOR_ALPHA:
				codeLines.push_back(std::format("Color {};",lowestVar));
				currentOffset +=16;
				break;
			case VariableType::STRING:
			case VariableType::ASSET:
			case VariableType::IMAGE:
				codeLines.push_back(std::format("const char* {};",lowestVar));
				currentOffset +=8;
				break;
			case VariableType::ASSET_HANDLE:
				codeLines.push_back(std::format("uint32_t {};",lowestVar));
				currentOffset +=4;
				break;
			}
			addedVars++;
		}

	}
	codeLines.push_back("};");
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
	GenerateArguments();
	GenerateCodeStruct();
	GenerateCode();
}


void RuiExportPrototype::WriteToFile(fs::path path) {
	std::ofstream file(path,std::ios::binary);
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
	pkgHdr.defaultValuesSize = (uint16_t)defaultValues.size();
	pkgHdr.dataStructSize = currentDataStructSize;
	pkgHdr.defaultStringsDataSize = defaultStrings.str().size();
	pkgHdr.styleDescriptorCount = (uint16_t)styleDescriptor.size();
	pkgHdr.renderJobSize = (uint16_t)renderJobData.size();
	pkgHdr.transformDataSize = (uint16_t)transformData.size();
	pkgHdr.rpakPointersInDefaltDataCount = (uint16_t)rpakPointersInDefaultValues.size();
	pkgHdr.mappingCount = (uint16_t)mappings.size();
	pkgHdr.mappingSize = 0;
	pkgHdr.renderJobCount = renderJobCount;
	pkgHdr.argClusterCount = 1;
	pkgHdr.argCount = cluster.argCount;
	pkgHdr.unk_A4 = 0;
	pkgHdr.argNamesSize = 0;

	//pkgHdr.defaultValuesOffset = pkgHdr.nameOffset + pkgHdr.nameSize;
	//pkgHdr.defaultStringDataOffset = pkgHdr.defaultValuesOffset + pkgHdr.defaultValuesSize;
	//pkgHdr.rpakPointersInDefaultDataOffset = pkgHdr.defaultStringDataOffset + pkgHdr.defaultStringsDataSize;
	//pkgHdr.styleDescriptorOffset = pkgHdr.rpakPointersInDefaultDataOffset + pkgHdr.rpakPointersInDefaltDataCount * sizeof(uint16_t);
	//pkgHdr.renderJobOffset = pkgHdr.styleDescriptorOffset + pkgHdr.styleDescriptorCount * sizeof(StyleDescriptorOffsets);
	//pkgHdr.transformDataOffset = pkgHdr.renderJobOffset + pkgHdr.renderJobSize;
	//pkgHdr.argumentsOffset = pkgHdr.transformDataOffset + pkgHdr.transformDataSize;
	//pkgHdr.argClusterOffset = pkgHdr.argumentsOffset + pkgHdr.argCount * sizeof(Argument_t);
	//pkgHdr.argNamesOffset = pkgHdr.argClusterOffset + pkgHdr.argClusterCount * sizeof(ArgCluster_t);
	//pkgHdr.mappingOffset = pkgHdr.argNamesOffset + pkgHdr.argNamesSize;		

	file.write((char*)&pkgHdr,sizeof(pkgHdr));

	pkgHdr.nameOffset = file.tellp();
	file.write(name.c_str(),name.size());
	file.put(0);

	pkgHdr.defaultValuesOffset = file.tellp();
	file.write((char*)defaultValues.data(),defaultValues.size());

	pkgHdr.defaultStringDataOffset = file.tellp();
	file.write((char*)defaultStrings.str().c_str(),defaultStrings.str().size());

	pkgHdr.rpakPointersInDefaultDataOffset = file.tellp();
	file.write((char*)rpakPointersInDefaultValues.data(),rpakPointersInDefaultValues.size()*sizeof(uint16_t));

	pkgHdr.styleDescriptorOffset = file.tellp();
	file.write((char*)styleDescriptor.data(),styleDescriptor.size()*sizeof(StyleDescriptorOffsets));

	pkgHdr.renderJobOffset = file.tellp();
	file.write((char*)renderJobData.data(),renderJobData.size());

	pkgHdr.transformDataOffset = file.tellp();
	file.write((char*)transformData.data(),transformData.size());

	pkgHdr.argumentsOffset = file.tellp();
	file.write((char*)exportArgs.data(),exportArgs.size()*sizeof(Argument_t));

	pkgHdr.argClusterOffset = file.tellp();
	file.write((char*)&cluster,sizeof(cluster));

	file.seekp(0);
	file.write((char*) &pkgHdr, sizeof(pkgHdr));

	file.close();
	fs::path codeFilePath = path.replace_extension("cpp");
	std::ofstream codeFile(codeFilePath);
	if(!codeFile.good())
		return;
	const std::string import = "#include \"RuiHeaders.h\"";
	codeFile.write(import.c_str(),import.size());
	codeFile.put('\n');
	for (auto& line : codeLines) {
		codeFile.write(line.c_str(),line.size());
		codeFile.put('\n');
	}
}