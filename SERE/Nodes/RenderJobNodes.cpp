#include "RenderJobNodes.h"
#include "RenderManager.h"







AssetRenderNode::AssetRenderNode(RenderInstance& prot, NodeStyles& styles):proto(prot) {
	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this);
	}
	getIn<TransformResult>("Transform")->setEmptyVal(proto.transformResults[2]);
}


void AssetRenderNode::draw() {
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
	input.flags = 0x1F00;
	Render_Asset(proto,input);
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



TextStyleNode::TextStyleNode(RenderInstance& prot, NodeStyles& styles) :proto(prot) {
	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	currentFont = &fonts[0].fonts.begin()->second;
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this);
	}
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

void TextStyleNode::draw() {
	ImGui::PushItemWidth(130.f);
	if(ImGui::BeginCombo("Font", currentFont->name.c_str())) {
		for (auto& atlas : fonts) {
			for (auto& font : atlas.fonts) {
				bool isSelected = font.first == currentFont->fontIndex;
				if (ImGui::Selectable(font.second.name.c_str(), isSelected)) {
					currentFont = &font.second;
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

TextSizeNode::TextSizeNode(RenderInstance& prot, NodeStyles& styles):proto(prot) {
	setTitle(name);
	setStyle(styles.GetNodeStyle(category));

	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this);
	}
	getOut<TextInputData>("Text Data")->behaviour([this]() {
		
		TextInputData data;

		data.text = getInVal<StringVariable>("text");
		data.minSize = getInVal<Float2Variable>("minSize");
		data.maxSize = getInVal<Float2Variable>("maxSize");
		data.styles[0] = getInVal<TextStyleData>("Style_0");
		data.styles[1] = getInVal<TextStyleData>("Style_1");
		data.styles[2] = getInVal<TextStyleData>("Style_2");
		data.styles[3] = getInVal<TextStyleData>("Style_3");
		GetTextSize(data);
		return data;
	});
	getOut<TransformSize>("Size")->behaviour([this]() {
		
		TextInputData data;

		data.text = getInVal<StringVariable>("text");
		data.minSize = getInVal<Float2Variable>("minSize");
		data.maxSize = getInVal<Float2Variable>("maxSize");
		data.styles[0] = getInVal<TextStyleData>("Style_0");
		data.styles[1] = getInVal<TextStyleData>("Style_1");
		data.styles[2] = getInVal<TextStyleData>("Style_2");
		data.styles[3] = getInVal<TextStyleData>("Style_3");
		
		TransformSize size;
		size.size = GetTextSize(data);

		return size;
	});

}



void TextSizeNode::draw() {


}

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


TextRenderNode::TextRenderNode(RenderInstance& prot, NodeStyles& styles) :proto(prot) {
	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	for (auto& pin : GetPinInfo()) {
		pin->CreatePin(this);
	}
	getIn<TransformResult>("Parent")->setEmptyVal(proto.transformResults[2]);
}

void TextRenderNode::draw() {
	const TextInputData& data = getInVal<TextInputData>("Data");
	const TransformResult& parent = getInVal<TransformResult>("Parent");
	Text_Render(proto,data,parent);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> TextRenderNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<TextInputData>>("Data",ImFlow::ConnectionFilter::SameType(),TextInputData()));
	info.push_back(std::make_shared<ImFlow::InPinProto<TransformResult>>("Parent",ImFlow::ConnectionFilter::SameType(),TransformResult()));
	return info;
}



void AddRenderNodes(NodeEditor& editor) {
	editor.AddNodeType<AssetRenderNode>();
	editor.AddNodeType<TextStyleNode>();
	editor.AddNodeType<TextSizeNode>();
	editor.AddNodeType<TextRenderNode>();

}