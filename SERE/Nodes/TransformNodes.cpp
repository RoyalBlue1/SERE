#include "TransformNodes.h"
#include "imgui/imgui_stdlib.h"
#include "Util.h"

__m128 xmmword_12A146C0 = _mm_castsi128_ps(_mm_set_epi32(0xFFFFFFFF,0,0,0xFFFFFFFF));

Transform0Node::Transform0Node(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	uint64_t outHash = randomInt64();
	getOut<TransformResult>("Out")->behaviour([this,outHash]() {
		TransformResult res; 

		res.directionVector = _mm_and_ps(getInVal<TransformSize>("Size").size, (__m128)xmmword_12A146C0);
		res.position = _mm_setzero_ps();
		res.hash = outHash;
		return res;
	});
}

Transform0Node::Transform0Node(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):Transform0Node(rend,style){}

void Transform0Node::draw() {
	ImGui::PushItemWidth(90);

	ImGui::PopItemWidth();
}

void Transform0Node::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void Transform0Node::Export(RuiExportPrototype& proto) {

}

std::vector<std::shared_ptr<ImFlow::PinProto>> Transform0Node::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<TransformSize>>("Size",ImFlow::ConnectionFilter::SameType(),TransformSize(_mm_set1_ps(64.f))));
	info.push_back(std::make_shared<ImFlow::OutPinProto<TransformResult>>("Out"));
	return info;
}

Transform1Node::Transform1Node(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getIn<TransformResult>("Source")->setEmptyVal(render.transformResults[2]);
	getOut<TransformResult>("Out")->behaviour([this]() {
		TransformResult res;
		const TransformResult& parent = getInVal<TransformResult>("Source");
		const TransformSize& size = getInVal<TransformSize>("Size");

		res.position = parent.position;
		res.directionVector = parent.directionVector;
		res.inputSize = size.size;
		render.transformResults.push_back(res);
		return res;
	});;
}

Transform1Node::Transform1Node(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):Transform1Node(rend,style){}

void Transform1Node::draw() {
	ImGui::PushItemWidth(90);

	ImGui::PopItemWidth();
}

void Transform1Node::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void Transform1Node::Export(RuiExportPrototype& proto) {

}

std::vector<std::shared_ptr<ImFlow::PinProto>> Transform1Node::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<TransformResult>>("Source",ImFlow::ConnectionFilter::SameType(),TransformResult()));
	info.push_back(std::make_shared<ImFlow::InPinProto<TransformSize>>("Size",ImFlow::ConnectionFilter::SameType(),TransformSize(_mm_set1_ps(64.f))));
	info.push_back(std::make_shared<ImFlow::OutPinProto<TransformResult>>("Out"));
	return info;
}

Transform2Node::Transform2Node(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getIn<TransformResult>("Parent")->setEmptyVal(render.transformResults[2]);
	uint64_t outHash = randomInt64();
	getOut<TransformResult>("Out")->behaviour([this,outHash]() {
		TransformResult res;
		res.hash = outHash;
		const Float2Variable& val_0 = getInVal<Float2Variable>("Val_0");
		const Float2Variable& val_3 = getInVal<Float2Variable>("Val_3");
		const TransformSize& size = getInVal<TransformSize>("Size");
		const TransformResult& parent = getInVal<TransformResult>("Parent");

		__m128 elementSizeRatio = _mm_set_ps(0,0,render.elementHeightRatio,render.elementWidthRatio);
		__m128 elementSizeRatio_unpacked = _mm_unpacklo_ps(elementSizeRatio, elementSizeRatio);
		__m128 v13 = _mm_and_ps(_mm_mul_ps(size.size, elementSizeRatio_unpacked), (__m128)xmmword_12A146C0);
		__m128 v14 = _mm_shuffle_ps(
			_mm_set_ps(0,0,0,val_0.value.x),
			_mm_set_ps(0,0,0,val_0.value.y),
			0);
		__m128 v15 = _mm_set_ps(0,0,0,val_3.value.x);
		__m128 v16 = _mm_set_ps(0,0,0,val_0.value.y);
		res.directionVector = v13;
		__m128 v18 = _mm_mul_ps(v14, parent.directionVector);
		__m128 v19 = _mm_mul_ps(_mm_shuffle_ps(v15, v16, 0), v13);
		res.position = _mm_sub_ps(
			_mm_add_ps(
				_mm_add_ps((__m128)_mm_shuffle_ps(v18,v18, 78), v18),
				parent.position),
			_mm_add_ps((__m128)_mm_shuffle_ps(v19,v19, 78), v19));
		res.inputSize = size.size;
		render.transformResults.push_back(res);
		return res;
	});
}

Transform2Node::Transform2Node(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):Transform2Node(rend,style){}

void Transform2Node::draw() {
	ImGui::PushItemWidth(90);

	ImGui::PopItemWidth();
}

void Transform2Node::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void Transform2Node::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<TransformResult>("Out")->val();
	const auto& v0 = getInVal<Float2Variable>("Val_0");
	const auto& v3 = getInVal<Float2Variable>("Val_3");
	const auto& parent = getInVal<TransformResult>("Parent");
	const auto& size = getInVal<TransformSize>("Size");
	proto.AddDataVariable(v0);
	proto.AddDataVariable(v3);
	ExportElement<uint64_t> ele;
	ele.identifier = out.hash;
	ele.dependencys = {parent.hash};
	ele.callback = [parent,v0,v3,out,size](RuiExportPrototype& proto) {
		uint16_t transId = (uint16_t)proto.transformIndices.size();
		proto.transformIndices.emplace(out.hash,transId);
		ExportElement<std::string> ele;
		ele.identifier = Variable::UniqueName();
		ele.dependencys = { size.name };
		ele.callback = [transId, size](RuiExportPrototype& proto) {
			proto.codeLines.push_back(std::format("transformSize[{}] = {};",transId,size.GetFormattedName(proto)));
		};
		proto.codeElements.push_back(ele);
		struct Transform2FileStruct {
			uint8_t type = 2;
			uint8_t count = 1;
			uint16_t parent;
			Float2Offsets val0;
			Float2Offsets val3;
		};
		Transform2FileStruct trans{};
		trans.parent = proto.transformIndices[parent.hash];
		trans.val0 = proto.GetFloat2DataVariableOffset(v0);
		trans.val3 = proto.GetFloat2DataVariableOffset(v3);
		proto.AddTransformData((uint8_t*)&trans,sizeof(trans));
	};
	proto.transformCallbacks.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> Transform2Node::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<TransformResult>>("Parent",ImFlow::ConnectionFilter::SameType(),TransformResult()));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Val_0",ImFlow::ConnectionFilter::SameType(),Float2Variable(.5f,.5f)));

	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Val_3",ImFlow::ConnectionFilter::SameType(),Float2Variable(.5f,.5f)));

	info.push_back(std::make_shared<ImFlow::InPinProto<TransformSize>>("Size",ImFlow::ConnectionFilter::SameType(),TransformSize(_mm_set1_ps(64.f))));

	info.push_back(std::make_shared<ImFlow::OutPinProto<TransformResult>>("Out"));
	return info;
}

Transform3Node::Transform3Node(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getIn<TransformResult>("Parent")->setEmptyVal(render.transformResults[2]);
	uint64_t outHash = randomInt64();
	getOut<TransformResult>("Out")->behaviour([this,outHash]() {
		TransformResult res;
		res.hash = outHash;
		const Float2Variable& val_0 = getInVal<Float2Variable>("Val_0");
		const Float2Variable& val_3 = getInVal<Float2Variable>("Val_3");
		const TransformResult& parent = getInVal<TransformResult>("Parent");
		__m128 v9 = _mm_shuffle_ps(
			_mm_set_ps(0,0,0,val_0.value.x),
			_mm_set_ps(0,0,0,val_0.value.y),
			0);

		__m128 v11 = _mm_mul_ps(parent.directionVector, parent.directionVector);
		__m128 v12 = _mm_and_ps(
			_mm_sqrt_ps(_mm_add_ps(_mm_shuffle_ps(v11,v11, 177), v11)),
			xmmword_12A146C0);
		__m128 v13 = _mm_mul_ps(
			_mm_shuffle_ps(
				_mm_set_ps(0,0,0,val_3.value.x),
				_mm_set_ps(0,0,0,val_3.value.y),
				0),
			v12);
		res.directionVector = v12;
		__m128 v14 = _mm_mul_ps(v9, parent.directionVector);
		res.position = _mm_sub_ps(
			_mm_add_ps(
				_mm_add_ps(_mm_shuffle_ps(v14,v14, 78), v14),
				parent.position),
			_mm_add_ps(_mm_shuffle_ps(v13,v13, 78), v13));
		render.transformResults.push_back(res);
		return res;
	});
}

