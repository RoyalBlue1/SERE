#include "RenderJobNodes.h"
#include "RenderManager.h"







AssetRenderNode::AssetRenderNode(RenderInstance& prot, NodeStyles& styles):proto(prot) {
	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addIN<ColorVariable>("Main Color", ColorVariable(1.f,1.f,1.f,1.f), ImFlow::ConnectionFilter::SameType(), styles.colorVariable);
	ImFlow::BaseNode::addIN<ColorVariable>("Mask Color",ColorVariable(1.f,1.f,1.f,1.f),ImFlow::ConnectionFilter::SameType(), styles.colorVariable);
	ImFlow::BaseNode::addIN<ColorVariable>("Tertiary Color",ColorVariable(1.f,1.f,1.f,1.f),ImFlow::ConnectionFilter::SameType(), styles.colorVariable);
	ImFlow::BaseNode::addIN<AssetVariable>("Main Asset",AssetVariable("white"), ImFlow::ConnectionFilter::SameType(), styles.assetVariable);
	ImFlow::BaseNode::addIN<AssetVariable>("Mask Asset",AssetVariable(INVALID_ASSET),ImFlow::ConnectionFilter::SameType(),styles.assetVariable);
	ImFlow::BaseNode::addIN<Float2Variable>("Mins",Float2Variable(0.f,0.f), ImFlow::ConnectionFilter::SameType(), styles.float2Variable);
	ImFlow::BaseNode::addIN<Float2Variable>("Maxs",Float2Variable(1.f,1.f),ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<Float2Variable>("Texture Mins",Float2Variable(0.f,0.f),ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<Float2Variable>("Texture Maxs",Float2Variable(1.f,1.f),ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<FloatVariable>("Blend",FloatVariable(1.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Premul",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<Float2Variable>("Mask Center",Float2Variable(.5f,.5f),ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<Float2Variable>("Mask Translate",Float2Variable(0.f,0.f),ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<Float2Variable>("Mask Size",Float2Variable(1.f,1.f),ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<FloatVariable>("Mask Rotation",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<TransformResult>("Transform", proto.transformResults[2],ImFlow::ConnectionFilter::SameType(),styles.transformResult);
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
	input.flags = 0x1000;
	Render_Asset(proto,input);
}

std::vector<PinInfo> AssetRenderNode::GetPinInfo() {
	return {};
}

AssetSmallRenderNode::AssetSmallRenderNode(RenderInstance& prot, NodeStyles& styles):proto(prot) {
	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addIN<ColorVariable>("Main Color", ColorVariable(1.f,1.f,1.f,1.f), ImFlow::ConnectionFilter::SameType(), styles.colorVariable);
	ImFlow::BaseNode::addIN<ColorVariable>("Secondary Color",ColorVariable(1.f,1.f,1.f,1.f),ImFlow::ConnectionFilter::SameType(), styles.colorVariable);
	ImFlow::BaseNode::addIN<ColorVariable>("Tertiary Color",ColorVariable(1.f,1.f,1.f,1.f),ImFlow::ConnectionFilter::SameType(), styles.colorVariable);
	ImFlow::BaseNode::addIN<AssetVariable>("Main Asset",AssetVariable("white"), ImFlow::ConnectionFilter::SameType(), styles.assetVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Blend",FloatVariable(1.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Premul",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<Float2Variable>("Mins",Float2Variable(0.f,0.f),ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<Float2Variable>("Maxs",Float2Variable(1.f,1.f),ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<Float2Variable>("Texture Mins",Float2Variable(0.f,0.f),ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<Float2Variable>("Texture Maxs",Float2Variable(1.f,1.f),ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<FloatVariable>("Inner Slice Blend",FloatVariable(1.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Slice Begin",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Slice End",FloatVariable(1.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<Float2Variable>("Ellipse Size",Float2Variable(1.f,1.f),ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<FloatVariable>("Inner Mask",FloatVariable(1.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Vingette",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<TransformResult>("Transform", proto.transformResults[2],ImFlow::ConnectionFilter::SameType(),styles.transformResult);
}


void AssetSmallRenderNode::draw() {
	AssetSmallInputData input{};
	input.mainColor = getInVal<ColorVariable>("Main Color");
	input.scndColor = getInVal<ColorVariable>("Secondary Color");
	input.tertColor = getInVal<ColorVariable>("Tertiary Color");
	input.mainAsset = getInVal<AssetVariable>("Main Asset");
	input.blend = getInVal<FloatVariable>("Blend");
	input.premul = getInVal<FloatVariable>("Premul");
	input.mins = getInVal<Float2Variable>("Mins");
	input.maxs = getInVal<Float2Variable>("Maxs");
	input.texMins = getInVal<Float2Variable>("Texture Mins");
	input.texMaxs = getInVal<Float2Variable>("Texture Maxs");
	input.style_1E = getInVal<FloatVariable>("Inner Slice Blend");
	input.style_20 = getInVal<FloatVariable>("Slice Begin");
	input.style_22 = getInVal<FloatVariable>("Slice End");
	input.ellipseSize = getInVal<Float2Variable>("Ellipse Size");
	input.innerMask = getInVal<FloatVariable>("Inner Mask");
	input.vingette = getInVal<FloatVariable>("Vingette");

	input.transform = getInVal<TransformResult>("Transform");
	input.flags = 0x2000;
	Render_AssetSmall(proto,input);
}

std::vector<PinInfo> AssetSmallRenderNode::GetPinInfo() {
	return {};
}



TextStyleNode::TextStyleNode(RenderInstance& prot, NodeStyles& styles) :proto(prot) {
	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	currentFont = &fonts[0].fonts.begin()->second;
	ImFlow::BaseNode::addIN<ColorVariable>("mainColor",ColorVariable(1.f,1.f,1.f,1.f),ImFlow::ConnectionFilter::SameType(), styles.colorVariable);
	ImFlow::BaseNode::addIN<ColorVariable>("scndColor",ColorVariable(0.f,0.f,0.f,0.f),ImFlow::ConnectionFilter::SameType(), styles.colorVariable);
	ImFlow::BaseNode::addIN<ColorVariable>("tertColor",ColorVariable(0.f,0.f,0.f,0.f),ImFlow::ConnectionFilter::SameType(), styles.colorVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Blend",FloatVariable(1.f), ImFlow::ConnectionFilter::SameType(), styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Premul",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Dropshadow Hardness",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<Float2Variable>("Dropshadow Offset", Float2Variable(0.f,0.f), ImFlow::ConnectionFilter::SameType(), styles.float2Variable);
	ImFlow::BaseNode::addIN<FloatVariable>("Dropshadow Blur",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Size",FloatVariable(50.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("stretchX",FloatVariable(1.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("backgroundSize",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Boltness",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Blur",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("style_32",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addOUT<TextStyleData>("Style")->behaviour([this]() {
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

std::vector<PinInfo> TextStyleNode::GetPinInfo() {
	return {};
}

TextSizeNode::TextSizeNode(RenderInstance& prot, NodeStyles& styles):proto(prot) {
	setTitle(name);
	setStyle(styles.GetNodeStyle(category));

	ImFlow::BaseNode::addIN<StringVariable>("text",StringVariable("Default Text"), ImFlow::ConnectionFilter::SameType(), styles.stringVariable);

	ImFlow::BaseNode::addIN<Float2Variable>("minSize", Float2Variable(0.f,0.f), ImFlow::ConnectionFilter::SameType(), styles.float2Variable);
	ImFlow::BaseNode::addIN<Float2Variable>("maxSize", Float2Variable(1000000000.f,1000000000.f), ImFlow::ConnectionFilter::SameType(), styles.float2Variable);
	ImFlow::BaseNode::addIN<TextStyleData>("Style_0",TextStyleData(),ImFlow::ConnectionFilter::SameType());
	ImFlow::BaseNode::addIN<TextStyleData>("Style_1",TextStyleData(),ImFlow::ConnectionFilter::SameType());
	ImFlow::BaseNode::addIN<TextStyleData>("Style_2",TextStyleData(),ImFlow::ConnectionFilter::SameType());
	ImFlow::BaseNode::addIN<TextStyleData>("Style_3",TextStyleData(),ImFlow::ConnectionFilter::SameType());
	ImFlow::BaseNode::addOUT<TextInputData>("Text Data", styles.textData)->behaviour([this]() {
		
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
	ImFlow::BaseNode::addOUT<TransformSize>("Size", styles.transformSize)->behaviour([this]() {
		
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

std::vector<PinInfo> TextSizeNode::GetPinInfo() {
	return {};
}


TextRenderNode::TextRenderNode(RenderInstance& prot, NodeStyles& styles) :proto(prot) {
	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addIN<TextInputData>("Data",TextInputData(), ImFlow::ConnectionFilter::SameType(), styles.textData);
	ImFlow::BaseNode::addIN<TransformResult>("Parent",proto.transformResults[2], ImFlow::ConnectionFilter::SameType(), styles.transformResult);

}

void TextRenderNode::draw() {
	const TextInputData& data = getInVal<TextInputData>("Data");
	const TransformResult& parent = getInVal<TransformResult>("Parent");
	Text_Render(proto,data,parent);
}

std::vector<PinInfo> TextRenderNode::GetPinInfo() {
	return {};
}



void AddRenderNodes(NodeEditor& editor) {
	editor.AddNodeType<AssetRenderNode>();
	editor.AddNodeType<AssetSmallRenderNode>();
	editor.AddNodeType<TextStyleNode>();
	editor.AddNodeType<TextSizeNode>();
	editor.AddNodeType<TextRenderNode>();

}