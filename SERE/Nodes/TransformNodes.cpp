#include "TransformNodes.h"
#include "imgui/imgui_stdlib.h"
#include "IntrinUtil.h"

__m128 xmmword_12A146C0 = _mm_castsi128_ps(_mm_set_epi32(0xFFFFFFFF,0,0,0xFFFFFFFF));

Transform0Node::Transform0Node(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addIN<TransformSize>("Size", { _mm_set1_ps(64) }, ImFlow::ConnectionFilter::SameType(), styles.transformSize);
	ImFlow::BaseNode::addOUT<TransformResult>("Out", styles.transformResult)->behaviour([this]() {
		TransformResult res; 
		res.index = proto.transformResults.size();
		res.directionVector = _mm_and_ps(getInVal<TransformSize>("Size").size, (__m128)xmmword_12A146C0);
		res.position = _mm_setzero_ps();
		proto.transformResults.push_back(res);
		return res;
	});
}

void Transform0Node::draw() {
	ImGui::PushItemWidth(90);

	ImGui::PopItemWidth();
}

std::vector<PinInfo> Transform0Node::GetPinInfo() {
	return {};
}

Transform1Node::Transform1Node(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addIN<TransformResult>("Source",proto.transformResults[2], ImFlow::ConnectionFilter::SameType(),styles.transformResult);
	ImFlow::BaseNode::addIN<TransformSize>("Size", { _mm_set1_ps(64) }, ImFlow::ConnectionFilter::SameType(), styles.transformSize);
	ImFlow::BaseNode::addOUT<TransformResult>("Out", styles.transformResult)->behaviour([this]() {
		TransformResult res;
		const TransformResult& parent = getInVal<TransformResult>("Source");
		const TransformSize& size = getInVal<TransformSize>("Size");

		res.index = proto.transformResults.size();
		res.position = parent.position;
		res.directionVector = parent.directionVector;
		res.inputSize = size.size;
		proto.transformResults.push_back(res);
		return res;
	});;
}

void Transform1Node::draw() {
	ImGui::PushItemWidth(90);

	ImGui::PopItemWidth();
}

std::vector<PinInfo> Transform1Node::GetPinInfo() {
	return {};
}