Transform3Node::Transform3Node(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):Transform3Node(rend,style){}

void Transform3Node::draw() {
	ImGui::PushItemWidth(90);

	ImGui::PopItemWidth();
}

void Transform3Node::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void Transform3Node::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<TransformResult>("Out")->val();
	const auto& v0 = getInVal<Float2Variable>("Val_0");
	const auto& v3 = getInVal<Float2Variable>("Val_3");
	const auto& parent = getInVal<TransformResult>("Parent");
	const auto& size = getInVal<TransformSize>("Size");
	proto.AddDataVariable(v0);
	proto.AddDataVariable(v3);
	ExportElement<uint64_t> ele;
	ele.identifier = out.hash;
	ele.dependencys = {parent.hash};
	ele.callback = [parent,v0,v3,out,size](RuiExportPrototype& proto) {
		uint16_t transId = (uint16_t)proto.transformIndices.size();
		proto.transformIndices.emplace(out.hash,transId);
		ExportElement<std::string> ele;
		ele.identifier = Variable::UniqueName();
		ele.dependencys = { size.name };
		ele.callback = [transId, size](RuiExportPrototype& proto) {
			proto.codeLines.push_back(std::format("transformSize[{}] = {};",transId,size.GetFormattedName(proto)));
		};
		proto.codeElements.push_back(ele);
		struct Transform3FileStruct {
			uint8_t type = 3;
			uint8_t count = 1;
			uint16_t parent;
			Float2Offsets val0;
			Float2Offsets val3;
		};
		Transform3FileStruct trans{};
		trans.parent = proto.transformIndices[parent.hash];
		trans.val0 = proto.GetFloat2DataVariableOffset(v0);
		trans.val3 = proto.GetFloat2DataVariableOffset(v3);
		proto.AddTransformData((uint8_t*)&trans,sizeof(trans));
	};
	proto.transformCallbacks.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> Transform3Node::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<TransformResult>>("Parent",ImFlow::ConnectionFilter::SameType(),TransformResult()));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Val_0",ImFlow::ConnectionFilter::SameType(),Float2Variable(.5f,.5f)));

	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Val_3",ImFlow::ConnectionFilter::SameType(),Float2Variable(.5f,.5f)));

	info.push_back(std::make_shared<ImFlow::InPinProto<TransformSize>>("Size",ImFlow::ConnectionFilter::SameType(),TransformSize(_mm_set1_ps(64.f))));

	info.push_back(std::make_shared<ImFlow::OutPinProto<TransformResult>>("Out"));
	return info;
}

Transform4Node::Transform4Node(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getIn<TransformResult>("Parent")->setEmptyVal(render.transformResults[2]);
	uint64_t outHash = randomInt64();
	getOut<TransformResult>("Out")->behaviour([this,outHash]() {
		TransformResult res;
		res.hash = outHash;
		const Float2Variable& val_0 = getInVal<Float2Variable>("Val_0");
		const Float2Variable& val_3 = getInVal<Float2Variable>("Val_3");
		const TransformSize& size = getInVal<TransformSize>("Size");
		const TransformResult& parent = getInVal<TransformResult>("Parent");

		__m128 v11 = _mm_shuffle_ps(
			_mm_set_ps(0,0,0,val_0.value.x),
			_mm_set_ps(0,0,0,val_0.value.y),
			0);


		__m128 v14 = _mm_and_ps(
			_mm_mul_ps(_mm_div_ps(size.size, parent.inputSize), parent.directionVector),
			_mm_cmpneq_ps(_mm_setzero_ps(), size.size));
		__m128 v15 = _mm_mul_ps(
			_mm_shuffle_ps(
				_mm_set_ps(0,0,0,val_3.value.x),
				_mm_set_ps(0,0,0,val_3.value.y),
				0),
			v14);
		res.directionVector = v14;
		__m128 v16 = _mm_mul_ps(v11, parent.directionVector);
		res.position = _mm_sub_ps(
			_mm_add_ps(
				_mm_add_ps(_mm_shuffle_ps(v16,v16, _MM_SHUFFLE(1,0,3,2)), v16),
				parent.position),
			_mm_add_ps(_mm_shuffle_ps(v15,v15, _MM_SHUFFLE(1,0,3,2)), v15));
		res.inputSize = size.size;
		render.transformResults.push_back(res);
		return res;
	});
}

Transform4Node::Transform4Node(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):Transform4Node(rend,style){}

void Transform4Node::draw() {
	ImGui::PushItemWidth(90);

	ImGui::PopItemWidth();
}

void Transform4Node::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void Transform4Node::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<TransformResult>("Out")->val();
	const auto& v0 = getInVal<Float2Variable>("Val_0");
	const auto& v3 = getInVal<Float2Variable>("Val_3");
	const auto& parent = getInVal<TransformResult>("Parent");
	const auto& size = getInVal<TransformSize>("Size");
	proto.AddDataVariable(v0);
	proto.AddDataVariable(v3);
	ExportElement<uint64_t> ele;
	ele.identifier = out.hash;
	ele.dependencys = {parent.hash};
	ele.callback = [parent,v0,v3,out,size](RuiExportPrototype& proto) {
		uint16_t transId = (uint16_t)proto.transformIndices.size();
		proto.transformIndices.emplace(out.hash,transId);
		ExportElement<std::string> ele;
		ele.identifier = Variable::UniqueName();
		ele.dependencys = { size.name };
		ele.callback = [transId, size](RuiExportPrototype& proto) {
			proto.codeLines.push_back(std::format("transformSize[{}] = {};",transId,size.GetFormattedName(proto)));
		};
		proto.codeElements.push_back(ele);
		struct Transform4FileStruct {
			uint8_t type = 4;
			uint8_t count = 1;
			uint16_t parent;
			Float2Offsets val0;
			Float2Offsets val3;
		};
		Transform4FileStruct trans{};
		trans.parent = proto.transformIndices[parent.hash];
		trans.val0 = proto.GetFloat2DataVariableOffset(v0);
		trans.val3 = proto.GetFloat2DataVariableOffset(v3);
		proto.AddTransformData((uint8_t*)&trans,sizeof(trans));
	};
	proto.transformCallbacks.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> Transform4Node::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<TransformResult>>("Parent",ImFlow::ConnectionFilter::SameType(),TransformResult()));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Val_0",ImFlow::ConnectionFilter::SameType(),Float2Variable(.5f,.5f)));

	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Val_3",ImFlow::ConnectionFilter::SameType(),Float2Variable(.5f,.5f)));

	info.push_back(std::make_shared<ImFlow::InPinProto<TransformSize>>("Size",ImFlow::ConnectionFilter::SameType(),TransformSize(_mm_set1_ps(64.f))));

	info.push_back(std::make_shared<ImFlow::OutPinProto<TransformResult>>("Out"));
	return info;
}

void sub_100520(RenderInstance& render,__m128* a2,__m128* a3) {
	__m128 m128_10 = *(__m128 *)&render.drawInfo.ruiUnk3[0].screenWidth;
	__m128 m128_20 = *(__m128 *)&render.drawInfo.ruiUnk3[0].float_10;

	__m128 v6 = _mm_mul_ps(m128_20, m128_20);
	__m128 v7 = _mm_mul_ps(m128_10, m128_10);

	__m128 v9 = _mm_add_ps(_mm_unpacklo_ps(v7, v6), _mm_unpackhi_ps(v7, v6));
	__m128 v10 = _mm_sqrt_ps(_mm_add_ps((__m128)_mm_shuffle_ps(v9,v9, _MM_SHUFFLE(1,0,3,2)), v9));
	*a2 = _mm_shuffle_ps(v10,v10, _MM_SHUFFLE(0,0,0,0));
	*a3 = _mm_shuffle_ps(v10,v10, _MM_SHUFFLE(1,1,1,1));
}

