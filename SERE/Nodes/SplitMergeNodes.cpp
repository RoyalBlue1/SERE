#include "SplitMergeNodes.h"

SplitFloat2Node::SplitFloat2Node(RenderInstance& rend,NodeStyles& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<FloatVariable>("X")->behaviour([this]() {
		const Float2Variable& in = getInVal<Float2Variable>("In");
		return FloatVariable(in.value.x,in.isConstant);

	});
	getOut<FloatVariable>("Y")->behaviour([this]() {
		const Float2Variable& in = getInVal<Float2Variable>("In");
		return FloatVariable(in.value.y,in.isConstant);

	});
}

SplitFloat2Node::SplitFloat2Node(RenderInstance& rend,NodeStyles& style, rapidjson::GenericObject<false,rapidjson::Value> obj):SplitFloat2Node(rend,style){}

void SplitFloat2Node::draw() {
	const Float2Variable& in = getInVal<Float2Variable>("In");
	ImGui::Text("%f",in.value.x);
	ImGui::Text("%f",in.value.y);

}


void SplitFloat2Node::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> SplitFloat2Node::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("In",ImFlow::ConnectionFilter::SameType(),Float2Variable(0.f,0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("X"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Y"));
	return info;
}

MergeFloat2Node::MergeFloat2Node(RenderInstance& rend,NodeStyles& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<Float2Variable>("Out")->behaviour([this]() {

		const FloatVariable& inX = getInVal<FloatVariable>("X");
		const FloatVariable& inY = getInVal<FloatVariable>("Y");
		bool isConstant = inX.isConstant&&inY.isConstant;

		return Float2Variable(inX.value,inY.value,isConstant);

	});
}

MergeFloat2Node::MergeFloat2Node(RenderInstance& rend,NodeStyles& style, rapidjson::GenericObject<false,rapidjson::Value> obj):MergeFloat2Node(rend,style){}

void MergeFloat2Node::draw() {
	const FloatVariable& inX = getInVal<FloatVariable>("X");
	const FloatVariable& inY = getInVal<FloatVariable>("Y");
	ImGui::Text("%f",inX.value);
	ImGui::Text("%f",inY.value);
}

void MergeFloat2Node::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> MergeFloat2Node::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("X",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Y",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float2Variable>>("Out"));
	return info;
}

SplitFloat3Node::SplitFloat3Node(RenderInstance& rend,NodeStyles& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<FloatVariable>("X")->behaviour([this]() {
		const Float3Variable& in = getInVal<Float3Variable>("In");
		return FloatVariable(in.value.x,in.isConstant);

	});
	getOut<FloatVariable>("Y")->behaviour([this]() {
		const Float3Variable& in = getInVal<Float3Variable>("In");
		return FloatVariable(in.value.y,in.isConstant);

	});
	getOut<FloatVariable>("Z")->behaviour([this]() {
		const Float3Variable& in = getInVal<Float3Variable>("In");
		return FloatVariable(in.value.z,in.isConstant);

	});
}

SplitFloat3Node::SplitFloat3Node(RenderInstance& rend,NodeStyles& style, rapidjson::GenericObject<false,rapidjson::Value> obj):SplitFloat3Node(rend,style){}

void SplitFloat3Node::draw() {
	const Float3Variable& in = getInVal<Float3Variable>("In");
	ImGui::Text("%f",in.value.x);
	ImGui::Text("%f",in.value.y);
	ImGui::Text("%f",in.value.z);

}

void SplitFloat3Node::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> SplitFloat3Node::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<Float3Variable>>("In",ImFlow::ConnectionFilter::SameType(),Float3Variable(0.f,0.f,0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("X"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Y"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Z"));
	return info;
}

MergeFloat3Node::MergeFloat3Node(RenderInstance& rend,NodeStyles& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<Float3Variable>("Out")->behaviour([this]() {

		const FloatVariable& inX = getInVal<FloatVariable>("X");
		const FloatVariable& inY = getInVal<FloatVariable>("Y");
		const FloatVariable& inZ = getInVal<FloatVariable>("Z");
		bool isConstant = inX.isConstant&&inY.isConstant&&inZ.isConstant;
		return Float3Variable(inX.value,inY.value,inZ.value,isConstant);


	});
}

MergeFloat3Node::MergeFloat3Node(RenderInstance& rend,NodeStyles& style, rapidjson::GenericObject<false,rapidjson::Value> obj):MergeFloat3Node(rend,style){}

void MergeFloat3Node::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void MergeFloat3Node::draw() {
	const FloatVariable& inX = getInVal<FloatVariable>("X");
	const FloatVariable& inY = getInVal<FloatVariable>("Y");
	const FloatVariable& inZ = getInVal<FloatVariable>("Z");
	ImGui::Text("%f",inX.value);
	ImGui::Text("%f",inY.value);
	ImGui::Text("%f",inZ.value);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> MergeFloat3Node::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("X",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Y",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Z",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<Float3Variable>>("Out"));
	return info;
}

SplitColorNode::SplitColorNode(RenderInstance& rend,NodeStyles& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<FloatVariable>("Red")->behaviour([this]() {

		const ColorVariable& in = getInVal<ColorVariable>("In");
		return FloatVariable(in.value.red,in.isConstant);

	});
	getOut<FloatVariable>("Green")->behaviour([this]() {
		const ColorVariable& in = getInVal<ColorVariable>("In");
		return FloatVariable(in.value.green,in.isConstant);

	});
	getOut<FloatVariable>("Blue")->behaviour([this]() {
		const ColorVariable& in = getInVal<ColorVariable>("In");
		return FloatVariable(in.value.blue,in.isConstant);

	});
	getOut<FloatVariable>("Alpha")->behaviour([this]() {
		const ColorVariable& in = getInVal<ColorVariable>("In");
		return FloatVariable(in.value.alpha,in.isConstant);

	});
}

SplitColorNode::SplitColorNode(RenderInstance& rend,NodeStyles& style, rapidjson::GenericObject<false,rapidjson::Value> obj):SplitColorNode(rend,style){}

void SplitColorNode::draw() {
	const ColorVariable& col = getInVal<ColorVariable>("In");
	ImGui::Text("%f",col.value.red);
	ImGui::Text("%f",col.value.green);
	ImGui::Text("%f",col.value.blue);
	ImGui::Text("%f",col.value.alpha);
}

void SplitColorNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> SplitColorNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<ColorVariable>>("In",ImFlow::ConnectionFilter::SameType(),ColorVariable(0.f,0.f,0.f,0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Red"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Green"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Blue"));
	info.push_back(std::make_shared<ImFlow::OutPinProto<FloatVariable>>("Alpha"));
	
	return info;
}

RGBToColorNode::RGBToColorNode(RenderInstance& rend,NodeStyles& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<ColorVariable>("Out")->behaviour([this]() {
		const FloatVariable& inRed = getInVal<FloatVariable>("Red");
		const FloatVariable& inGreen = getInVal<FloatVariable>("Green");
		const FloatVariable& inBlue = getInVal<FloatVariable>("Blue");
		const FloatVariable& inAlpha = getInVal<FloatVariable>("Alpha");
		bool isConstant = inRed.isConstant&&inGreen.isConstant&&inBlue.isConstant&&inAlpha.isConstant;
		return ColorVariable(inRed.value,inGreen.value,inBlue.value,inAlpha.value,isConstant);

	});
}

RGBToColorNode::RGBToColorNode(RenderInstance& rend,NodeStyles& style, rapidjson::GenericObject<false,rapidjson::Value> obj):RGBToColorNode(rend,style){}

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

void RGBToColorNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> RGBToColorNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Red",ImFlow::ConnectionFilter::SameType(),FloatVariable(1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Green",ImFlow::ConnectionFilter::SameType(),FloatVariable(1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Blue",ImFlow::ConnectionFilter::SameType(),FloatVariable(1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Alpha",ImFlow::ConnectionFilter::SameType(),FloatVariable(1.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<ColorVariable>>("Out"));
	return info;
}

HSVToColorNode::HSVToColorNode(RenderInstance& rend,NodeStyles& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getOut<ColorVariable>("Out")->behaviour([this]() {

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

HSVToColorNode::HSVToColorNode(RenderInstance& rend,NodeStyles& style, rapidjson::GenericObject<false,rapidjson::Value> obj):HSVToColorNode(rend,style){}

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

void HSVToColorNode::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> HSVToColorNode::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Hue",ImFlow::ConnectionFilter::SameType(),FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Saturation",ImFlow::ConnectionFilter::SameType(),FloatVariable(1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Vibrance",ImFlow::ConnectionFilter::SameType(),FloatVariable(1.f)));
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Alpha",ImFlow::ConnectionFilter::SameType(),FloatVariable(1.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<ColorVariable>>("Out"));
	return info;
}

void AddSplitMergeNodes(NodeEditor& editor) {
	editor.AddNodeType<MergeFloat2Node>();
	editor.AddNodeType<MergeFloat2Node>();
	editor.AddNodeType<SplitFloat3Node>();
	editor.AddNodeType<MergeFloat3Node>();
	editor.AddNodeType<SplitColorNode>();
	editor.AddNodeType<RGBToColorNode>();
	editor.AddNodeType<HSVToColorNode>();
}