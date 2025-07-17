#include "RenderJobNodes.h"



struct AssetInputData {
	ColorVariable mainColor;
	ColorVariable maskColor;
	ColorVariable tertColor;
	AssetVariable mainAsset;
	AssetVariable maskAsset;
	Float2Variable mins;
	Float2Variable maxs;
	Float2Variable texMins;
	Float2Variable texMaxs;
	FloatVariable blend;
	FloatVariable premul;
	Float2Variable maskCenter;
	Float2Variable maskTranslate;
	Float2Variable maskSize;
	FloatVariable maskRotation;
	int flags;
	TransformResult transform;

};

AssetRenderNode::AssetRenderNode(RenderInstance& prot, NodeStyles& styles):proto(prot) {
	setTitle("Render Asset");
	setStyle(styles.renderNode);
	ImFlow::BaseNode::addIN<ColorVariable>("Main Color", {true,false,{1.f,1.f,1.f,1.f}}, ImFlow::ConnectionFilter::SameType(), styles.colorVariable);
	ImFlow::BaseNode::addIN<ColorVariable>("Mask Color",{true,false,{1.f,1.f,1.f,1.f}},ImFlow::ConnectionFilter::SameType(), styles.colorVariable);
	ImFlow::BaseNode::addIN<ColorVariable>("Tertiary Color",{true,false,{1.f,1.f,1.f,1.f}},ImFlow::ConnectionFilter::SameType(), styles.colorVariable);
	ImFlow::BaseNode::addIN<AssetVariable>("Main Asset",{true,false,"white"},ImFlow::ConnectionFilter::SameType(),styles.assetVariable);
	ImFlow::BaseNode::addIN<AssetVariable>("Mask Asset",{true,false,""},ImFlow::ConnectionFilter::SameType(),styles.assetVariable);
	ImFlow::BaseNode::addIN<Float2Variable>("Mins",{true,false,{0.f,0.f}},ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<Float2Variable>("Maxs",{true,false,{1.f,1.f}},ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<Float2Variable>("Texture Mins",{true,false,{0.f,0.f}},ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<Float2Variable>("Texture Maxs",{true,false,{1.f,1.f}},ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<FloatVariable>("Blend",{true,false,1.0f},ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<FloatVariable>("Premul",{true,false,0.0f},ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<Float2Variable>("Mask Center",{true,false,{0.5f,0.5f}},ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<Float2Variable>("Mask Translate",{true,false,{0.f,0.f}},ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<Float2Variable>("Mask Size",{true,false,{1.f,1.f}},ImFlow::ConnectionFilter::SameType(),styles.float2Variable);
	ImFlow::BaseNode::addIN<FloatVariable>("Mask Rotation",{true,false,0.f},ImFlow::ConnectionFilter::SameType(),styles.floatVariable);
	ImFlow::BaseNode::addIN<TransformResult>("Transform", proto.transformResults[2],ImFlow::ConnectionFilter::SameType(),styles.transformResult);
}


bool Render_Asset(RenderInstance& proto,AssetInputData& input) {
	__int16 unknown8Index; // bp
	//testStruct* a5; // r9

	__m128 v9; // xmm2
	__m128 v10; // xmm6
	int a6; // ebx
	__m128 v12; // xmm1
	__m128 v13; // xmm11
	__int64 v14; // rcx
	__int16 assetIndex2; // si
	__int64 atlasIndex; // r11
	__int16 assetIndex; // r15
	__int16 v18; // r10
	Asset_t a7; // rdi
	__int64 v20; // rcx
	__m128 v22; // xmm8
	__m128 v23; // xmm9
	__m128 v26; // xmm5
	__m128 v27; // xmm13
	__m128 v28; // xmm4

	__m128 v30; // xmm13
	__m128 v31; // xmm4
	__m128 v32; // xmm4
	__m128 v33; // xmm1
	__m128 v34; // xmm2

	__m128 v36; // xmm3
	__m128 v37; // xmm3
	__m128 v39; // xmm15
	__m128 v40; // xmm4
	__m128 v41; // xmm12
	__m128 v42; // xmm4
	__m128 v43; // xmm12
	__m128 v45; // xmm9
	__m128 v46; // xmm10
	__m128 v47; // xmm9
	__m128i v48; // xmm8
	__m128i v49; // xmm7
	__m128 v50; // xmm6
	__m128 v51; // xmm2
	__m128 v52; // xmm9
	__m128 v53; // xmm10
	__m128 v54; // xmm4
	__m128 v55; // xmm5
	__m128 v56; // xmm4
	__m128 v57; // xmm11
	__m128 v58; // xmm6
	__m128 v59; // xmm11
	__m128 v60; // xmm3
	__m128 v61; // xmm1
	__m128 v62; // xmm2
	__m128 v63; // xmm14
	__m128 v64; // xmm0
	//ruiGlobals* globals; // rcx
	__m128 a11; // [rsp+60h] [rbp-1A8h] BYREF
	__m128 a10; // [rsp+70h] [rbp-198h] BYREF
	__m128 v68; // [rsp+80h] [rbp-188h]
	__m128 a9; // [rsp+90h] [rbp-178h] BYREF

	__m128 a8; // [rsp+B0h] [rbp-158h] BYREF

	//ruiUnknownStruct_1 a4a; // [rsp+D0h] [rbp-138h] BYREF


	//StyleDescriptor_t styleDesc = rui.styleDescriptors[hdr.styleDescriptorIndex];
	RenderQuad quad;


	if(input.mainColor.value[3] <= 0.0)
		return false;// is alpha less than 0

	TransformResult& a5 = input.transform;
	v9 = _mm_sub_ps(
		_mm_mul_ps(
			_mm_shuffle_ps(a5.directionVector, a5.directionVector, _MM_SHUFFLE(3, 3, 3, 3)),
			_mm_shuffle_ps(a5.directionVector, a5.directionVector, _MM_SHUFFLE(0, 0, 0, 0))),
		_mm_mul_ps(
			_mm_shuffle_ps(a5.directionVector, a5.directionVector, _MM_SHUFFLE(2, 2, 2, 2)),
			_mm_shuffle_ps(a5.directionVector, a5.directionVector, _MM_SHUFFLE(1, 1, 1, 1))));
	if (_mm_movemask_ps(_mm_cmpeq_ps(_mm_setzero_ps(), v9)))
		return 0;


	v10 = _mm_div_ps(_mm_xor_ps(_mm_shuffle_ps(a5.directionVector, a5.directionVector, _MM_SHUFFLE(0, 2, 1, 3)), _mm_set_ps(0.0, -0.0, -0.0, 0.0)), v9);
	a6 = _mm_movemask_ps(v9) & 2;
	v12 = _mm_mul_ps(_mm_xor_ps(v10, _mm_set1_ps(-0.0)), (__m128)_mm_shuffle_ps(a5.position, a5.position, 216));
	v13 = _mm_add_ps((__m128)_mm_shuffle_ps(v12, v12, 78), (__m128)v12);
	v14 = loadAsset(input.mainAsset.value.c_str());
	if (v14 != -1)
	{
		assetIndex2 = -1;
		atlasIndex = imageAssetMap[v14].atlasIndex;//g_AssetIndexData[v14].atlasIndex;
		assetIndex = imageAssetMap[v14].imageIndex;//g_AssetIndexData[v14].assetIndex;
		v18 = input.flags | (unsigned __int8)imageAssetMap[v14].flags;//(unsigned __int8)g_AssetIndexData[v14].byte7;
		a7 = imageAssetMap[v14];
		v20 = loadAsset(input.maskAsset.value.c_str());
		if (v20 != -1)
		{
			if (atlasIndex != imageAssetMap[v20].atlasIndex)//g_AssetIndexData[v20].atlasIndex)
				return 0;
			assetIndex2 = imageAssetMap[v20].imageIndex;//g_AssetIndexData[v20].assetIndex;
			v18 |= 4 * imageAssetMap[v20].flags;//g_AssetIndexData[v20].byte7;
		}

		//v22 = (__m128i)_mm_unpacklo_ps(
		//    (__m128)*(unsigned int *)&a2->dataValues[a3->uint16_8],
		//    (__m128)*(unsigned int *)&a2->dataValues[a3->uint16_A]);
		v22 = _mm_set_ps(0, 0, input.mins.value[1], input.mins.value[0]);
		//v23 = (__m128i)_mm_unpacklo_ps(
		//    (__m128)*(unsigned int *)&a2->dataValues[a3->uint16_C],
		//    (__m128)*(unsigned int *)&a2->dataValues[a3->uint16_E]);
		v23 = _mm_set_ps(0, 0, input.maxs.value[1], input.maxs.value[0]);
		v26 = _mm_movelh_ps(_mm_xor_ps(_mm_set1_ps(-0.0), (__m128)v22), (__m128)v23);
		a9 = v26;
		//v27 = _mm_unpacklo_ps(v24, (__m128)*(unsigned int *)&a2->dataValues[uint16_12]);
		v27 = _mm_set_ps(0, 0, input.texMins.value[1], input.texMins.value[0]);
		//v28 = (__m128)*(unsigned int *)&a2->dataValues[a3->uint16_14];
		v30 = _mm_movelh_ps(v27, v27);
		a10 = v30;
		//v31 = _mm_unpacklo_ps(v28, (__m128)*(unsigned int *)&a2->dataValues[uint16_16]);
		v31 = _mm_set_ps(0, 0, input.texMaxs.value[1], input.texMaxs.value[0]);

		v32 = _mm_movelh_ps(v31, v31);
		v33 = xmm_12A4E830a[((__int64)v18 >> 4) & 3];//*(__m128 *)(16 * (((__int64)v18 >> 4) & 3) + 0x12A4E830);
		v34 = _mm_sub_ps(v32, v30);

		v36 = imageAtlases[atlasIndex].offsets[assetIndex].m128_0;

		a11 = v34;
		v37 = _mm_div_ps(
			_mm_sub_ps(v36, _mm_xor_ps(_mm_and_ps(_mm_min_ps(v30, v32), v33), _mm_set_ps(0, 0, -0.0, -0.0))),
			_mm_or_ps(_mm_and_ps(_mm_andnot_ps(_mm_set1_ps(-0.0), v34), v33), _mm_andnot_ps(v33, _mm_set1_ps(1))));
		if (!_mm_movemask_ps(_mm_cmplt_ps(v37, _mm_set1_ps(-3.4028235e38f))))
		{
			__m128 k = _mm_min_ps(v26, v37);
			a8 = _mm_xor_ps(k, _mm_set_ps(0, 0, -0.0, -0.0));
			if (!_mm_movemask_ps(_mm_cmple_ps((__m128)_mm_shuffle_ps(a8, a8, 238), (__m128)_mm_shuffle_ps(a8, a8, 68))))
			{
				//pixelBufferElementCount = a4->pixelBufferElementCount;
				quad.assetIndex = assetIndex;
				quad.assetIndex2 = assetIndex2;
				quad.flags = v18;
				quad.styleDescriptorIndex = proto.styleDescriptor.size();// + pixelBufferElementCount;
				StyleDescriptorShader_t desc;
				
				desc.color0_red = input.mainColor.value[0];
				desc.color0_green = input.mainColor.value[1];
				desc.color0_blue = input.mainColor.value[2];
				desc.color0_alpha = input.mainColor.value[3];
				desc.color1_red = input.maskColor.value[0];
				desc.color1_green = input.maskColor.value[1];
				desc.color1_blue = input.maskColor.value[2];
				desc.color1_alpha = input.maskColor.value[3];
				desc.color2_red = input.tertColor.value[0];
				desc.color2_green = input.tertColor.value[1];
				desc.color2_blue = input.tertColor.value[2];
				desc.color2_alpha = input.tertColor.value[3];
				desc.float_30 = input.blend.value;
				desc.float_34 = input.premul.value;
				proto.styleDescriptor.push_back(desc);
				v39 = _mm_add_ps(_mm_mul_ps(v13, v34), v30);
				v40 = _mm_shuffle_ps(imageAtlases[atlasIndex].offsets[assetIndex].m128_10, imageAtlases[atlasIndex].offsets[assetIndex].m128_10, _MM_SHUFFLE(3, 2, 3, 2));
				v68 = _mm_mul_ps(v10, v34);
				v41 = _mm_mul_ps(v40, v39);
				v42 = _mm_mul_ps(v40, v68);
				v43 = _mm_add_ps(v41, _mm_shuffle_ps(imageAtlases[atlasIndex].offsets[assetIndex].m128_10, _mm_setzero_ps(), _MM_SHUFFLE(1, 0, 1, 0)));

				if (assetIndex2 == -1)
				{
					v60 = (__m128)_mm_shuffle_ps(v22, v22, 68);
					v61 = _mm_max_ps(_mm_set1_ps(1.1754944e-38f), _mm_sub_ps((__m128)_mm_shuffle_ps(v23, v23, 68), v60));
					v62 = _mm_rcp_ps(v61);
					v63 = _mm_sub_ps(_mm_set1_ps(1), _mm_mul_ps(v62, v61));
					v64 = _mm_add_ps(_mm_mul_ps(_mm_add_ps(_mm_mul_ps(v63, v63), v63), v62), v62);
					v58 = _mm_mul_ps(v10, v64);
					v59 = _mm_mul_ps(_mm_sub_ps(v13, v60), v64);
				}
				else
				{
					v45 = _mm_set_ps(0, 0, 0, input.maskRotation.value);
					//v46 = _mm_unpacklo_ps(
					//    (__m128)*(unsigned int *)&a2->dataValues[a3->uint16_18],
					//    (__m128)*(unsigned int *)&a2->dataValues[a3->uint16_1A]);
					v46 = _mm_set_ps(0, 0, input.maskCenter.value[1],input.maskCenter.value[0]);
					v47 = _mm_mul_ps(
						_mm_add_ps(
							_mm_xor_ps(_mm_shuffle_ps(v45, v45, 0), _mm_set_ps(0, -0.0, 0, 0)),
							_mm_set_ps(0.25, 0, 0, 0.25)),
						_mm_set1_ps(4));
					v48 = _mm_cvtps_epi32(v47);
					v49 = _mm_cmpeq_epi32(
						_mm_and_si128(_mm_set1_epi32(1), v48),
						_mm_setzero_si128());
					//v50 = _mm_unpacklo_ps(
					//    (__m128)*(unsigned int *)&a2->dataValues[a3->uint16_22],
					//    (__m128)*(unsigned int *)&a2->dataValues[a3->uint16_24]);
					v50 = _mm_set_ps(0, 0, input.maskSize.value[1],input.maskSize.value[0]);

					v51 = _mm_shuffle_ps(imageAtlases[atlasIndex].offsets[assetIndex].m128_10, imageAtlases[atlasIndex].offsets[assetIndex2].m128_10, _MM_SHUFFLE(3, 2, 3, 2));
					v52 = _mm_sub_ps(v47, _mm_cvtepi32_ps(v48));
					v53 = _mm_add_ps(_mm_mul_ps(_mm_movelh_ps(v46, v46), v34), v30);
					v54 = _mm_mul_ps(v52, v52);
					v55 = _mm_mul_ps(
						_mm_xor_ps(
							_mm_or_ps(
								_mm_andnot_ps(
									_mm_castsi128_ps(v49),
									_mm_sub_ps(
										_mm_set1_ps(1.f),
										_mm_sub_ps(
											v54,
											_mm_mul_ps(
												_mm_add_ps(
													_mm_mul_ps(
														_mm_add_ps(
															_mm_mul_ps(
																_mm_add_ps(
																	_mm_mul_ps(_mm_set1_ps(0.00091595226f), v54),
																	_mm_set1_ps(-0.020863468f)),
																v54),
															_mm_set1_ps(0.25366944f)),
														v54),
													_mm_set1_ps(-0.23370054f)),
												v54)))),
								_mm_and_ps(
									_mm_add_ps(
										_mm_mul_ps(
											_mm_add_ps(
												_mm_mul_ps(
													_mm_add_ps(
														_mm_mul_ps(
															_mm_add_ps(
																_mm_mul_ps(_mm_set1_ps(-0.004600245f), v54),
																_mm_set1_ps(0.079678982f)),
															v54),
														_mm_set1_ps(-0.64596325f)),
													v54),
												_mm_set1_ps(0.57079631f)),
											v52),
										v52),
									_mm_castsi128_ps(v49))),
							_mm_castsi128_ps(_mm_slli_epi32(
								_mm_and_si128(_mm_set1_epi32(2), v48),
								0x1Eu))),
						_mm_movelh_ps(v50, v50));
					__m128 a = _mm_sub_ps(v39, v53);
					v56 = _mm_mul_ps(_mm_shuffle_ps(a, a, 216), v55);
					v57 = _mm_mul_ps(
						_mm_add_ps(
							_mm_add_ps(
								_mm_set_ps(0, 0, input.maskTranslate.value[1],input.maskTranslate.value[0]),

								v53),
							_mm_add_ps(_mm_shuffle_ps(v56, v56, 78), v56)),
						v51);
					v58 = _mm_mul_ps(
						_mm_add_ps(
							_mm_mul_ps(_mm_shuffle_ps(v55, v55, 78), _mm_shuffle_ps(v68, v68, 165)),
							_mm_mul_ps(_mm_shuffle_ps(v68, v68, 240), v55)),
						v51);


					v59 = _mm_add_ps(v57, _mm_shuffle_ps(imageAtlases[atlasIndex].offsets[assetIndex2].m128_10, _mm_setzero_ps(), _MM_SHUFFLE(1, 0, 1, 0)));
				}
				//globals = a1->globals;
				quad.xUvVector = _mm_movelh_ps(v42, v58);
				quad.yUvVector = _mm_movehl_ps(v58, v42);
				quad.xUvBase = _mm_movelh_ps(v43, v59);
				quad.m128_30 = _mm_setzero_ps();
				quad.m128_40 = _mm_setzero_ps();
				quad.m128_50 = _mm_setzero_ps();
				//__autohookfuncsub_F9B80(globals, a2, a4, &a4a, a5, a6, a7, &a8, &a9, &a10, &a11);
				__m128 ruiSize = _mm_set_ps(proto.elementHeightRatio,proto.elementWidthRatio,proto.elementHeight,proto.elementWidth);
				proto.sub_F9B80_rev(ruiSize, quad, &a5, a6, a7, &a8, &a9, &a10, &a11);
				return 1;
			}
		}
	}
}

void AssetRenderNode::draw() {
	AssetInputData input;
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