Transform5Node::Transform5Node(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getIn<TransformResult>("Parent")->setEmptyVal(render.transformResults[2]);
	uint64_t outHash = randomInt64();
	getOut<TransformResult>("Out")->behaviour([this,outHash]() {
		TransformResult res;
		res.hash = outHash;
		const Float2Variable& val_0 = getInVal<Float2Variable>("Val_0");
		const Float2Variable& val_3 = getInVal<Float2Variable>("Val_3");
		const TransformSize& size = getInVal<TransformSize>("Size");
		const TransformResult& parent = getInVal<TransformResult>("Parent");


		__m128 v25;
		__m128 v26;
		sub_100520(render, &v25, &v26);

		__m128 v6 = _mm_unpacklo_ps(v25, v26);
		
		__m128 v17 = _mm_mul_ps(
			_mm_shuffle_ps(
				_mm_set_ps(0,0,0,val_0.value.x), 
				_mm_set_ps(0,0,0,val_0.value.y), 
				0),
			parent.directionVector);
		
		__m128 v19 = _mm_add_ps(
			_mm_add_ps((__m128)_mm_shuffle_ps(v17,v17, 78), (__m128)v17),
			parent.position);

		__m128 v16 = _mm_set_ps(0,0,0,val_3.value.x);
		__m128 v18 = _mm_set_ps(0,0,0,val_3.value.y);


		__m128 v22 = _mm_xor_ps(
			_mm_mul_ps((__m128)_mm_shuffle_ps(parent.inputSize,parent.inputSize, 0), v6),
			_mm_set_ps(0,-0.0,0,0));
		__m128 a = _mm_mul_ps(parent.directionVector, v6);
		__m128 v23 = _mm_and_ps(
			_mm_div_ps(
				_mm_mul_ps(
					(__m128)_mm_shuffle_ps(a,a, 20),
					size.size),
				v22),
			_mm_cmpneq_ps(_mm_setzero_ps(), v22));
		__m128 v24 = _mm_mul_ps(_mm_shuffle_ps(v16, v18, 0), v23);
		res.directionVector = v23;
		res.position = _mm_sub_ps(v19, _mm_add_ps(_mm_shuffle_ps(v24,v24, 78), (__m128)v24));
		res.inputSize = size.size;


		render.transformResults.push_back(res);
		return res;
	});
}

Transform5Node::Transform5Node(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):Transform5Node(rend,style){}

void Transform5Node::draw() {
	ImGui::PushItemWidth(90);

	ImGui::PopItemWidth();
}

void Transform5Node::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void Transform5Node::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<TransformResult>("Out")->val();
	const auto& v0 = getInVal<Float2Variable>("Val_0");
	const auto& v3 = getInVal<Float2Variable>("Val_3");
	const auto& parent = getInVal<TransformResult>("Parent");
	const auto& size = getInVal<TransformSize>("Size");
	proto.AddDataVariable(v0);
	proto.AddDataVariable(v3);
	ExportElement<uint64_t> ele;
	ele.identifier = out.hash;
	ele.dependencys = {parent.hash};
	ele.callback = [parent,v0,v3,out,size](RuiExportPrototype& proto) {
		uint16_t transId = (uint16_t)proto.transformIndices.size();
		proto.transformIndices.emplace(out.hash,transId);
		ExportElement<std::string> ele;
		ele.identifier = Variable::UniqueName();
		ele.dependencys = { size.name };
		ele.callback = [transId, size](RuiExportPrototype& proto) {
			proto.codeLines.push_back(std::format("transformSize[{}] = {};",transId,size.GetFormattedName(proto)));
		};
		proto.codeElements.push_back(ele);
		struct Transform5FileStruct {
			uint8_t type = 5;
			uint8_t count = 1;
			uint16_t parent;
			Float2Offsets val0;
			Float2Offsets val3;
		};
		Transform5FileStruct trans{};
		trans.parent = proto.transformIndices[parent.hash];
		trans.val0 = proto.GetFloat2DataVariableOffset(v0);
		trans.val3 = proto.GetFloat2DataVariableOffset(v3);
		proto.AddTransformData((uint8_t*)&trans,sizeof(trans));
	};
	proto.transformCallbacks.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> Transform5Node::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<TransformResult>>("Parent",ImFlow::ConnectionFilter::SameType(),TransformResult()));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Val_0",ImFlow::ConnectionFilter::SameType(),Float2Variable(.5f,.5f)));

	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Val_3",ImFlow::ConnectionFilter::SameType(),Float2Variable(.5f,.5f)));

	info.push_back(std::make_shared<ImFlow::InPinProto<TransformSize>>("Size",ImFlow::ConnectionFilter::SameType(),TransformSize(_mm_set1_ps(64.f))));

	info.push_back(std::make_shared<ImFlow::OutPinProto<TransformResult>>("Out"));
	return info;
}

Transform6Node::Transform6Node(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {


	getIn<TransformResult>("Parent")->setEmptyVal(render.transformResults[2]);
	uint64_t outHash = randomInt64();
	getOut<TransformResult>("Out")->behaviour([this,outHash]() {
		TransformResult res;
		res.hash = outHash;
		const Float2Variable& val_0 = getInVal<Float2Variable>("Val_0");
		const Float2Variable& val_3 = getInVal<Float2Variable>("Val_3");
		const TransformSize& size = getInVal<TransformSize>("Size");
		const TransformResult& parent = getInVal<TransformResult>("Parent");

		__m128 v25;
		__m128 v26;
		sub_100520(render, &v25, &v26);

		__m128 v6 = _mm_unpacklo_ps(v25, v26);

		__m128 v13 = _mm_set_ps(0,0,0,val_0.value.y);



		__m128 v16 = _mm_set_ps(0,0,0,val_3.value.x);
		__m128 v17 = _mm_mul_ps(
			_mm_shuffle_ps(_mm_set_ps(0,0,0,val_0.value.x), v13, 0),
			parent.directionVector);
		__m128 v18 = _mm_set_ps(0,0,0,val_3.value.y);
		__m128 v19 = _mm_add_ps(
			_mm_add_ps((__m128)_mm_shuffle_ps(v17,v17, 78), v17),
			parent.position);
		__m128 v22 = _mm_xor_ps(
			_mm_mul_ps((__m128)_mm_shuffle_ps(parent.inputSize,parent.inputSize, 255), v6),
			_mm_set_ps(0,0,-0.0,0));
		__m128 a = _mm_mul_ps(parent.directionVector, v6);
		__m128 v23 = _mm_and_ps(
			_mm_div_ps(
				_mm_mul_ps(
					(__m128)_mm_shuffle_ps(a,a, 235),
					size.size),
				v22),
			_mm_cmpneq_ps(_mm_setzero_ps(), v22));
		__m128 v24 = _mm_mul_ps(_mm_shuffle_ps(v16, v18, 0), v23);
		res.directionVector = v23;
		res.position = _mm_sub_ps(v19, _mm_add_ps(_mm_shuffle_ps(v24,v24, 78), (__m128)v24));
		res.inputSize = size.size;

		render.transformResults.push_back(res);
		return res;
	});
}

Transform6Node::Transform6Node(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):Transform6Node(rend,style){}

void Transform6Node::draw() {
	ImGui::PushItemWidth(90);

	ImGui::PopItemWidth();
}

void Transform6Node::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void Transform6Node::Export(RuiExportPrototype& proto) {
	const auto& out = getOut<TransformResult>("Out")->val();
	const auto& v0 = getInVal<Float2Variable>("Val_0");
	const auto& v3 = getInVal<Float2Variable>("Val_3");
	const auto& parent = getInVal<TransformResult>("Parent");
	const auto& size = getInVal<TransformSize>("Size");
	proto.AddDataVariable(v0);
	proto.AddDataVariable(v3);
	ExportElement<uint64_t> ele;
	ele.identifier = out.hash;
	ele.dependencys = {parent.hash};
	ele.callback = [parent,v0,v3,out,size](RuiExportPrototype& proto) {
		struct Transform6FileStruct {
			uint8_t type = 6;
			uint8_t count = 1;
			uint16_t parent;
			Float2Offsets val0;
			Float2Offsets val3;
		};
		uint16_t transId = (uint16_t)proto.transformIndices.size();
		proto.transformIndices.emplace(out.hash,transId);
		ExportElement<std::string> ele;
		ele.identifier = Variable::UniqueName();
		ele.dependencys = { size.name };
		ele.callback = [transId, size](RuiExportPrototype& proto) {
			proto.codeLines.push_back(std::format("transformSize[{}] = {};",transId,size.GetFormattedName(proto)));
		};
		proto.codeElements.push_back(ele);
		Transform6FileStruct trans{};
		trans.parent = proto.transformIndices[parent.hash];
		trans.val0 = proto.GetFloat2DataVariableOffset(v0);
		trans.val3 = proto.GetFloat2DataVariableOffset(v3);
		proto.AddTransformData((uint8_t*)&trans,sizeof(trans));
	};
	proto.transformCallbacks.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> Transform6Node::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<TransformResult>>("Parent",ImFlow::ConnectionFilter::SameType(),TransformResult()));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Val_0",ImFlow::ConnectionFilter::SameType(),Float2Variable(.5f,.5f)));

	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Val_3",ImFlow::ConnectionFilter::SameType(),Float2Variable(.5f,.5f)));

	info.push_back(std::make_shared<ImFlow::InPinProto<TransformSize>>("Size",ImFlow::ConnectionFilter::SameType(),TransformSize(_mm_set1_ps(64.f))));

	info.push_back(std::make_shared<ImFlow::OutPinProto<TransformResult>>("Out"));
	return info;
}

