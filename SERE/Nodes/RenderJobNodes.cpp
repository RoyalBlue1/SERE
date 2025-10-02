#include "RenderJobNodes.h"
#include "RuiRendering/RenderManager.h"







AssetRenderNode::AssetRenderNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style),maskFlag(false) {

	getIn<TransformResult>("Transform")->setEmptyVal(render.transformResults[2]);
}

AssetRenderNode::AssetRenderNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :AssetRenderNode(rend, style) {}

void AssetRenderNode::draw() {

	ImGui::Checkbox("Mask mode",&maskFlag);
	
	AssetInputData input{};
	input.mainColor = getInVal<ColorVariable>("Main Color");
	input.maskColor = getInVal<ColorVariable>("Mask Color");
	input.tertColor = getInVal<ColorVariable>("Tertiary Color");
	input.mainAsset = getInVal<AssetVariable>("Main Asset");
	input.maskAsset = getInVal<AssetVariable>("Mask Asset");
	input.mins = getInVal<Float2Variable>("Mins");
	input.maxs = getInVal<Float2Variable>("Maxs");
	input.texMins = getInVal<Float2Variable>("Texture Mins");
	input.texMaxs = getInVal<Float2Variable>("Texture Maxs");
	input.blend = getInVal<FloatVariable>("Blend");
	input.premul = getInVal<FloatVariable>("Premul");
	input.maskCenter = getInVal<Float2Variable>("Mask Center");
	input.maskTranslate = getInVal<Float2Variable>("Mask Translate");
	input.maskSize = getInVal<Float2Variable>("Mask Size");
	input.maskRotation = getInVal<FloatVariable>("Mask Rotation");
	input.transform = getInVal<TransformResult>("Transform");
	input.flags = 0x1000 | maskFlag;
	Render_Asset(render,input);
}

void AssetRenderNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void AssetRenderNode::Export(RuiExportPrototype& proto) {
	proto.renderJobCount++;
	AssetInputData input{};
	input.mainColor = getInVal<ColorVariable>("Main Color");
	input.maskColor = getInVal<ColorVariable>("Mask Color");
	input.tertColor = getInVal<ColorVariable>("Tertiary Color");
	input.mainAsset = getInVal<AssetVariable>("Main Asset");
	input.maskAsset = getInVal<AssetVariable>("Mask Asset");
	input.mins = getInVal<Float2Variable>("Mins");
	input.maxs = getInVal<Float2Variable>("Maxs");
	input.texMins = getInVal<Float2Variable>("Texture Mins");
	input.texMaxs = getInVal<Float2Variable>("Texture Maxs");
	input.blend = getInVal<FloatVariable>("Blend");
	input.premul = getInVal<FloatVariable>("Premul");
	input.maskCenter = getInVal<Float2Variable>("Mask Center");
	input.maskTranslate = getInVal<Float2Variable>("Mask Translate");
	input.maskSize = getInVal<Float2Variable>("Mask Size");
	input.maskRotation = getInVal<FloatVariable>("Mask Rotation");
	input.transform = getInVal<TransformResult>("Transform");
	input.flags = 0x1000 | maskFlag;


	if (!input.mainAsset.name.size()) {
		std::string mainAssetFallback = Variable::UniqueName();
		input.mainAsset.name = mainAssetFallback;
		ExportElement<std::string> ele;
		ele.dependencys = { };
		ele.identifier = mainAssetFallback;
		ele.callback =[mainAssetFallback](RuiExportPrototype& proto) {

			proto.codeLines.push_back(std::format("data->{} = funcs->LoadAsset(inst,\"white\");",mainAssetFallback));
		};
		proto.codeElements.push_back(ele);
	}


	if (!input.maskAsset.name.size()) {
		std::string maskAssetFallback = Variable::UniqueName();
		input.maskAsset.name = maskAssetFallback;
		ExportElement<std::string> ele;
		ele.dependencys = { };
		ele.identifier = maskAssetFallback;
		ele.callback =[maskAssetFallback](RuiExportPrototype& proto) {
			
			proto.codeLines.push_back(std::format("data->{} = -1;",maskAssetFallback));
		};
		proto.codeElements.push_back(ele);
	}

	proto.AddDataVariable(input.mainColor);
	proto.AddDataVariable(input.maskColor);
	proto.AddDataVariable(input.tertColor);
	proto.AddDataVariable(input.mainAsset);
	proto.AddDataVariable(input.maskAsset);
	proto.AddDataVariable(input.mins);
	proto.AddDataVariable(input.maxs);
	proto.AddDataVariable(input.texMins);
	proto.AddDataVariable(input.texMaxs);
	proto.AddDataVariable(input.blend);
	proto.AddDataVariable(input.premul);
	proto.AddDataVariable(input.maskCenter);
	proto.AddDataVariable(input.maskTranslate);
	proto.AddDataVariable(input.maskSize);
	proto.AddDataVariable(input.maskRotation);


	

	proto.step2Callbacks.push_back([input](RuiExportPrototype& proto) {
		StyleDescriptorOffsets style;
		style.type = 1;
		style.color0 = proto.GetColorDataVariableOffset(input.mainColor);
		style.color1 = proto.GetColorDataVariableOffset(input.maskColor);
		style.color2 = proto.GetColorDataVariableOffset(input.tertColor);
		style.blend = proto.GetFloatDataVariableOffset(input.blend);
		style.premul = proto.GetFloatDataVariableOffset(input.premul);
		uint16_t styleId = proto.styleDescriptor.size();
		proto.styleDescriptor.push_back(style);
		struct AssetRenderOffsets {
			uint16_t type = 1;
			uint16_t transformIndex;
			uint16_t assetIndex_0;
			uint16_t assetIndex_1;
			Float2Offsets mins;
			Float2Offsets maxs;
			Float2Offsets texMins;
			Float2Offsets texMaxs;
			Float2Offsets maskCenter;
			uint16_t maskRotation;
			Float2Offsets maskTranslate;
			Float2Offsets maskSize;
			uint16_t flags;
			uint8_t styleIndex;
			char pad_29 = 0;
		};
		AssetRenderOffsets rend{};
		rend.transformIndex = proto.transformIndices[input.transform.hash];
		rend.assetIndex_0 = proto.GetAssetDataVariableOffset(input.mainAsset);
		rend.assetIndex_1 = proto.GetAssetDataVariableOffset(input.maskAsset);
		rend.mins = proto.GetFloat2DataVariableOffset(input.mins);
		rend.maxs = proto.GetFloat2DataVariableOffset(input.maxs);
		rend.texMins = proto.GetFloat2DataVariableOffset(input.texMins);
		rend.texMaxs = proto.GetFloat2DataVariableOffset(input.texMaxs);
		rend.maskCenter = proto.GetFloat2DataVariableOffset(input.maskCenter);
		rend.maskRotation = proto.GetFloatDataVariableOffset(input.maskRotation);
		rend.maskTranslate = proto.GetFloat2DataVariableOffset(input.maskTranslate);
		rend.maskSize = proto.GetFloat2DataVariableOffset(input.maskSize);
		rend.flags = input.flags;
		rend.styleIndex = styleId;
		proto.AddRenderJobData((uint8_t*)& rend, sizeof(rend));

	});
}

