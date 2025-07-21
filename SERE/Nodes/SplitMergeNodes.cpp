#include "SplitMergeNodes.h"

SplitFloat2Node::SplitFloat2Node(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Split Vector2");
	setStyle(styles.splitMergeNode);
	ImFlow::BaseNode::addIN<Float2Variable>("In",Float2Variable(0.f,0.f),ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addOUT<FloatVariable>("X",styles.floatVariable)->behaviour([this]() {
		const Float2Variable& in = getInVal<Float2Variable>("In");
		return FloatVariable(in.value.x,in.isConstant);

	});
	ImFlow::BaseNode::addOUT<FloatVariable>("Y",styles.floatVariable)->behaviour([this]() {
		const Float2Variable& in = getInVal<Float2Variable>("In");
		return FloatVariable(in.value.y,in.isConstant);

	});
}

void SplitFloat2Node::draw() {
	const Float2Variable& in = getInVal<Float2Variable>("In");
	ImGui::Text("%f",in.value.x);
	ImGui::Text("%f",in.value.y);

}

MergeFloat2Node::MergeFloat2Node(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Merge Vector2");
	setStyle(styles.splitMergeNode);
	ImFlow::BaseNode::addIN<FloatVariable>("X",FloatVariable(0.f), ImFlow::ConnectionFilter::SameType(), styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Y",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);

	ImFlow::BaseNode::addOUT<Float2Variable>("Out",styles.float2Variable)->behaviour([this]() {

		const FloatVariable& inX = getInVal<FloatVariable>("X");
		const FloatVariable& inY = getInVal<FloatVariable>("Y");
		bool isConstant = inX.isConstant&&inY.isConstant;

		return Float2Variable(inX.value,inY.value,isConstant);

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
	ImFlow::BaseNode::addIN<Float3Variable>("In",Float3Variable(0.f,0.f,0.f), ImFlow::ConnectionFilter::SameType(), styles.float3Variable);
	ImFlow::BaseNode::addOUT<FloatVariable>("X",styles.floatVariable)->behaviour([this]() {
		const Float3Variable& in = getInVal<Float3Variable>("In");
		return FloatVariable(in.value.x,in.isConstant);

	});
	ImFlow::BaseNode::addOUT<FloatVariable>("Y",styles.floatVariable)->behaviour([this]() {
		const Float3Variable& in = getInVal<Float3Variable>("In");
		return FloatVariable(in.value.y,in.isConstant);

	});
	ImFlow::BaseNode::addOUT<FloatVariable>("Z",styles.floatVariable)->behaviour([this]() {
		const Float3Variable& in = getInVal<Float3Variable>("In");
		return FloatVariable(in.value.z,in.isConstant);

	});
}

void SplitFloat3Node::draw() {
	const Float3Variable& in = getInVal<Float3Variable>("In");
	ImGui::Text("%f",in.value.x);
	ImGui::Text("%f",in.value.y);
	ImGui::Text("%f",in.value.z);

}

MergeFloat3Node::MergeFloat3Node(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("Merge Vector2");
	setStyle(styles.splitMergeNode);
	ImFlow::BaseNode::addIN<FloatVariable>("X",FloatVariable(0.f), ImFlow::ConnectionFilter::SameType(), styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Y",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Z",FloatVariable(0.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);

	ImFlow::BaseNode::addOUT<Float3Variable>("Out",styles.float3Variable)->behaviour([this]() {

		const FloatVariable& inX = getInVal<FloatVariable>("X");
		const FloatVariable& inY = getInVal<FloatVariable>("Y");
		const FloatVariable& inZ = getInVal<FloatVariable>("Z");
		bool isConstant = inX.isConstant&&inY.isConstant&&inZ.isConstant;
		return Float3Variable(inX.value,inY.value,inZ.value,isConstant);


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
	ImFlow::BaseNode::addIN<ColorVariable>("In",ColorVariable(1.f,1.f,1.f,1.f),ImFlow::ConnectionFilter::SameType(),styles.colorVariable);
	ImFlow::BaseNode::addOUT<FloatVariable>("Red",styles.floatVariable)->behaviour([this]() {

		const ColorVariable& in = getInVal<ColorVariable>("In");
		return FloatVariable(in.value.red,in.isConstant);

	});
	ImFlow::BaseNode::addOUT<FloatVariable>("Green",styles.floatVariable)->behaviour([this]() {
		const ColorVariable& in = getInVal<ColorVariable>("In");
		return FloatVariable(in.value.green,in.isConstant);

	});
	ImFlow::BaseNode::addOUT<FloatVariable>("Blue",styles.floatVariable)->behaviour([this]() {
		const ColorVariable& in = getInVal<ColorVariable>("In");
		return FloatVariable(in.value.blue,in.isConstant);

	});
	ImFlow::BaseNode::addOUT<FloatVariable>("Alpha",styles.floatVariable)->behaviour([this]() {
		const ColorVariable& in = getInVal<ColorVariable>("In");
		return FloatVariable(in.value.alpha,in.isConstant);

	});
}

void SplitColorNode::draw() {
	const ColorVariable& col = getInVal<ColorVariable>("In");
	ImGui::Text("%f",col.value.red);
	ImGui::Text("%f",col.value.green);
	ImGui::Text("%f",col.value.blue);
	ImGui::Text("%f",col.value.alpha);
}

RGBToColorNode::RGBToColorNode(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle("RGB Color");
	setStyle(styles.splitMergeNode);
	ImFlow::BaseNode::addIN<FloatVariable>("Red",FloatVariable(1.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Green",FloatVariable(1.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Blue",FloatVariable(1.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Alpha",FloatVariable(1.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);

	ImFlow::BaseNode::addOUT<ColorVariable>("Out",styles.float2Variable)->behaviour([this]() {
		const FloatVariable& inRed = getInVal<FloatVariable>("Red");
		const FloatVariable& inGreen = getInVal<FloatVariable>("Green");
		const FloatVariable& inBlue = getInVal<FloatVariable>("Blue");
		const FloatVariable& inAlpha = getInVal<FloatVariable>("Alpha");
		bool isConstant = inRed.isConstant&&inGreen.isConstant&&inBlue.isConstant&&inAlpha.isConstant;
		return ColorVariable(inRed.value,inGreen.value,inBlue.value,inAlpha.value,isConstant);

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

	setTitle("HSV Color");
	setStyle(styles.splitMergeNode);
	ImFlow::BaseNode::addIN<FloatVariable>("Hue",FloatVariable(0.f), ImFlow::ConnectionFilter::SameType(), styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Saturation",FloatVariable(1.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Vibrance",FloatVariable(1.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Alpha",FloatVariable(1.f),ImFlow::ConnectionFilter::SameType(),styles.floatVariable);

	ImFlow::BaseNode::addOUT<ColorVariable>("Out",styles.float2Variable)->behaviour([this]() {

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


		bool isConstant = inH.isConstant&&inS.isConstant&&inV.isConstant&&inAlpha.isConstant;
		return ColorVariable(r,g,b,inAlpha.value,isConstant);

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