Transform7Node::Transform7Node(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {
	
	getIn<TransformResult>("Pin 1 Parent")->setEmptyVal(render.transformResults[2]);
	getIn<TransformResult>("Pin 2 Parent")->setEmptyVal(render.transformResults[2]);
	uint64_t outHash = randomInt64();
	getOut<TransformResult>("Out")->behaviour([this,outHash]() {
		TransformResult res;
		res.hash = outHash;
		const TransformSize& size = getInVal<TransformSize>("Size");
		const TransformResult& p1parent = getInVal<TransformResult>("Pin 1 Parent");
		const Float2Variable& p1Pos = getInVal<Float2Variable>("Pin 1 Position");
		const TransformResult& p2parent = getInVal<TransformResult>("Pin 2 Parent");
		const Float2Variable& p2Pos = getInVal<Float2Variable>("Pin 2 Position");
		const Float2Variable& translate = getInVal<Float2Variable>("Translate");
		const Float2Variable& point = getInVal<Float2Variable>("Point 1");


		__m128 v6 = _mm_shuffle_ps(
			_mm_set_ps(0,0,0,p1Pos.value.x),
			_mm_set_ps(0,0,0,p1Pos.value.y),
			0);
		__m128 v7 = _mm_shuffle_ps(
			_mm_set_ps(0,0,0,translate.value.x),
			_mm_set_ps(0,0,0,translate.value.y),
			0);
		__m128 v8 = _mm_shuffle_ps(
			_mm_set_ps(0,0,0,p2Pos.value.x),
			_mm_set_ps(0,0,0,p2Pos.value.y),
			0);


		__m128 v10 = _mm_shuffle_ps(
			_mm_set_ps(0,0,0,point.value.x),
			_mm_set_ps(0,0,0,point.value.y),
			20);


		__m128 v12 = _mm_mul_ps(v6, p1parent.inputSize);
		__m128 v13 = _mm_mul_ps(v8, p2parent.inputSize);
		__m128 v14 = _mm_add_ps(
			_mm_add_ps(_mm_shuffle_ps(v12,v12, 78), v12),
			p1parent.position);
		__m128 v15 = _mm_mul_ps(
			_mm_sub_ps(
				_mm_add_ps(
					_mm_add_ps((__m128)_mm_shuffle_ps(v13,v13, 78), v13),
					p2parent.position),
				v14),
			v10);
		__m128 v16 = _mm_mul_ps(v7, v15);

		res.directionVector = v15;
		res.position = _mm_sub_ps(v14, _mm_add_ps(_mm_shuffle_ps(v16,v16, 78), v16));


		res.inputSize = size.size;

		render.transformResults.push_back(res);
		return res;
	});
}

Transform7Node::Transform7Node(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):Transform7Node(rend,style){}

void Transform7Node::draw() {
	ImGui::PushItemWidth(90);

	ImGui::PopItemWidth();
}

void Transform7Node::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void Transform7Node::Export(RuiExportPrototype& proto) {
	const TransformSize& size = getInVal<TransformSize>("Size");
	const TransformResult& p1parent = getInVal<TransformResult>("Pin 1 Parent");
	const Float2Variable& p1Pos = getInVal<Float2Variable>("Pin 1 Position");
	const TransformResult& p2parent = getInVal<TransformResult>("Pin 2 Parent");
	const Float2Variable& p2Pos = getInVal<Float2Variable>("Pin 2 Position");
	const Float2Variable& translate = getInVal<Float2Variable>("Translate");
	const Float2Variable& point = getInVal<Float2Variable>("Point 1");
	const TransformResult& out = getOut<TransformResult>("Out")->val();
	proto.AddDataVariable(p1Pos);
	proto.AddDataVariable(p2Pos);
	proto.AddDataVariable(translate);
	proto.AddDataVariable(point);
	ExportElement<uint64_t> ele;
	ele.identifier = out.hash;
	ele.dependencys = {p1parent.hash,p2parent.hash};
	ele.callback = [p1parent,p1Pos,p2parent,p2Pos,translate,point,out,size](RuiExportPrototype& proto) {
		struct Transform7FileStruct {
			uint8_t type = 7;
			uint8_t count = 1;
			uint16_t p1parent;
			Float2Offsets p1pos;
			uint16_t p2parent;
			Float2Offsets p2pos;
			Float2Offsets translate;
			Float2Offsets point;
		};
		uint16_t transId = (uint16_t)proto.transformIndices.size();
		proto.transformIndices.emplace(out.hash,transId);
		ExportElement<std::string> ele;
		ele.identifier = Variable::UniqueName();
		ele.dependencys = { size.name };
		ele.callback = [transId, size](RuiExportPrototype& proto) {
			proto.codeLines.push_back(std::format("transformSize[{}] = {};",transId,size.GetFormattedName(proto)));
		};
		proto.codeElements.push_back(ele);
		Transform7FileStruct trans{};
		trans.p1parent = proto.transformIndices[p1parent.hash];
		trans.p1pos = proto.GetFloat2DataVariableOffset(p1Pos);
		trans.p1parent = proto.transformIndices[p2parent.hash];
		trans.p1pos = proto.GetFloat2DataVariableOffset(p2Pos);
		trans.translate = proto.GetFloat2DataVariableOffset(translate);
		trans.point = proto.GetFloat2DataVariableOffset(point);
		proto.AddTransformData((uint8_t*)&trans,sizeof(trans));

		
	};
	proto.transformCallbacks.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> Transform7Node::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<TransformResult>>("Pin 1 Parent",ImFlow::ConnectionFilter::SameType(),TransformResult()));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Pin 1 Position",ImFlow::ConnectionFilter::SameType(),Float2Variable(.5f,.5f)));

	info.push_back(std::make_shared<ImFlow::InPinProto<TransformResult>>("Pin 2 Parent",ImFlow::ConnectionFilter::SameType(),TransformResult()));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Pin 2 Position",ImFlow::ConnectionFilter::SameType(),Float2Variable(.5f,.5f)));


	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Translate",ImFlow::ConnectionFilter::SameType(),Float2Variable(0.f,0.f)));

	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Point 1",ImFlow::ConnectionFilter::SameType(),Float2Variable(1.f,1.f)));



	info.push_back(std::make_shared<ImFlow::InPinProto<TransformSize>>("Size",ImFlow::ConnectionFilter::SameType(),TransformSize(_mm_set1_ps(64.f))));

	info.push_back(std::make_shared<ImFlow::OutPinProto<TransformResult>>("Out"));
	return info;
}