std::vector<std::shared_ptr<ImFlow::PinProto>> AssetRenderNode::GetPinInfo() {
	static std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	if(info.size())return info;
	info.push_back(std::make_shared<ImFlow::InPinProto<ColorVariable>>("Main Color",ImFlow::ConnectionFilter::SameType(),ColorVariable(1.f,1.f,1.f,1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<ColorVariable>>("Mask Color",ImFlow::ConnectionFilter::SameType(),ColorVariable(1.f,1.f,1.f,1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<ColorVariable>>("Tertiary Color",ImFlow::ConnectionFilter::SameType(),ColorVariable(1.f,1.f,1.f,1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<AssetVariable>>("Main Asset",ImFlow::ConnectionFilter::SameType(),AssetVariable("white")));
	info.push_back(std::make_shared<ImFlow::InPinProto<AssetVariable>>("Mask Asset",ImFlow::ConnectionFilter::SameType(),AssetVariable("")));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Mins",ImFlow::ConnectionFilter::SameType(),Float2Variable(0.f,0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Maxs",ImFlow::ConnectionFilter::SameType(),Float2Variable(1.f,1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Texture Mins",ImFlow::ConnectionFilter::SameType(),Float2Variable(0.f,0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Texture Maxs",ImFlow::ConnectionFilter::SameType(),Float2Variable(1.f,1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Blend",ImFlow::ConnectionFilter::SameType(),FloatVariable(1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Premul",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Mask Center",ImFlow::ConnectionFilter::SameType(),Float2Variable(.5f,.5f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Mask Translate",ImFlow::ConnectionFilter::SameType(),Float2Variable(0.f,0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Mask Size",ImFlow::ConnectionFilter::SameType(),Float2Variable(1.f,1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Mask Rotation",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<TransformResult>>("Transform",ImFlow::ConnectionFilter::SameType(),TransformResult()));
	return info;
}

AssetCircleRenderNode::AssetCircleRenderNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getIn<TransformResult>("Transform")->setEmptyVal(render.transformResults[2]);
}

AssetCircleRenderNode::AssetCircleRenderNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :AssetCircleRenderNode(rend, style) {}

void AssetCircleRenderNode::draw() {
	AssetCircleInputData input{};
	input.mainColor = getInVal<ColorVariable>("Main Color");
	input.scndColor = getInVal<ColorVariable>("Secondary Color");
	input.tertColor = getInVal<ColorVariable>("Tertiary Color");
	input.mainAsset = getInVal<AssetVariable>("Asset");
	input.blend = getInVal<FloatVariable>("Blend");
	input.premul = getInVal<FloatVariable>("Premul");
	input.mins = getInVal<Float2Variable>("Mins");
	input.maxs = getInVal<Float2Variable>("Maxs");
	input.texMins = getInVal<Float2Variable>("Texture Mins");
	input.texMaxs = getInVal<Float2Variable>("Texture Maxs");
	input.innerSliceBlend = getInVal<FloatVariable>("Inner Slice Blend");
	input.sliceBegin = getInVal<FloatVariable>("Slice Begin");
	input.sliceEnd = getInVal<FloatVariable>("Slice End");
	input.ellipseSize = getInVal<Float2Variable>("Ellipse Size");
	input.innerMask = getInVal<FloatVariable>("Inner Mask");
	input.vingette = getInVal<FloatVariable>("Vingette");

	input.transform = getInVal<TransformResult>("Transform");
	input.flags = 0x2000;
	Render_AssetSmall(render,input);
}

void AssetCircleRenderNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void AssetCircleRenderNode::Export(RuiExportPrototype& proto) {
	proto.renderJobCount++;
	AssetCircleInputData input{};
	input.mainColor = getInVal<ColorVariable>("Main Color");
	input.scndColor = getInVal<ColorVariable>("Secondary Color");
	input.tertColor = getInVal<ColorVariable>("Tertiary Color");
	input.mainAsset = getInVal<AssetVariable>("Asset");
	input.blend = getInVal<FloatVariable>("Blend");
	input.premul = getInVal<FloatVariable>("Premul");
	input.mins = getInVal<Float2Variable>("Mins");
	input.maxs = getInVal<Float2Variable>("Maxs");
	input.texMins = getInVal<Float2Variable>("Texture Mins");
	input.texMaxs = getInVal<Float2Variable>("Texture Maxs");
	input.innerSliceBlend = getInVal<FloatVariable>("Inner Slice Blend");
	input.sliceBegin = getInVal<FloatVariable>("Slice Begin");
	input.sliceEnd = getInVal<FloatVariable>("Slice End");
	input.ellipseSize = getInVal<Float2Variable>("Ellipse Size");
	input.innerMask = getInVal<FloatVariable>("Inner Mask");
	input.vingette = getInVal<FloatVariable>("Vingette");

	input.transform = getInVal<TransformResult>("Transform");
	input.flags = 0x2000;

	proto.AddDataVariable(input.mainColor);
	proto.AddDataVariable(input.scndColor);
	proto.AddDataVariable(input.tertColor);
	proto.AddDataVariable(input.mainAsset);
	proto.AddDataVariable(input.blend);
	proto.AddDataVariable(input.premul);
	proto.AddDataVariable(input.mins);
	proto.AddDataVariable(input.maxs);
	proto.AddDataVariable(input.texMins);
	proto.AddDataVariable(input.innerSliceBlend);
	proto.AddDataVariable(input.sliceBegin);
	proto.AddDataVariable(input.sliceEnd);
	proto.AddDataVariable(input.ellipseSize);
	proto.AddDataVariable(input.innerMask);
	proto.AddDataVariable(input.vingette);



	proto.step2Callbacks.push_back([input](RuiExportPrototype& proto) {
		StyleDescriptorOffsets style;
		style.type = 2;
		style.color0 = proto.GetColorDataVariableOffset(input.mainColor);
		style.color1 = proto.GetColorDataVariableOffset(input.scndColor);
		style.color2 = proto.GetColorDataVariableOffset(input.tertColor);
		style.blend = proto.GetFloatDataVariableOffset(input.blend);
		style.premul = proto.GetFloatDataVariableOffset(input.premul);
		style._anon_0 = proto.GetFloatDataVariableOffset(input.innerSliceBlend);
		style._anon_1 = proto.GetFloatDataVariableOffset(input.sliceBegin);
		style._anon_2 = proto.GetFloatDataVariableOffset(input.sliceEnd);
		auto ellipseSizeOffset = proto.GetFloat2DataVariableOffset(input.ellipseSize);
		style._anon_3 = ellipseSizeOffset.x;
		style._anon_4 = ellipseSizeOffset.y;
		style._anon_5 = proto.GetFloatDataVariableOffset(input.innerMask);
		style._anon_6 = proto.GetFloatDataVariableOffset(input.vingette);
		uint16_t styleId = proto.styleDescriptor.size();
		proto.styleDescriptor.push_back(style);
		struct AssetCircleRenderOffsets {
			uint16_t type = 2;
			uint16_t transformIndex;
			uint16_t assetIndex;
			Float2Offsets mins;
			Float2Offsets maxs;
			Float2Offsets texMins;
			Float2Offsets texMaxs;
			uint16_t flags;
			uint8_t styleIndex;
			char pad_29 = 0;
		};
		AssetCircleRenderOffsets rend{};
		rend.transformIndex = proto.transformIndices[input.transform.hash];
		rend.assetIndex = proto.GetAssetDataVariableOffset(input.mainAsset);
		rend.mins = proto.GetFloat2DataVariableOffset(input.mins);
		rend.maxs = proto.GetFloat2DataVariableOffset(input.maxs);
		rend.texMins = proto.GetFloat2DataVariableOffset(input.texMins);
		rend.texMaxs = proto.GetFloat2DataVariableOffset(input.texMaxs);
		rend.flags = input.flags;
		rend.styleIndex = styleId;
		proto.AddRenderJobData((uint8_t*)&rend, sizeof(rend));

	});
}

std::vector<std::shared_ptr<ImFlow::PinProto>> AssetCircleRenderNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<ColorVariable>>("Main Color", ImFlow::ConnectionFilter::SameType(), ColorVariable(1.f,1.f,1.f,1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<ColorVariable>>("Secondary Color",ImFlow::ConnectionFilter::SameType(), ColorVariable(1.f,1.f,1.f,1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<ColorVariable>>("Tertiary Color",ImFlow::ConnectionFilter::SameType(), ColorVariable(1.f,1.f,1.f,1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<AssetVariable>>("Asset", ImFlow::ConnectionFilter::SameType(), AssetVariable("white")));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Blend",ImFlow::ConnectionFilter::SameType(),FloatVariable(1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Premul",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Mins",ImFlow::ConnectionFilter::SameType(),Float2Variable(0.f,0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Maxs",ImFlow::ConnectionFilter::SameType(),Float2Variable(1.f,1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Texture Mins",ImFlow::ConnectionFilter::SameType(),Float2Variable(0.f,0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Texture Maxs",ImFlow::ConnectionFilter::SameType(),Float2Variable(1.f,1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Inner Slice Blend",ImFlow::ConnectionFilter::SameType(),FloatVariable(1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Slice Begin",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Slice End",ImFlow::ConnectionFilter::SameType(),FloatVariable(1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Ellipse Size",ImFlow::ConnectionFilter::SameType(),Float2Variable(1.f,1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Inner Mask",ImFlow::ConnectionFilter::SameType(),FloatVariable(1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Vingette",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<TransformResult>>("Transform",ImFlow::ConnectionFilter::SameType(),TransformResult()));
	return info;
}



TextStyleNode::TextStyleNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	currentFont = &fonts[0].fonts.begin()->second;

	getOut<TextStyleData>("Style")->behaviour([this]() {
		TextStyleData res;
		res.mainColor = getInVal<ColorVariable>("mainColor");
		res.scndColor = getInVal<ColorVariable>("scndColor");
		res.tertColor = getInVal<ColorVariable>("tertColor");
		res.blend = getInVal<FloatVariable>("Blend");
		res.premul = getInVal<FloatVariable>("Premul");
		res.dropShadowHardness = getInVal<FloatVariable>("Dropshadow Hardness");
		res.dropShadowOffset = getInVal<Float2Variable>("Dropshadow Offset");
		res.dropShadowBlur = getInVal<FloatVariable>("Dropshadow Blur");
		res.fontIndex = currentFont->fontIndex;
		res.size = getInVal<FloatVariable>("Size");
		res.stretchX = getInVal<FloatVariable>("stretchX");
		res.backgroundSize = getInVal<FloatVariable>("backgroundSize");
		res.boltness = getInVal<FloatVariable>("Boltness");
		res.blur = getInVal<FloatVariable>("Blur");
		res.style_32 = getInVal<FloatVariable>("style_32");
		return res;
	});
}

TextStyleNode::TextStyleNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :TextStyleNode(rend, style) {
	if (obj.HasMember("FontName") && obj["FontName"].IsString()) {
		std::string fontName = obj["FontName"].GetString();
		for (auto& fontAtlas : fonts) {
			for (auto& [index, font] : fontAtlas.fonts) {
				if (font.name == fontName) {
					currentFont = &font;
				}
			}
		}
	}
	
}

void TextStyleNode::draw() {
	ImGui::PushItemWidth(130.f);
	if(ImGui::BeginCombo("Font", currentFont->name.c_str())) {
		for (auto& atlas : fonts) {
			for (auto& [index,font] : atlas.fonts) {
				bool isSelected = index == currentFont->fontIndex;
				if (ImGui::Selectable(font.name.c_str(), isSelected)) {
					currentFont = &font;
				}
				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();
}

void TextStyleNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	obj.AddMember("FontName",currentFont->name, allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void TextStyleNode::Export(RuiExportPrototype&){}//export handled in TextRender

std::vector<std::shared_ptr<ImFlow::PinProto>> TextStyleNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<ColorVariable>>("mainColor",ImFlow::ConnectionFilter::SameType(),ColorVariable(1.f,1.f,1.f,1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<ColorVariable>>("scndColor",ImFlow::ConnectionFilter::SameType(),ColorVariable(0.f,0.f,0.f,0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<ColorVariable>>("tertColor",ImFlow::ConnectionFilter::SameType(),ColorVariable(0.f,0.f,0.f,0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Blend",ImFlow::ConnectionFilter::SameType(),FloatVariable(1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Premul",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Dropshadow Hardness",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Dropshadow Offset",ImFlow::ConnectionFilter::SameType(),Float2Variable(0.f,0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Dropshadow Blur",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Size",ImFlow::ConnectionFilter::SameType(),FloatVariable(10.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("stretchX",ImFlow::ConnectionFilter::SameType(),FloatVariable(1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("backgroundSize",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Boltness",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Blur",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("style_32",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<TextStyleData>>("Style"));
	return info;
}

TextSizeNode::TextSizeNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	std::string sizeName = Variable::UniqueName();
	getOut<TextInputData>("Text Data")->behaviour([this,sizeName]() {
		
		TextInputData data;

		data.text = getInVal<StringVariable>("text");
		data.minSize = getInVal<Float2Variable>("minSize");
		data.maxSize = getInVal<Float2Variable>("maxSize");
		data.styles[0] = getInVal<TextStyleData>("Style_0");
		data.styles[1] = getInVal<TextStyleData>("Style_1");
		data.styles[2] = getInVal<TextStyleData>("Style_2");
		data.styles[3] = getInVal<TextStyleData>("Style_3");
		data.sizeName = sizeName;
		GetTextSize(data);
		return data;
	});
	getOut<TransformSize>("Size")->behaviour([this,sizeName]() {
		
		TextInputData data;

		data.text = getInVal<StringVariable>("text");
		data.minSize = getInVal<Float2Variable>("minSize");
		data.maxSize = getInVal<Float2Variable>("maxSize");
		data.styles[0] = getInVal<TextStyleData>("Style_0");
		data.styles[1] = getInVal<TextStyleData>("Style_1");
		data.styles[2] = getInVal<TextStyleData>("Style_2");
		data.styles[3] = getInVal<TextStyleData>("Style_3");
		
		TransformSize size{ GetTextSize(data),sizeName };
		return size;
	});

}

TextSizeNode::TextSizeNode(RenderInstance& rend, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj) :TextSizeNode(rend, style) {}

void TextSizeNode::draw() {


}

void TextSizeNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}


void TextSizeNode::Export(RuiExportPrototype&){}

std::vector<std::shared_ptr<ImFlow::PinProto>> TextSizeNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<StringVariable>>("text",ImFlow::ConnectionFilter::SameType(),StringVariable("Default Text")));

	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("minSize",ImFlow::ConnectionFilter::SameType(),Float2Variable(0.f,0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("maxSize",ImFlow::ConnectionFilter::SameType(),Float2Variable(1000000000.f,1000000000.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<TextStyleData>>("Style_0",ImFlow::ConnectionFilter::SameType(),TextStyleData()));
	info.push_back(std::make_shared<ImFlow::InPinProto<TextStyleData>>("Style_1",ImFlow::ConnectionFilter::SameType(),TextStyleData()));
	info.push_back(std::make_shared<ImFlow::InPinProto<TextStyleData>>("Style_2",ImFlow::ConnectionFilter::SameType(),TextStyleData()));
	info.push_back(std::make_shared<ImFlow::InPinProto<TextStyleData>>("Style_3",ImFlow::ConnectionFilter::SameType(),TextStyleData()));
	info.push_back(std::make_shared<ImFlow::OutPinProto<TextInputData>>("Text Data"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<TransformSize>>("Size"));
	return info;
}


TextRenderNode::TextRenderNode(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getIn<TransformResult>("Parent")->setEmptyVal(render.transformResults[2]);
}

TextRenderNode::TextRenderNode(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):TextRenderNode(rend,style){}

void TextRenderNode::draw() {
	const TextInputData& data = getInVal<TextInputData>("Data");
	const TransformResult& parent = getInVal<TransformResult>("Parent");
	Text_Render(render,data,parent);
}

void TextRenderNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void AddTextStyleVariablesToProto(RuiExportPrototype& proto,const TextStyleData& style) {
	proto.AddDataVariable(style.mainColor);
	proto.AddDataVariable(style.scndColor);
	proto.AddDataVariable(style.tertColor);
	proto.AddDataVariable(style.blend);
	proto.AddDataVariable(style.premul);
	proto.AddDataVariable(style.dropShadowBlur);
	proto.AddDataVariable(style.dropShadowOffset);
	proto.AddDataVariable(style.dropShadowHardness);
	proto.AddDataVariable(style.size);
	proto.AddDataVariable(style.stretchX);
	proto.AddDataVariable(style.backgroundSize);
	proto.AddDataVariable(style.boltness);
	proto.AddDataVariable(style.blur);
	proto.AddDataVariable(style.style_32);
}

void AddTextStyleToDependency(std::set<std::string>& deps,const TextStyleData& style) {
	deps.insert(style.mainColor.name);
	deps.insert(style.scndColor.name);
	deps.insert(style.tertColor.name);
	deps.insert(style.blend.name);
	deps.insert(style.premul.name);
	deps.insert(style.dropShadowBlur.name);
	deps.insert(style.dropShadowOffset.name);
	deps.insert(style.dropShadowHardness.name);
	deps.insert(style.size.name);
	deps.insert(style.stretchX.name);
	deps.insert(style.backgroundSize.name);
	deps.insert(style.boltness.name);
	deps.insert(style.blur.name);
	deps.insert(style.style_32.name);
}

uint8_t AddTextStyleToProto(RuiExportPrototype& proto,const TextStyleData& style) {
	
	StyleDescriptorOffsets style0{};
	style0.type = 0;
	style0.color0 = proto.GetColorDataVariableOffset(style.mainColor);
	style0.color1 = proto.GetColorDataVariableOffset(style.scndColor);
	style0.color2 = proto.GetColorDataVariableOffset(style.tertColor);
	style0.blend = proto.GetFloatDataVariableOffset(style.blend);
	style0.premul = proto.GetFloatDataVariableOffset(style.premul);
	style0._anon_0 =	style.fontIndex;
	Float2Offsets dropShadowOffset = proto.GetFloat2DataVariableOffset(style.dropShadowOffset);
	style0._anon_1 = proto.GetFloatDataVariableOffset(style.dropShadowHardness);
	style0._anon_2 = dropShadowOffset.x;
	style0._anon_3 = dropShadowOffset.y;
	style0._anon_4 = proto.GetFloatDataVariableOffset(style.dropShadowBlur);
	style0._anon_5 = proto.GetFloatDataVariableOffset(style.size);
	style0._anon_6 = proto.GetFloatDataVariableOffset(style.stretchX);
	style0._anon_7 = proto.GetFloatDataVariableOffset(style.backgroundSize);
	style0._anon_8 = proto.GetFloatDataVariableOffset(style.boltness);
	style0._anon_9 = proto.GetFloatDataVariableOffset(style.blur);
	style0._anon_10 = proto.GetFloatDataVariableOffset(style.style_32);
	uint16_t res = proto.styleDescriptor.size();
	proto.styleDescriptor.push_back(style0);
	return res;
}

void TextRenderNode::Export(RuiExportPrototype& proto) {
	const TextInputData& input = getInVal<TextInputData>("Data");
	const TransformResult& parent = getInVal<TransformResult>("Parent");

	proto.renderJobCount++;
	proto.AddDataVariable(input.text);
	proto.AddDataVariable(input.minSize);
	proto.AddDataVariable(input.maxSize);
	AddTextStyleVariablesToProto(proto,input.styles[0]);
	AddTextStyleVariablesToProto(proto,input.styles[1]);
	AddTextStyleVariablesToProto(proto,input.styles[2]);
	AddTextStyleVariablesToProto(proto,input.styles[3]);




	proto.step2Callbacks.push_back([input,parent](RuiExportPrototype& proto) {

		struct TextRenderOffsets {
			uint16_t type = 0;
			uint16_t transformIndex;
			uint8_t styleIndex[4];
			uint16_t text;
			Float2Offsets maxSize;
			Float2Offsets minSize;
		};
		TextRenderOffsets rend{};
		rend.transformIndex = proto.transformIndices[parent.hash];
		rend.styleIndex[0] = AddTextStyleToProto(proto,input.styles[0]);
		rend.styleIndex[1] = AddTextStyleToProto(proto,input.styles[1]);
		rend.styleIndex[2] = AddTextStyleToProto(proto,input.styles[2]);
		rend.styleIndex[3] = AddTextStyleToProto(proto,input.styles[3]);
		rend.text = proto.GetStringDataVariableOffset(input.text);
		rend.minSize = proto.GetFloat2DataVariableOffset(input.minSize);
		rend.maxSize = proto.GetFloat2DataVariableOffset(input.maxSize);
		size_t jobOffset = proto.renderJobData.size();
		proto.AddRenderJobData((uint8_t*)&rend, sizeof(rend));
		
		ExportElement<std::string> ele;
		ele.identifier = input.sizeName;
		ele.dependencys = {input.text.name,input.minSize.name,input.maxSize.name};
		AddTextStyleToDependency(ele.dependencys,input.styles[0]);
		AddTextStyleToDependency(ele.dependencys,input.styles[1]);
		AddTextStyleToDependency(ele.dependencys,input.styles[2]);
		AddTextStyleToDependency(ele.dependencys,input.styles[3]);
		ele.callback = [input, jobOffset](RuiExportPrototype& proto) {
			proto.codeLines.push_back(std::format("__m128 {} = funcs->GetTextSize(inst,{});",input.sizeName,jobOffset));
		};
		proto.codeElements.push_back(ele);

	});
}


std::vector<std::shared_ptr<ImFlow::PinProto>> TextRenderNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<TextInputData>>("Data",ImFlow::ConnectionFilter::SameType(),TextInputData()));
	info.push_back(std::make_shared<ImFlow::InPinProto<TransformResult>>("Parent",ImFlow::ConnectionFilter::SameType(),TransformResult()));
	return info;
}



void AddRenderNodes(NodeEditor& editor) {
	editor.AddNodeType<AssetRenderNode>();
	editor.AddNodeType<AssetCircleRenderNode>();
	editor.AddNodeType<TextStyleNode>();
	editor.AddNodeType<TextSizeNode>();
	editor.AddNodeType<TextRenderNode>();

}