Transform2Node::Transform2Node(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addIN<Float2Variable>("Val_0",Float2Variable(.5f,.5f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<Float2Variable>("Val_3",Float2Variable(1.f,1.f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<TransformSize>("Size", { _mm_set1_ps(64) }, ImFlow::ConnectionFilter::SameType(),styles.transformSize);
	ImFlow::BaseNode::addIN<TransformResult>("Parent",proto.transformResults[2], ImFlow::ConnectionFilter::SameType(),styles.transformResult);
	ImFlow::BaseNode::addOUT<TransformResult>("Out", styles.transformResult)->behaviour([this]() {
		TransformResult res;

		const Float2Variable& val_0 = getInVal<Float2Variable>("Val_0");
		const Float2Variable& val_3 = getInVal<Float2Variable>("Val_3");
		const TransformSize& size = getInVal<TransformSize>("Size");
		const TransformResult& parent = getInVal<TransformResult>("Parent");

		__m128 elementSizeRatio = _mm_set_ps(0,0,proto.elementHeightRatio,proto.elementWidthRatio);
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
		res.index = proto.transformResults.size();
		proto.transformResults.push_back(res);
		return res;
	});
}

void Transform2Node::draw() {
	ImGui::PushItemWidth(90);

	ImGui::PopItemWidth();
}

std::vector<PinInfo> Transform2Node::GetPinInfo() {
	return {};
}

Transform3Node::Transform3Node(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addIN<Float2Variable>("Val_0",Float2Variable(.5f,.5f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<Float2Variable>("Val_3",Float2Variable(1.f,1.f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<TransformSize>("Size", { _mm_set1_ps(64) }, ImFlow::ConnectionFilter::SameType(),styles.transformSize);
	ImFlow::BaseNode::addIN<TransformResult>("Parent",proto.transformResults[2], ImFlow::ConnectionFilter::SameType(),styles.transformResult);
	ImFlow::BaseNode::addOUT<TransformResult>("Out",styles.transformResult)->behaviour([this]() {
		TransformResult res;
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
		res.index = proto.transformResults.size();
		proto.transformResults.push_back(res);
		return res;
	});
}

void Transform3Node::draw() {
	ImGui::PushItemWidth(90);

	ImGui::PopItemWidth();
}

std::vector<PinInfo> Transform3Node::GetPinInfo() {
	return {};
}

Transform4Node::Transform4Node(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addIN<Float2Variable>("Val_0",Float2Variable(.5f,.5f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<Float2Variable>("Val_3",Float2Variable(1.f,1.f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<TransformSize>("Size", { _mm_set1_ps(64) }, ImFlow::ConnectionFilter::SameType(),styles.transformSize);
	ImFlow::BaseNode::addIN<TransformResult>("Parent",proto.transformResults[2], ImFlow::ConnectionFilter::SameType(),styles.transformResult);
	ImFlow::BaseNode::addOUT<TransformResult>("Out",styles.transformResult)->behaviour([this]() {
		TransformResult res;
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
		res.index = proto.transformResults.size();
		proto.transformResults.push_back(res);
		return res;
	});
}

void Transform4Node::draw() {
	ImGui::PushItemWidth(90);

	ImGui::PopItemWidth();
}

std::vector<PinInfo> Transform4Node::GetPinInfo() {
	return {};
}

void sub_100520(RenderInstance& proto,__m128* a2,__m128* a3) {
	__m128 m128_10 = *(__m128 *)&proto.drawInfo.ruiUnk3[0].screenWidth;
	__m128 m128_20 = *(__m128 *)&proto.drawInfo.ruiUnk3[0].float_10;

	__m128 v6 = _mm_mul_ps(m128_20, m128_20);
	__m128 v7 = _mm_mul_ps(m128_10, m128_10);

	__m128 v9 = _mm_add_ps(_mm_unpacklo_ps(v7, v6), _mm_unpackhi_ps(v7, v6));
	__m128 v10 = _mm_sqrt_ps(_mm_add_ps((__m128)_mm_shuffle_ps(v9,v9, _MM_SHUFFLE(1,0,3,2)), v9));
	*a2 = _mm_shuffle_ps(v10,v10, _MM_SHUFFLE(0,0,0,0));
	*a3 = _mm_shuffle_ps(v10,v10, _MM_SHUFFLE(1,1,1,1));
}

Transform5Node::Transform5Node(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addIN<Float2Variable>("Val_0",Float2Variable(.5f,.5f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<Float2Variable>("Val_3",Float2Variable(1.f,1.f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<TransformSize>("Size", { _mm_set1_ps(64) }, ImFlow::ConnectionFilter::SameType(),styles.transformSize);
	ImFlow::BaseNode::addIN<TransformResult>("Parent",proto.transformResults[2], ImFlow::ConnectionFilter::SameType(),styles.transformResult);
	ImFlow::BaseNode::addOUT<TransformResult>("Out",styles.transformResult)->behaviour([this]() {
		TransformResult res;
		const Float2Variable& val_0 = getInVal<Float2Variable>("Val_0");
		const Float2Variable& val_3 = getInVal<Float2Variable>("Val_3");
		const TransformSize& size = getInVal<TransformSize>("Size");
		const TransformResult& parent = getInVal<TransformResult>("Parent");


		__m128 v25;
		__m128 v26;
		sub_100520(proto, &v25, &v26);

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

		res.index = proto.transformResults.size();
		proto.transformResults.push_back(res);
		return res;
	});
}

void Transform5Node::draw() {
	ImGui::PushItemWidth(90);

	ImGui::PopItemWidth();
}

std::vector<PinInfo> Transform5Node::GetPinInfo() {
	return {};
}

Transform6Node::Transform6Node(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));
	ImFlow::BaseNode::addIN<Float2Variable>("Val_0",Float2Variable(.5f,.5f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<Float2Variable>("Val_3",Float2Variable(1.f,1.f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<TransformSize>("Size", { _mm_set1_ps(64) }, ImFlow::ConnectionFilter::SameType(),styles.transformSize);
	ImFlow::BaseNode::addIN<TransformResult>("Parent",proto.transformResults[2], ImFlow::ConnectionFilter::SameType(),styles.transformResult);
	ImFlow::BaseNode::addOUT<TransformResult>("Out",styles.transformResult)->behaviour([this]() {
		TransformResult res;
		const Float2Variable& val_0 = getInVal<Float2Variable>("Val_0");
		const Float2Variable& val_3 = getInVal<Float2Variable>("Val_3");
		const TransformSize& size = getInVal<TransformSize>("Size");
		const TransformSize& parentSize = getInVal<TransformSize>("ParentSize");
		const TransformResult& parent = getInVal<TransformResult>("Parent");

		__m128 v25;
		__m128 v26;
		sub_100520(proto, &v25, &v26);

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
		res.index = proto.transformResults.size();
		proto.transformResults.push_back(res);
		return res;
	});
}

void Transform6Node::draw() {
	ImGui::PushItemWidth(90);

	ImGui::PopItemWidth();
}

std::vector<PinInfo> Transform6Node::GetPinInfo() {
	return {};
}

Transform7Node::Transform7Node(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));

	ImFlow::BaseNode::addIN<TransformResult>("Pin 1 Parent",proto.transformResults[2], ImFlow::ConnectionFilter::SameType(),styles.transformResult);
	ImFlow::BaseNode::addIN<Float2Variable>("Pin 1 Position",Float2Variable(.5f,.5f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);

	ImFlow::BaseNode::addIN<TransformResult>("Pin 2 Parent",proto.transformResults[2], ImFlow::ConnectionFilter::SameType(),styles.transformResult);
	ImFlow::BaseNode::addIN<Float2Variable>("Pin 2 Position",Float2Variable(.5f,.5f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);

	ImFlow::BaseNode::addIN<Float2Variable>("Translate",Float2Variable(0.f,0.f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);

	ImFlow::BaseNode::addIN<Float2Variable>("Point 1",Float2Variable(1.f,1.f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<TransformSize>("Size", { _mm_set1_ps(64) }, ImFlow::ConnectionFilter::SameType(),styles.transformSize);
	ImFlow::BaseNode::addOUT<TransformResult>("Out",styles.transformResult)->behaviour([this]() {
		TransformResult res;
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
		res.index = proto.transformResults.size();
		proto.transformResults.push_back(res);
		return res;
	});
}

void Transform7Node::draw() {
	ImGui::PushItemWidth(90);

	ImGui::PopItemWidth();
}

std::vector<PinInfo> Transform7Node::GetPinInfo() {
	return {};
}

Transform8Node::Transform8Node(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));

	ImFlow::BaseNode::addIN<TransformResult>("Pin 1 Parent",proto.transformResults[2], ImFlow::ConnectionFilter::SameType(),styles.transformResult);
	ImFlow::BaseNode::addIN<Float2Variable>("Pin 1 Position",Float2Variable(.5f,.5f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);

	ImFlow::BaseNode::addIN<TransformResult>("Pin 2 Parent",proto.transformResults[2], ImFlow::ConnectionFilter::SameType(),styles.transformResult);
	ImFlow::BaseNode::addIN<Float2Variable>("Pin 2 Position",Float2Variable(.5f,.5f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);

	ImFlow::BaseNode::addIN<Float2Variable>("Translate",Float2Variable(0.f,0.f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);

	ImFlow::BaseNode::addIN<Float2Variable>("Point 1",Float2Variable(1.f,1.f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<TransformSize>("Size", { _mm_set1_ps(64) }, ImFlow::ConnectionFilter::SameType(),styles.transformSize);
	ImFlow::BaseNode::addOUT<TransformResult>("Out",styles.transformResult)->behaviour([this]() {
		TransformResult res;
		const TransformSize& size = getInVal<TransformSize>("Size");
		const TransformResult& p1parent = getInVal<TransformResult>("Pin 1 Parent");
		const Float2Variable& p1Pos = getInVal<Float2Variable>("Pin 1 Position");
		const TransformResult& p2parent = getInVal<TransformResult>("Pin 2 Parent");
		const Float2Variable& p2Pos = getInVal<Float2Variable>("Pin 2 Position");
		const Float2Variable& translate = getInVal<Float2Variable>("Translate");
		const Float2Variable& point = getInVal<Float2Variable>("Point 1");

		__m128 v5 = _mm_set_ps(proto.elementHeight,proto.elementWidth,proto.elementHeight,proto.elementWidth);

		__m128 v8 = _mm_set_ps(proto.elementHeightRatio,proto.elementWidthRatio,proto.elementHeightRatio,proto.elementWidthRatio);


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
		res.index = proto.transformResults.size();
		proto.transformResults.push_back(res);
		return res;
	});
}

void Transform8Node::draw() {
	ImGui::PushItemWidth(90);

	ImGui::PopItemWidth();
}

std::vector<PinInfo> Transform8Node::GetPinInfo() {
	return {};
}

Transform9Node::Transform9Node(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));

	ImFlow::BaseNode::addIN<TransformResult>("Pin 1 Parent",proto.transformResults[2], ImFlow::ConnectionFilter::SameType(),styles.transformResult);
	ImFlow::BaseNode::addIN<Float2Variable>("Pin 1 Position",Float2Variable(.5f,.5f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);

	ImFlow::BaseNode::addIN<TransformResult>("Pin 2 Parent",proto.transformResults[2], ImFlow::ConnectionFilter::SameType(),styles.transformResult);
	ImFlow::BaseNode::addIN<Float2Variable>("Pin 2 Position",Float2Variable(.5f,.5f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);

	ImFlow::BaseNode::addIN<Float2Variable>("Translate",Float2Variable(0.f,0.f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);

	ImFlow::BaseNode::addIN<Float2Variable>("Point 1",Float2Variable(1.f,1.f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<TransformSize>("Size", { _mm_set1_ps(64) }, ImFlow::ConnectionFilter::SameType(), styles.transformSize);
	ImFlow::BaseNode::addOUT<TransformResult>("Out",styles.transformResult)->behaviour([this]() {
		TransformResult res;
		const TransformSize& size = getInVal<TransformSize>("Size");
		const TransformResult& p1parent = getInVal<TransformResult>("Pin 1 Parent");
		const Float2Variable& p1Pos = getInVal<Float2Variable>("Pin 1 Position");
		const TransformResult& p2parent = getInVal<TransformResult>("Pin 2 Parent");
		const Float2Variable& p2Pos = getInVal<Float2Variable>("Pin 2 Position");
		const Float2Variable& translate = getInVal<Float2Variable>("Translate");
		const Float2Variable& point = getInVal<Float2Variable>("Point 1");

		__m128 v5 = _mm_set_ps(proto.elementHeight,proto.elementWidth,proto.elementHeight,proto.elementWidth);

		__m128 v6 = _mm_set_ps(proto.elementHeightRatio,proto.elementWidthRatio,proto.elementHeightRatio,proto.elementWidthRatio);


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
		res.index = proto.transformResults.size();
		proto.transformResults.push_back(res);
		return res;
	});
}

void Transform9Node::draw() {
	ImGui::PushItemWidth(90);

	ImGui::PopItemWidth();
}

std::vector<PinInfo> Transform9Node::GetPinInfo() {
	return {};
}

Transform10Node::Transform10Node(RenderInstance& prot,NodeStyles& styles):proto(prot) {

	setTitle(name);
	setStyle(styles.GetNodeStyle(category));

	ImFlow::BaseNode::addIN<TransformResult>("Pin 1 Parent",proto.transformResults[2], ImFlow::ConnectionFilter::SameType(),styles.transformResult);
	ImFlow::BaseNode::addIN<Float2Variable>("Pin 1 Position",Float2Variable(.5f,.5f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);

	ImFlow::BaseNode::addIN<TransformResult>("Pin 2 Parent",proto.transformResults[2], ImFlow::ConnectionFilter::SameType(),styles.transformResult);
	ImFlow::BaseNode::addIN<Float2Variable>("Pin 2 Position",Float2Variable(.5f,.5f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);

	ImFlow::BaseNode::addIN<TransformResult>("Pin 3 Parent",proto.transformResults[2], ImFlow::ConnectionFilter::SameType(),styles.transformResult);
	ImFlow::BaseNode::addIN<Float2Variable>("Pin 3 Position",Float2Variable(.5f,.5f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);

	ImFlow::BaseNode::addIN<Float2Variable>("Translate",Float2Variable(0.f,0.f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);

	ImFlow::BaseNode::addIN<Float2Variable>("Point 1",Float2Variable(0.f,1.f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);

	ImFlow::BaseNode::addIN<Float2Variable>("Point 2",Float2Variable(1.f,1.f), ImFlow::ConnectionFilter::SameType(),styles.float2Variable);

	ImFlow::BaseNode::addIN<TransformSize>("Size", { _mm_set1_ps(64) }, ImFlow::ConnectionFilter::SameType(),styles.transformSize);
	ImFlow::BaseNode::addOUT<TransformResult>("Out",styles.transformResult)->behaviour([this]() {
		TransformResult res;
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
		res.index = proto.transformResults.size();
		proto.transformResults.push_back(res);
		return res;
	});
}

void Transform10Node::draw() {
	ImGui::PushItemWidth(90);

	ImGui::PopItemWidth();
}

std::vector<PinInfo> Transform10Node::GetPinInfo() {
	return {};
}

void AddTransformNodes(NodeEditor& editor) {
	editor.AddNodeType<Transform0Node>();
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