Transform8Node::Transform8Node(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getIn<TransformResult>("Pin 1 Parent")->setEmptyVal(render.transformResults[2]);
	getIn<TransformResult>("Pin 2 Parent")->setEmptyVal(render.transformResults[2]);
	uint64_t outHash = randomInt64();
	getOut<TransformResult>("Out")->behaviour([this,outHash]() {
		TransformResult res;
		res.hash = outHash;
		const TransformSize& size = getInVal<TransformSize>("Size");
		const TransformResult& p1parent = getInVal<TransformResult>("Pin 1 Parent");
		const Float2Variable& p1Pos = getInVal<Float2Variable>("Pin 1 Position");
		const TransformResult& p2parent = getInVal<TransformResult>("Pin 2 Parent");
		const Float2Variable& p2Pos = getInVal<Float2Variable>("Pin 2 Position");
		const Float2Variable& translate = getInVal<Float2Variable>("Translate");
		const Float2Variable& point = getInVal<Float2Variable>("Point 1");

		__m128 v5 = _mm_set_ps(render.elementHeight,render.elementWidth,render.elementHeight,render.elementWidth);

		__m128 v8 = _mm_set_ps(render.elementHeightRatio,render.elementWidthRatio,render.elementHeightRatio,render.elementWidthRatio);


		__m128 v14 = _mm_shuffle_ps(
			_mm_set_ps(0,0,0,p2Pos.value.x),
			_mm_set_ps(0,0,0,p2Pos.value.y),
			0);

		__m128 v16 = _mm_shuffle_ps(
			_mm_set_ps(0,0,0,translate.value.x),
			_mm_set_ps(0,0,0,translate.value.y),
			0);
		__m128 v17 = _mm_shuffle_ps(
			_mm_set_ps(0,0,0,p1Pos.value.x),
			_mm_set_ps(0,0,0,p1Pos.value.y),
			0);




		__m128 v20 = _mm_mul_ps(v17, p1parent.directionVector);

		__m128 v21 = _mm_mul_ps(v14, p2parent.directionVector);
		__m128 v22 = _mm_mul_ps(
			_mm_shuffle_ps(
				_mm_set_ps(0,0,0,point.value.x),
				_mm_set_ps(0,0,0,point.value.y),
				0),
			size.size);
		__m128 pin1 = _mm_add_ps(
			_mm_add_ps(_mm_shuffle_ps(v20,v20, 78), v20),
			p1parent.position);
		__m128 pin2 = _mm_add_ps(
			_mm_add_ps((__m128)_mm_shuffle_ps(v21,v21, 78), (__m128)v21),
			p2parent.position);
		__m128 v24 = _mm_mul_ps(v22, v22);
		__m128 v25 = _mm_max_ps(_mm_set1_ps(1.1754944e-38f), _mm_add_ps(_mm_shuffle_ps(v24, v24, 27), (__m128)v24));
		__m128 v28 = _mm_mul_ps(
			_mm_mul_ps(
				_mm_mul_ps(
					_mm_sub_ps(pin2, pin1),
					v5),
				v22),
			NRReciprocal(v25));
		__m128 v29 = _mm_mul_ps(
			_mm_add_ps(_mm_xor_ps((__m128)_mm_shuffle_ps(v28,v28, 27), _mm_set_ps(0,-0.0,-0.0,0)), (__m128)v28),
			_mm_mul_ps(size.size, v8));
		__m128 v30 = _mm_mul_ps(v16, v29);
		res.directionVector = v29;
		res.position = _mm_sub_ps(pin1, _mm_add_ps((__m128)_mm_shuffle_ps(v30,v30, 78), (__m128)v30));


		res.inputSize = size.size;

		render.transformResults.push_back(res);
		return res;
	});
}

Transform8Node::Transform8Node(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):Transform8Node(rend,style){}

void Transform8Node::draw() {
	ImGui::PushItemWidth(90);

	ImGui::PopItemWidth();
}

void Transform8Node::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void Transform8Node::Export(RuiExportPrototype& proto) {
	const TransformSize& size = getInVal<TransformSize>("Size");
	const TransformResult& p1parent = getInVal<TransformResult>("Pin 1 Parent");
	const Float2Variable& p1Pos = getInVal<Float2Variable>("Pin 1 Position");
	const TransformResult& p2parent = getInVal<TransformResult>("Pin 2 Parent");
	const Float2Variable& p2Pos = getInVal<Float2Variable>("Pin 2 Position");
	const Float2Variable& translate = getInVal<Float2Variable>("Translate");
	const Float2Variable& point = getInVal<Float2Variable>("Point 1");
	const TransformResult& out = getOut<TransformResult>("Out")->val();
	proto.AddDataVariable(p1Pos);
	proto.AddDataVariable(p2Pos);
	proto.AddDataVariable(translate);
	proto.AddDataVariable(point);
	ExportElement<uint64_t> ele;
	ele.identifier = out.hash;
	ele.dependencys = {p1parent.hash,p2parent.hash};
	ele.callback = [p1parent,p1Pos,p2parent,p2Pos,translate,point,out,size](RuiExportPrototype& proto) {
		struct Transform8FileStruct {
			uint8_t type = 8;
			uint8_t count = 1;
			uint16_t p1parent;
			Float2Offsets p1pos;
			uint16_t p2parent;
			Float2Offsets p2pos;
			Float2Offsets translate;
			Float2Offsets point;
		};
		uint16_t transId = (uint16_t)proto.transformIndices.size();
		proto.transformIndices.emplace(out.hash,transId);
		ExportElement<std::string> ele;
		ele.identifier = Variable::UniqueName();
		ele.dependencys = { size.name };
		ele.callback = [transId, size](RuiExportPrototype& proto) {
			proto.codeLines.push_back(std::format("transformSize[{}] = {};",transId,size.GetFormattedName(proto)));
		};
		proto.codeElements.push_back(ele);
		Transform8FileStruct trans{};
		trans.p1parent = proto.transformIndices[p1parent.hash];
		trans.p1pos = proto.GetFloat2DataVariableOffset(p1Pos);
		trans.p2parent = proto.transformIndices[p2parent.hash];
		trans.p2pos = proto.GetFloat2DataVariableOffset(p2Pos);
		trans.translate = proto.GetFloat2DataVariableOffset(translate);
		trans.point = proto.GetFloat2DataVariableOffset(point);
		proto.AddTransformData((uint8_t*)&trans,sizeof(trans));


	};
	proto.transformCallbacks.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> Transform8Node::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<TransformResult>>("Pin 1 Parent",ImFlow::ConnectionFilter::SameType(),TransformResult()));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Pin 1 Position",ImFlow::ConnectionFilter::SameType(),Float2Variable(.5f,.5f)));

	info.push_back(std::make_shared<ImFlow::InPinProto<TransformResult>>("Pin 2 Parent",ImFlow::ConnectionFilter::SameType(),TransformResult()));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Pin 2 Position",ImFlow::ConnectionFilter::SameType(),Float2Variable(.5f,.5f)));


	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Translate",ImFlow::ConnectionFilter::SameType(),Float2Variable(0.f,0.f)));

	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Point 1",ImFlow::ConnectionFilter::SameType(),Float2Variable(1.f,1.f)));



	info.push_back(std::make_shared<ImFlow::InPinProto<TransformSize>>("Size",ImFlow::ConnectionFilter::SameType(),TransformSize(_mm_set1_ps(64.f))));

	info.push_back(std::make_shared<ImFlow::OutPinProto<TransformResult>>("Out"));
	return info;
}

