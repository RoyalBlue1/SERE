#include "SplitMergeNodes.h"

SplitFloat2Node::SplitFloat2Node(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Split Vector2");
	setStyle(styles.splitMergeNode);
	ImFlow::BaseNode::addIN<Float2Variable>("In",{true,false,{0.f,0.f}},ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addOUT<FloatVariable>("X",styles.floatVariable)->behaviour([this]() {
		FloatVariable var;
		const Float2Variable& in = getInVal<Float2Variable>("In");
		var.isConstant = in.isConstant;
		var.value = in.value[0];
		return var;

	});
	ImFlow::BaseNode::addOUT<FloatVariable>("Y",styles.floatVariable)->behaviour([this]() {
		FloatVariable var;
		const Float2Variable& in = getInVal<Float2Variable>("In");
		var.isConstant = in.isConstant;
		var.value = in.value[1];
		return var;

	});
}

void SplitFloat2Node::draw() {
	const Float2Variable& in = getInVal<Float2Variable>("In");
	ImGui::Text("%f",in.value[0]);
	ImGui::Text("%f",in.value[1]);

}

MergeFloat2Node::MergeFloat2Node(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Merge Vector2");
	setStyle(styles.splitMergeNode);
	ImFlow::BaseNode::addIN<FloatVariable>("X",{true,false,0.f},ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Y",{true,false,0.f},ImFlow::ConnectionFilter::SameType(),styles.floatVariable);

	ImFlow::BaseNode::addOUT<Float2Variable>("Out",styles.float2Variable)->behaviour([this]() {
		Float2Variable var;
		const FloatVariable& inX = getInVal<FloatVariable>("X");
		const FloatVariable& inY = getInVal<FloatVariable>("Y");
		var.isConstant = inX.isConstant&&inY.isConstant;
		var.value[0] = inX.value;
		var.value[1] = inY.value;
		return var;

	});
}

void MergeFloat2Node::draw() {
	const FloatVariable& inX = getInVal<FloatVariable>("X");
	const FloatVariable& inY = getInVal<FloatVariable>("Y");
	ImGui::Text("%f",inX.value);
	ImGui::Text("%f",inY.value);
}

SplitFloat3Node::SplitFloat3Node(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Split Vector3");
	setStyle(styles.splitMergeNode);
	ImFlow::BaseNode::addIN<Float3Variable>("In",{true,false,{0.f,0.f}},ImFlow::ConnectionFilter::SameType(),styles.float3Variable);
	ImFlow::BaseNode::addOUT<FloatVariable>("X",styles.floatVariable)->behaviour([this]() {
		FloatVariable var;
		const Float3Variable& in = getInVal<Float3Variable>("In");
		var.isConstant = in.isConstant;
		var.value = in.value[0];
		return var;

	});
	ImFlow::BaseNode::addOUT<FloatVariable>("Y",styles.floatVariable)->behaviour([this]() {
		FloatVariable var;
		const Float3Variable& in = getInVal<Float3Variable>("In");
		var.isConstant = in.isConstant;
		var.value = in.value[1];
		return var;

	});
	ImFlow::BaseNode::addOUT<FloatVariable>("Z",styles.floatVariable)->behaviour([this]() {
		FloatVariable var;
		const Float3Variable& in = getInVal<Float3Variable>("In");
		var.isConstant = in.isConstant;
		var.value = in.value[2];
		return var;

	});
}

void SplitFloat3Node::draw() {
	const Float3Variable& in = getInVal<Float3Variable>("In");
	ImGui::Text("%f",in.value[0]);
	ImGui::Text("%f",in.value[1]);
	ImGui::Text("%f",in.value[2]);

}

MergeFloat3Node::MergeFloat3Node(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Merge Vector2");
	setStyle(styles.splitMergeNode);
	ImFlow::BaseNode::addIN<FloatVariable>("X",{true,false,0.f},ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Y",{true,false,0.f},ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Z",{true,false,0.f},ImFlow::ConnectionFilter::SameType(),styles.floatVariable);

	ImFlow::BaseNode::addOUT<Float3Variable>("Out",styles.float3Variable)->behaviour([this]() {
		Float3Variable var;
		const FloatVariable& inX = getInVal<FloatVariable>("X");
		const FloatVariable& inY = getInVal<FloatVariable>("Y");
		const FloatVariable& inZ = getInVal<FloatVariable>("Z");
		var.isConstant = inX.isConstant&&inY.isConstant&&inZ.isConstant;
		var.value[0] = inX.value;
		var.value[1] = inY.value;
		var.value[2] = inZ.value;
		return var;

	});
}

void MergeFloat3Node::draw() {
	const FloatVariable& inX = getInVal<FloatVariable>("X");
	const FloatVariable& inY = getInVal<FloatVariable>("Y");
	const FloatVariable& inZ = getInVal<FloatVariable>("Z");
	ImGui::Text("%f",inX.value);
	ImGui::Text("%f",inY.value);
	ImGui::Text("%f",inZ.value);
}



SplitColorNode::SplitColorNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Split Color");
	setStyle(styles.splitMergeNode);
	ImFlow::BaseNode::addIN<ColorVariable>("In",{true,false,{0.f,0.f}},ImFlow::ConnectionFilter::SameType(),styles.colorVariable);
	ImFlow::BaseNode::addOUT<FloatVariable>("Red",styles.floatVariable)->behaviour([this]() {
		FloatVariable var;
		const ColorVariable& in = getInVal<ColorVariable>("In");
		var.isConstant = in.isConstant;
		var.value = in.value[0];
		return var;

	});
	ImFlow::BaseNode::addOUT<FloatVariable>("Green",styles.floatVariable)->behaviour([this]() {
		FloatVariable var;
		const ColorVariable& in = getInVal<ColorVariable>("In");
		var.isConstant = in.isConstant;
		var.value = in.value[1];
		return var;

	});
	ImFlow::BaseNode::addOUT<FloatVariable>("Blue",styles.floatVariable)->behaviour([this]() {
		FloatVariable var;
		const ColorVariable& in = getInVal<ColorVariable>("In");
		var.isConstant = in.isConstant;
		var.value = in.value[2];
		return var;

	});
	ImFlow::BaseNode::addOUT<FloatVariable>("Alpha",styles.floatVariable)->behaviour([this]() {
		FloatVariable var;
		const ColorVariable& in = getInVal<ColorVariable>("In");
		var.isConstant = in.isConstant;
		var.value = in.value[3];
		return var;

	});
}

void SplitColorNode::draw() {
	const ColorVariable& col = getInVal<ColorVariable>("In");
	ImGui::Text("%f",col.value[0]);
	ImGui::Text("%f",col.value[1]);
	ImGui::Text("%f",col.value[2]);
	ImGui::Text("%f",col.value[3]);
}

RGBToColorNode::RGBToColorNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Merge Color");
	setStyle(styles.splitMergeNode);
	ImFlow::BaseNode::addIN<FloatVariable>("Red",{true,false,0.f},ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Green",{true,false,0.f},ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Blue",{true,false,0.f},ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Alpha",{true,false,0.f},ImFlow::ConnectionFilter::SameType(),styles.floatVariable);

	ImFlow::BaseNode::addOUT<ColorVariable>("Out",styles.float2Variable)->behaviour([this]() {
		ColorVariable var;
		const FloatVariable& inRed = getInVal<FloatVariable>("Red");
		const FloatVariable& inGreen = getInVal<FloatVariable>("Green");
		const FloatVariable& inBlue = getInVal<FloatVariable>("Blue");
		const FloatVariable& inAlpha = getInVal<FloatVariable>("Alpha");
		var.isConstant = inRed.isConstant&&inGreen.isConstant&&inBlue.isConstant&&inAlpha.isConstant;
		var.value[0] = inRed.value;
		var.value[1] = inBlue.value;
		var.value[2] = inGreen.value;
		var.value[3] = inAlpha.value;
		return var;

	});
}

void RGBToColorNode::draw() {
	const FloatVariable& inRed = getInVal<FloatVariable>("Red");
	const FloatVariable& inGreen = getInVal<FloatVariable>("Green");
	const FloatVariable& inBlue = getInVal<FloatVariable>("Blue");
	const FloatVariable& inAlpha = getInVal<FloatVariable>("Alpha");
	ImGui::Text("%f",inRed.value);
	ImGui::Text("%f",inGreen.value);
	ImGui::Text("%f",inBlue.value);
	ImGui::Text("%f",inAlpha.value);
}

HSVToColorNode::HSVToColorNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Merge Color");
	setStyle(styles.splitMergeNode);
	ImFlow::BaseNode::addIN<FloatVariable>("Hue",{true,false,0.f},ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Saturation",{true,false,0.f},ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Vibrance",{true,false,0.f},ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Alpha",{true,false,0.f},ImFlow::ConnectionFilter::SameType(),styles.floatVariable);

	ImFlow::BaseNode::addOUT<ColorVariable>("Out",styles.float2Variable)->behaviour([this]() {
		ColorVariable var;
		const FloatVariable& inH = getInVal<FloatVariable>("Hue");
		const FloatVariable& inS = getInVal<FloatVariable>("Saturation");
		const FloatVariable& inV = getInVal<FloatVariable>("Vibrance");
		const FloatVariable& inAlpha = getInVal<FloatVariable>("Alpha");

		float r = 0, g = 0, b = 0;

		if (inS.value == 0)
		{
			r = inV.value;
			g = inV.value;
			b = inV.value;
		}
		else
		{
			int i;
			float f, p, q, t;
			float h = inH.value * 360;
			if (h == 360)
				h = 0;
			else
				h = h/ 60;

			i = (int)trunc(h);
			f = h - i;

			p = inV.value * (1.0 - inS.value);
			q = inV.value * (1.0 - (inS.value * f));
			t = inV.value * (1.0 - (inS.value * (1.0 - f)));

			switch (i)
			{
			case 0:
				r = inV.value;
				g = t;
				b = p;
				break;

			case 1:
				r = q;
				g = inV.value;
				b = p;
				break;

			case 2:
				r = p;
				g = inV.value;
				b = t;
				break;

			case 3:
				r = p;
				g = q;
				b = inV.value;
				break;

			case 4:
				r = t;
				g = p;
				b = inV.value;
				break;

			default:
				r = inV.value;
				g = p;
				b = q;
				break;
			}

		}


		var.isConstant = inH.isConstant&&inS.isConstant&&inV.isConstant&&inAlpha.isConstant;
		var.value[0] = r;
		var.value[1] = g;
		var.value[2] = b;
		var.value[3] = inAlpha.value;
		return var;

	});
}

void HSVToColorNode::draw() {
	const FloatVariable& inRed = getInVal<FloatVariable>("Hue");
	const FloatVariable& inGreen = getInVal<FloatVariable>("Saturation");
	const FloatVariable& inBlue = getInVal<FloatVariable>("Vibrance");
	const FloatVariable& inAlpha = getInVal<FloatVariable>("Alpha");
	ImGui::Text("%f",inRed.value);
	ImGui::Text("%f",inGreen.value);
	ImGui::Text("%f",inBlue.value);
	ImGui::Text("%f",inAlpha.value);
}