Transform9Node::Transform9Node(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getIn<TransformResult>("Pin 1 Parent")->setEmptyVal(render.transformResults[2]);
	getIn<TransformResult>("Pin 2 Parent")->setEmptyVal(render.transformResults[2]);
	uint64_t outHash = randomInt64();
	getOut<TransformResult>("Out")->behaviour([this,outHash]() {
		TransformResult res;
		res.hash = outHash;
		const TransformSize& size = getInVal<TransformSize>("Size");
		const TransformResult& p1parent = getInVal<TransformResult>("Pin 1 Parent");
		const Float2Variable& p1Pos = getInVal<Float2Variable>("Pin 1 Position");
		const TransformResult& p2parent = getInVal<TransformResult>("Pin 2 Parent");
		const Float2Variable& p2Pos = getInVal<Float2Variable>("Pin 2 Position");
		const Float2Variable& translate = getInVal<Float2Variable>("Translate");
		const Float2Variable& point = getInVal<Float2Variable>("Point 1");

		__m128 v5 = _mm_set_ps(render.elementHeight,render.elementWidth,render.elementHeight,render.elementWidth);

		__m128 v6 = _mm_set_ps(render.elementHeightRatio,render.elementWidthRatio,render.elementHeightRatio,render.elementWidthRatio);


		__m128 v15 = _mm_shuffle_ps(
			_mm_set_ps(0,0,0,translate.value.x),
			_mm_set_ps(0,0,0,translate.value.y),
			0);
		__m128 v16 = _mm_shuffle_ps(
			_mm_set_ps(0,0,0,p2Pos.value.x),
			_mm_set_ps(0,0,0,p2Pos.value.y),
			0);



		__m128 v18 = _mm_mul_ps(
			_mm_shuffle_ps(
				_mm_set_ps(0,0,0,p1Pos.value.x),
				_mm_set_ps(0,0,0,p1Pos.value.y), 
				0),
			p1parent.directionVector);
		__m128 v19 = _mm_mul_ps(
			_mm_shuffle_ps(
				_mm_set_ps(0,0,0,point.value.x),
				_mm_set_ps(0,0,0,point.value.y),
				0),
			size.size);

		__m128 v21 = _mm_mul_ps(v16, p2parent.directionVector);
		__m128 v22 = _mm_mul_ps(v19, v19);
		__m128 v23 = _mm_add_ps(
			_mm_add_ps(_mm_shuffle_ps(v18,v18, 78), v18),
			p1parent.position);
		__m128 v24 = _mm_max_ps(_mm_set1_ps(1.1754944e-38f), _mm_add_ps(_mm_shuffle_ps(v22, v22, 27), (__m128)v22));
		__m128 v27 = _mm_mul_ps(
			_mm_sub_ps(
				_mm_add_ps(
					_mm_add_ps(_mm_shuffle_ps(v21,v21, 78), v21),
					p2parent.position),
				v23),
			v5);
		__m128 v28 = _mm_mul_ps(v19, v27);
		__m128 v29 = _mm_mul_ps(v27, v27);
		__m128 v30 = NRReciprocal(v24);
		__m128 v31 = _mm_max_ps(_mm_set1_ps(1.1754944e-38f), _mm_mul_ps(_mm_add_ps(_mm_shuffle_ps(v29, v29, _MM_SHUFFLE(0,1,2,3)), (__m128)v29), v30));
		__m128 v32 = _mm_rsqrt_ps(v31);
		__m128 v33 = _mm_mul_ps(
			_mm_mul_ps(
				_mm_add_ps(
					_mm_mul_ps(
						_mm_xor_ps(
							_mm_mul_ps(
								_mm_sub_ps(_mm_set1_ps(1.5f), _mm_mul_ps(_mm_mul_ps(_mm_mul_ps(v31, _mm_set1_ps(0.5f)), v32), v32)),
								v32),
							_mm_set_ps(0,-0.0,-0.0,0)),
						_mm_shuffle_ps(v28,v28, _MM_SHUFFLE(0,1,2,3))),
					v28),
				v30),
			_mm_mul_ps(size.size, v6));
		__m128 v34 = _mm_mul_ps(v15, v33);
		res.directionVector = v33;
		res.position = _mm_sub_ps(v23, _mm_add_ps((__m128)_mm_shuffle_ps(v34,v34, _MM_SHUFFLE(1,0,3,2)), (__m128)v34));
		res.inputSize = size.size;

		render.transformResults.push_back(res);
		return res;
	});
}

Transform9Node::Transform9Node(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):Transform9Node(rend,style){}

void Transform9Node::draw() {
	ImGui::PushItemWidth(90);

	ImGui::PopItemWidth();
}

void Transform9Node::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void Transform9Node::Export(RuiExportPrototype& proto) {
	const TransformSize& size = getInVal<TransformSize>("Size");
	const TransformResult& p1parent = getInVal<TransformResult>("Pin 1 Parent");
	const Float2Variable& p1Pos = getInVal<Float2Variable>("Pin 1 Position");
	const TransformResult& p2parent = getInVal<TransformResult>("Pin 2 Parent");
	const Float2Variable& p2Pos = getInVal<Float2Variable>("Pin 2 Position");
	const Float2Variable& translate = getInVal<Float2Variable>("Translate");
	const Float2Variable& point = getInVal<Float2Variable>("Point 1");
	const TransformResult& out = getOut<TransformResult>("Out")->val();
	proto.AddDataVariable(p1Pos);
	proto.AddDataVariable(p2Pos);
	proto.AddDataVariable(translate);
	proto.AddDataVariable(point);
	ExportElement<uint64_t> ele;
	ele.identifier = out.hash;
	ele.dependencys = {p1parent.hash,p2parent.hash};
	ele.callback = [p1parent,p1Pos,p2parent,p2Pos,translate,point,out,size](RuiExportPrototype& proto) {
		struct Transform9FileStruct {
			uint8_t type = 7;
			uint8_t count = 1;
			uint16_t p1parent;
			Float2Offsets p1pos;
			uint16_t p2parent;
			Float2Offsets p2pos;
			Float2Offsets translate;
			Float2Offsets point;
		};
		uint16_t transId = (uint16_t)proto.transformIndices.size();
		proto.transformIndices.emplace(out.hash,transId);
		ExportElement<std::string> ele;
		ele.identifier = Variable::UniqueName();
		ele.dependencys = { size.name };
		ele.callback = [transId, size](RuiExportPrototype& proto) {
			proto.codeLines.push_back(std::format("transformSize[{}] = {};",transId,size.GetFormattedName(proto)));
		};
		proto.codeElements.push_back(ele);
		Transform9FileStruct trans{};
		trans.p1parent = proto.transformIndices[p1parent.hash];
		trans.p1pos = proto.GetFloat2DataVariableOffset(p1Pos);
		trans.p2parent = proto.transformIndices[p2parent.hash];
		trans.p2pos = proto.GetFloat2DataVariableOffset(p2Pos);
		trans.translate = proto.GetFloat2DataVariableOffset(translate);
		trans.point = proto.GetFloat2DataVariableOffset(point);
		proto.AddTransformData((uint8_t*)&trans,sizeof(trans));


	};
	proto.transformCallbacks.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> Transform9Node::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<TransformResult>>("Pin 1 Parent",ImFlow::ConnectionFilter::SameType(),TransformResult()));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Pin 1 Position",ImFlow::ConnectionFilter::SameType(),Float2Variable(.5f,.5f)));

	info.push_back(std::make_shared<ImFlow::InPinProto<TransformResult>>("Pin 2 Parent",ImFlow::ConnectionFilter::SameType(),TransformResult()));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Pin 2 Position",ImFlow::ConnectionFilter::SameType(),Float2Variable(.5f,.5f)));


	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Translate",ImFlow::ConnectionFilter::SameType(),Float2Variable(0.f,0.f)));

	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Point 1",ImFlow::ConnectionFilter::SameType(),Float2Variable(1.f,1.f)));



	info.push_back(std::make_shared<ImFlow::InPinProto<TransformSize>>("Size",ImFlow::ConnectionFilter::SameType(),TransformSize(_mm_set1_ps(64.f))));

	info.push_back(std::make_shared<ImFlow::OutPinProto<TransformResult>>("Out"));
	return info;
}

Transform10Node::Transform10Node(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getIn<TransformResult>("Pin 1 Parent")->setEmptyVal(render.transformResults[2]);
	getIn<TransformResult>("Pin 2 Parent")->setEmptyVal(render.transformResults[2]);
	getIn<TransformResult>("Pin 3 Parent")->setEmptyVal(render.transformResults[2]);
	uint64_t outHash = randomInt64();
	getOut<TransformResult>("Out")->behaviour([this,outHash]() {
		TransformResult res;
		res.hash = outHash;
		const TransformSize& size = getInVal<TransformSize>("Size");
		const TransformResult& p1parent = getInVal<TransformResult>("Pin 1 Parent");
		const Float2Variable& p1Pos = getInVal<Float2Variable>("Pin 1 Position");
		const TransformResult& p2parent = getInVal<TransformResult>("Pin 2 Parent");
		const Float2Variable& p2Pos = getInVal<Float2Variable>("Pin 2 Position");
		const TransformResult& p3parent = getInVal<TransformResult>("Pin 3 Parent");
		const Float2Variable& p3Pos = getInVal<Float2Variable>("Pin 3 Position");
		const Float2Variable& translate = getInVal<Float2Variable>("Translate");
		const Float2Variable& point1 = getInVal<Float2Variable>("Point 1");
		const Float2Variable& point2 = getInVal<Float2Variable>("Point 2");
		__m128 v5 = _mm_set_ps(0, 0, 0, translate.value.x);
		__m128 v6 = _mm_set_ps(0, 0, 0, translate.value.y);




		__m128 v9 = _mm_mul_ps(
			_mm_shuffle_ps(
				_mm_set_ps(0, 0, 0, p1Pos.value.x),
				_mm_set_ps(0, 0, 0, p1Pos.value.y),
				0),
			p1parent.directionVector);
		__m128 v10 = _mm_mul_ps(
			_mm_shuffle_ps(
				_mm_set_ps(0, 0, 0, p2Pos.value.x),
				_mm_set_ps(0, 0, 0, p2Pos.value.y),
				0),
			p2parent.directionVector);
		__m128 v11 = _mm_add_ps(
			_mm_add_ps((__m128)_mm_shuffle_ps(v9,v9, 78), (__m128)v9),
			p1parent.position);


		__m128 v14 = _mm_mul_ps(
			_mm_shuffle_ps(
				_mm_set_ps(0, 0, 0, p3Pos.value.x),
				_mm_set_ps(0, 0, 0, p3Pos.value.y),
				0),
			p3parent.directionVector);
		__m128 v15 = _mm_add_ps(
			_mm_mul_ps(
				_mm_sub_ps(
					_mm_add_ps(
						_mm_add_ps((__m128)_mm_shuffle_ps(v14,v14, 78), (__m128)v14),
						p3parent.position),
					v11),
				_mm_shuffle_ps(
					_mm_set_ps(0, 0, 0, point1.value.y),
					_mm_set_ps(0, 0, 0, point1.value.x),
					0)),
			_mm_mul_ps(
				_mm_sub_ps(
					_mm_add_ps(
						_mm_add_ps((__m128)_mm_shuffle_ps(v10,v10, 78), (__m128)v10),
						p2parent.position),
					v11),
				_mm_shuffle_ps(
					_mm_set_ps(0, 0, 0, point2.value.y),
					_mm_set_ps(0, 0, 0, point2.value.x),
					0)));
		__m128 v16 = _mm_mul_ps(_mm_shuffle_ps(v5, v6, 0), v15);
		res.directionVector = v15;
		res.position = _mm_sub_ps(v11, _mm_add_ps((__m128)_mm_shuffle_ps(v16,v16, 78), (__m128)v16));

		res.inputSize = size.size;

		render.transformResults.push_back(res);
		return res;
	});
}

Transform10Node::Transform10Node(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):Transform10Node(rend,style){}

void Transform10Node::draw() {
	ImGui::PushItemWidth(90);

	ImGui::PopItemWidth();
}

void Transform10Node::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void Transform10Node::Export(RuiExportPrototype& proto) {
	const TransformSize& size = getInVal<TransformSize>("Size");
	const TransformResult& p1parent = getInVal<TransformResult>("Pin 1 Parent");
	const Float2Variable& p1Pos = getInVal<Float2Variable>("Pin 1 Position");
	const TransformResult& p2parent = getInVal<TransformResult>("Pin 2 Parent");
	const Float2Variable& p2Pos = getInVal<Float2Variable>("Pin 2 Position");
	const TransformResult& p3parent = getInVal<TransformResult>("Pin 3 Parent");
	const Float2Variable& p3Pos = getInVal<Float2Variable>("Pin 3 Position");
	const Float2Variable& translate = getInVal<Float2Variable>("Translate");
	const Float2Variable& point1 = getInVal<Float2Variable>("Point 1");
	const Float2Variable& point2 = getInVal<Float2Variable>("Point 2");
	const TransformResult& out = getOut<TransformResult>("Out")->val();
	proto.AddDataVariable(p1Pos);
	proto.AddDataVariable(p2Pos);
	proto.AddDataVariable(p3Pos);
	proto.AddDataVariable(translate);
	proto.AddDataVariable(point1);
	proto.AddDataVariable(point2);
	ExportElement<uint64_t> ele;
	ele.identifier = out.hash;
	ele.dependencys = {p1parent.hash,p2parent.hash};
	ele.callback = [p1parent,p1Pos,p2parent,p2Pos,p3parent,p3Pos,translate,point1,point2,out,size](RuiExportPrototype& proto) {
		struct Transform10FileStruct {
			uint8_t type = 7;
			uint8_t count = 1;
			uint16_t p1parent;
			Float2Offsets p1pos;
			uint16_t p2parent;
			Float2Offsets p2pos;
			uint16_t p3parent;
			Float2Offsets p3pos;
			Float2Offsets translate;
			Float2Offsets point1;
			Float2Offsets point2;
		};
		uint16_t transId = (uint16_t)proto.transformIndices.size();
		proto.transformIndices.emplace(out.hash,transId);
		ExportElement<std::string> ele;
		ele.identifier = Variable::UniqueName();
		ele.dependencys = { size.name };
		ele.callback = [transId, size](RuiExportPrototype& proto) {
			proto.codeLines.push_back(std::format("transformSize[{}] = {};",transId,size.GetFormattedName(proto)));
		};
		proto.codeElements.push_back(ele);

		Transform10FileStruct trans{};
		trans.p1parent = proto.transformIndices[p1parent.hash];
		trans.p1pos = proto.GetFloat2DataVariableOffset(p1Pos);
		trans.p2parent = proto.transformIndices[p2parent.hash];
		trans.p2pos = proto.GetFloat2DataVariableOffset(p2Pos);
		trans.p3parent = proto.transformIndices[p3parent.hash];
		trans.p3pos = proto.GetFloat2DataVariableOffset(p3Pos);
		trans.translate = proto.GetFloat2DataVariableOffset(translate);
		trans.point1 = proto.GetFloat2DataVariableOffset(point1);
		trans.point2 = proto.GetFloat2DataVariableOffset(point2);
		proto.AddTransformData((uint8_t*)&trans,sizeof(trans));


	};
	proto.transformCallbacks.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> Transform10Node::GetPinInfo() {
	std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	info.push_back(std::make_shared<ImFlow::InPinProto<TransformResult>>("Pin 1 Parent",ImFlow::ConnectionFilter::SameType(),TransformResult()));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Pin 1 Position",ImFlow::ConnectionFilter::SameType(),Float2Variable(.5f,.5f)));

	info.push_back(std::make_shared<ImFlow::InPinProto<TransformResult>>("Pin 2 Parent",ImFlow::ConnectionFilter::SameType(),TransformResult()));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Pin 2 Position",ImFlow::ConnectionFilter::SameType(),Float2Variable(.5f,.5f)));

	info.push_back(std::make_shared<ImFlow::InPinProto<TransformResult>>("Pin 3 Parent",ImFlow::ConnectionFilter::SameType(),TransformResult()));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Pin 3 Position",ImFlow::ConnectionFilter::SameType(),Float2Variable(.5f,.5f)));

	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Translate",ImFlow::ConnectionFilter::SameType(),Float2Variable(0.f,0.f)));

	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Point 1",ImFlow::ConnectionFilter::SameType(),Float2Variable(0.f,1.f)));

	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Point 2",ImFlow::ConnectionFilter::SameType(),Float2Variable(1.f,1.f)));

	info.push_back(std::make_shared<ImFlow::InPinProto<TransformSize>>("Size",ImFlow::ConnectionFilter::SameType(),TransformSize()));

	info.push_back(std::make_shared<ImFlow::OutPinProto<TransformResult>>("Out"));
	return info;
}

Transform11Node::Transform11Node(RenderInstance& rend,ImFlow::StyleManager& style):RuiBaseNode(name,category,GetPinInfo(),rend,style) {

	getIn<TransformResult>("Parent")->setEmptyVal(render.transformResults[2]);
	uint64_t outHash = randomInt64();
	getOut<TransformResult>("Out")->behaviour([this,outHash]() {
		TransformResult res;
		res.hash = outHash;
		const TransformSize& size = getInVal<TransformSize>("Size");
		const TransformResult& parent = getInVal<TransformResult>("Parent");
		const Float2Variable& center = getInVal<Float2Variable>("Rotation Origin");
		const FloatVariable& rot = getInVal<FloatVariable>("Rotation");

		__m128 v2 = _mm_set_ps(0.f,0.f,0.f, 2147483600.0f);
		__m128 v3 = _mm_xor_ps(
			_mm_mul_ps(
				(__m128)_mm_set_ps(render.elementWidth,render.elementHeight,render.elementWidth,render.elementHeight),
				(__m128)_mm_set_ps(render.elementHeightRatio,render.elementWidthRatio,render.elementHeightRatio,render.elementWidthRatio)),
			_mm_set_ps(0.f,-0.0f,0.f,-0.0f));



		res = parent;
		res.inputSize = size.size;

		__m128 v7 = _mm_set_ps(0,0,0,rot.value);

		__m128 v9 = _mm_set_ps(0,0,0,center.value.y);


		
		__m128 a = _mm_and_ps(
			_mm_sub_ps(v7, _mm_cvtepi32_ps(_mm_cvttps_epi32(v7))),
			_mm_cmple_ss(_mm_cvtpd_ps(_mm_and_pd(_mm_cvtps_pd(v7), _mm_castsi128_pd(_mm_set1_epi64x(0x7FFFFFFFFFFFFFFF)))), v2));
		__m128 v12 = _mm_mul_ps(
			_mm_add_ps(
				_mm_shuffle_ps(
					a,a,
					80),
				_mm_set_ps(0,0.25,0,-0.75)),
			_mm_set1_ps(4));
		__m128i v13 = _mm_cvtps_epi32(v12);
		__m128 v14 = _mm_sub_ps(v12, _mm_cvtepi32_ps(v13));
		__m128 v15 = _mm_castsi128_ps(_mm_cmpeq_epi32(_mm_and_si128(_mm_set1_epi32(1), v13), _mm_setzero_si128()));
		__m128 v16 = _mm_mul_ps(v14, v14);
		__m128 v17 = _mm_xor_ps(
			_mm_or_ps(
				_mm_andnot_ps(
					v15,
					_mm_sub_ps(
						_mm_set1_ps(1),
						_mm_sub_ps(
							v16,
							_mm_mul_ps(
								_mm_add_ps(
									_mm_mul_ps(
										_mm_add_ps(
											_mm_mul_ps(
												_mm_add_ps(_mm_mul_ps(_mm_set1_ps(0.00091595226f), v16), _mm_set1_ps(-0.020863468f)),
												v16),
											_mm_set1_ps(0.25366944f)),
										v16),
									_mm_set1_ps(-0.23370054f)),
								v16)))),
				_mm_and_ps(
					_mm_add_ps(
						_mm_mul_ps(
							_mm_add_ps(
								_mm_mul_ps(
									_mm_add_ps(
										_mm_mul_ps(
											_mm_add_ps(_mm_mul_ps(_mm_set1_ps(-0.004600245f), v16), _mm_set1_ps(0.079678982f)),
											v16),
										_mm_set1_ps(-0.64596325f)),
									v16),
								_mm_set1_ps(0.57079631f)),
							v14),
						v14),
					v15)),
			_mm_castsi128_ps(_mm_slli_epi32(_mm_and_si128(_mm_set1_epi32(2), v13), 30u)));
		__m128 v18 = _mm_add_ps(
			_mm_mul_ps(
				_mm_mul_ps(_mm_shuffle_ps(v17,v17, 85), _mm_shuffle_ps(res.directionVector,res.directionVector, 177)),
				v3),
			_mm_mul_ps(_mm_shuffle_ps(v17,v17, 0), res.directionVector));
		__m128 v19 = _mm_mul_ps(
			_mm_shuffle_ps(_mm_set_ps(0,0,0,center.value.x), v9, 0),
			_mm_sub_ps(res.directionVector, v18));
		res.position = _mm_add_ps(_mm_add_ps(_mm_shuffle_ps(v19,v19, 78), v19), res.position);
		res.directionVector = v18;

		return res;
	});
}

Transform11Node::Transform11Node(RenderInstance& rend,ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj):Transform11Node(rend,style){}

void Transform11Node::draw() {
	ImGui::PushItemWidth(90);

	ImGui::PopItemWidth();
}

void Transform11Node::Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
	obj.AddMember("Name",name,allocator);
	obj.AddMember("Category",category,allocator);
	RuiBaseNode::Serialize(obj,allocator);
}

void Transform11Node::Export(RuiExportPrototype& proto) {
	const TransformSize& size = getInVal<TransformSize>("Size");
	const TransformResult& parent = getInVal<TransformResult>("Parent");
	const Float2Variable& center = getInVal<Float2Variable>("Rotation Origin");
	const FloatVariable& rot = getInVal<FloatVariable>("Rotation");
	const TransformResult& out = getOut<TransformResult>("Out")->val();
	proto.AddDataVariable(center);
	proto.AddDataVariable(rot);

	ExportElement<uint64_t> ele;
	ele.identifier = out.hash;
	ele.dependencys = {parent.hash,};
	ele.callback = [parent,center,rot,out,size](RuiExportPrototype& proto) {
		struct Transform11FileStruct {
			uint8_t type = 1;
			uint8_t count = 1;
			uint16_t parent;
			uint8_t type_ = 11;
			uint8_t count_ = 1;
			uint16_t parent_;
			uint16_t rotation;
			Float2Offsets center;
		};
		uint16_t transId = (uint16_t)proto.transformIndices.size();
		proto.transformIndices.emplace(out.hash,transId);
		ExportElement<std::string> ele;
		ele.identifier = Variable::UniqueName();
		ele.dependencys = { size.name };
		ele.callback = [transId, size](RuiExportPrototype& proto) {
			proto.codeLines.push_back(std::format("transformSize[{}] = {};",transId,size.GetFormattedName(proto)));
		};
		proto.codeElements.push_back(ele);
		Transform11FileStruct trans{};
		trans.parent = proto.transformIndices[parent.hash];
		trans.parent_ = transId;
		trans.rotation = proto.GetFloatDataVariableOffset(rot);
		trans.center = proto.GetFloat2DataVariableOffset(center);
		proto.AddTransformData((uint8_t*)&trans,sizeof(trans));


	};
	proto.transformCallbacks.push_back(ele);
}

std::vector<std::shared_ptr<ImFlow::PinProto>> Transform11Node::GetPinInfo(){
	static std::vector<std::shared_ptr<ImFlow::PinProto>> info;
	if(info.size()) return info;
	info.push_back(std::make_shared<ImFlow::InPinProto<TransformSize>>("Size",ImFlow::ConnectionFilter::SameType(), TransformSize(_mm_set1_ps(64.f))));
	info.push_back(std::make_shared<ImFlow::InPinProto<TransformResult>>("Parent",ImFlow::ConnectionFilter::SameType(), TransformResult()));
	info.push_back(std::make_shared<ImFlow::InPinProto<Float2Variable>>("Rotation Origin",ImFlow::ConnectionFilter::SameType(), Float2Variable(.5f,.5f))); 
	info.push_back(std::make_shared<ImFlow::InPinProto<FloatVariable>>("Rotation",ImFlow::ConnectionFilter::SameType(), FloatVariable(0.f)));
	info.push_back(std::make_shared<ImFlow::OutPinProto<TransformResult>>("Out"));
	return info;
	
}

void AddTransformNodes(NodeEditor& editor) {
	//editor.AddNodeType<Transform0Node>();
	editor.AddNodeType<Transform1Node>();
	editor.AddNodeType<Transform2Node>();
	editor.AddNodeType<Transform3Node>();
	editor.AddNodeType<Transform4Node>();
	editor.AddNodeType<Transform5Node>();
	editor.AddNodeType<Transform6Node>();
	editor.AddNodeType<Transform7Node>();
	editor.AddNodeType<Transform8Node>();
	editor.AddNodeType<Transform9Node>();
	editor.AddNodeType<Transform10Node>();
	//editor.AddNodeType<Transform11Node>();
	//editor.AddNodeType<Transform12Node>();
	//editor.AddNodeType<Transform13Node>();
}