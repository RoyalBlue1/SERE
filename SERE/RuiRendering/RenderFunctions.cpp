#include "RenderFunctions.h"
#include "RenderManager.h"
#include "Util.h"

#undef max
#undef min

#define TAU 6.2831855f

bool Render_Asset(RenderInstance& proto, AssetInputData input) {

	__m128 v9; // xmm2
	__m128 v10; // xmm6
	int a6; // ebx
	__m128 v12; // xmm1
	__m128 v13; // xmm11
	uint32_t v14; // rcx
	__int16 assetIndex2; // si
	__int64 atlasIndex; // r11
	__int16 assetIndex; // r15
	__int16 flags; // r10
	Asset_t a7; // rdi
	uint32_t v20; // rcx
	__m128 mins; // xmm8
	__m128 maxs; // xmm9
	__m128 v26; // xmm5
	__m128 texMins; // xmm13
	__m128 texMaxs; // xmm4
	__m128 v33; // xmm1
	__m128 texSize; // xmm2

	__m128 v36; // xmm3
	__m128 v37; // xmm3
	__m128 v39; // xmm15
	__m128 v40; // xmm4
	__m128 v41; // xmm12
	__m128 v42; // xmm4
	__m128 v43; // xmm12
	__m128 maskRotation; // xmm9
	__m128 maskCenter; // xmm10
	__m128 v47; // xmm9
	__m128i v48; // xmm8
	__m128i v49; // xmm7
	__m128 maskSize; // xmm6
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
	__m128 v64; // xmm0

	__m128 v68; // [rsp+80h] [rbp-188h]
	__m128 a9; // [rsp+90h] [rbp-178h] BYREF

	__m128 a8; // [rsp+B0h] [rbp-158h] BYREF

	//ruiUnknownStruct_1 a4a; // [rsp+D0h] [rbp-138h] BYREF


	//StyleDescriptor_t styleDesc = rui.styleDescriptors[hdr.styleDescriptorIndex];
	RenderQuad quad;


	if(input.mainColor.value.alpha <= 0.0)
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
		return false;


	v10 = _mm_div_ps(_mm_xor_ps(_mm_shuffle_ps(a5.directionVector, a5.directionVector, _MM_SHUFFLE(0, 2, 1, 3)), _mm_set_ps(0.0, -0.0, -0.0, 0.0)), v9);
	a6 = _mm_movemask_ps(v9) & 2;
	v12 = _mm_mul_ps(_mm_xor_ps(v10, _mm_set1_ps(-0.0)), (__m128)_mm_shuffle_ps(a5.position, a5.position, _MM_SHUFFLE(3,1,2,0)));
	v13 = _mm_add_ps((__m128)_mm_shuffle_ps(v12, v12, _MM_SHUFFLE(1,0,3,2)), (__m128)v12);
	v14 = input.mainAsset.hash;
	if(v14== INVALID_ASSET)
		return false;
	assetIndex2 = -1;
	atlasIndex = imageAssetMap[v14].atlasIndex;//g_AssetIndexData[v14].atlasIndex;
	assetIndex = imageAssetMap[v14].imageIndex;//g_AssetIndexData[v14].assetIndex;
	flags = input.flags | (unsigned __int8)imageAssetMap[v14].flags;//(unsigned __int8)g_AssetIndexData[v14].byte7;
	a7 = imageAssetMap[v14];
	v20 = input.maskAsset.hash;
	if (v20 != INVALID_ASSET)
	{
		if (atlasIndex != imageAssetMap[v20].atlasIndex)//g_AssetIndexData[v20].atlasIndex)
			return 0;
		assetIndex2 = imageAssetMap[v20].imageIndex;//g_AssetIndexData[v20].assetIndex;
		flags |= 4 * (uint8_t)imageAssetMap[v20].flags;//g_AssetIndexData[v20].byte7;
	}


	mins = _mm_set_ps(0, 0, input.mins.value.y, input.mins.value.x);

	maxs = _mm_set_ps(0, 0, input.maxs.value.y, input.maxs.value.x);
	v26 = _mm_movelh_ps(_mm_xor_ps(_mm_set1_ps(-0.0), (__m128)mins), (__m128)maxs);
	a9 = v26;

	texMins = _mm_set_ps(input.texMins.value.y, input.texMins.value.x, input.texMins.value.y, input.texMins.value.x);

	texMaxs = _mm_set_ps(input.texMaxs.value.y,input.texMaxs.value.x,input.texMaxs.value.y,input.texMaxs.value.x);
	v33 = xmm_12A4E830a[((__int64)flags >> 4) & 3];
	texSize = _mm_sub_ps(texMaxs, texMins);

	v36 = imageAtlases[atlasIndex].offsets[assetIndex].m128_0;

	v37 = _mm_div_ps(
		_mm_sub_ps(v36, _mm_xor_ps(_mm_and_ps(_mm_min_ps(texMins, texMaxs), v33), _mm_set_ps(0, 0, -0.0, -0.0))),
		_mm_or_ps(_mm_and_ps(_mm_andnot_ps(_mm_set1_ps(-0.0), texSize), v33), _mm_andnot_ps(v33, _mm_set1_ps(1))));
	if(_mm_movemask_ps(_mm_cmplt_ps(v37, _mm_set1_ps(-3.4028235e38f))))
		return false;


	__m128 k = _mm_min_ps(v26, v37);
	a8 = _mm_xor_ps(k, _mm_set_ps(0, 0, -0.0, -0.0));
	if(_mm_movemask_ps(_mm_cmple_ps((__m128)_mm_shuffle_ps(a8, a8, _MM_SHUFFLE(3,2,3,2)), (__m128)_mm_shuffle_ps(a8, a8, _MM_SHUFFLE(1,0,1,0)))))
		return false;

	//pixelBufferElementCount = a4->pixelBufferElementCount;
	quad.assetIndex = assetIndex;
	quad.assetIndex2 = assetIndex2;
	quad.flags = flags;
	quad.styleDescriptorIndex = proto.styleDescriptor.size();// + pixelBufferElementCount;
	StyleDescriptorShader_t desc{};

	desc.color0 = input.mainColor.value;
	desc.color1 = input.maskColor.value;
	desc.color2 = input.tertColor.value;
	desc.blend = input.blend.value;
	desc.premul = input.premul.value;
	proto.styleDescriptor.push_back(desc);
	v39 = _mm_add_ps(_mm_mul_ps(v13, texSize), texMins);
	v40 = _mm_shuffle_ps(imageAtlases[atlasIndex].offsets[assetIndex].m128_10, imageAtlases[atlasIndex].offsets[assetIndex].m128_10, _MM_SHUFFLE(3, 2, 3, 2));
	v68 = _mm_mul_ps(v10, texSize);
	v41 = _mm_mul_ps(v40, v39);
	v42 = _mm_mul_ps(v40, v68);
	v43 = _mm_add_ps(v41, _mm_shuffle_ps(imageAtlases[atlasIndex].offsets[assetIndex].m128_10, _mm_setzero_ps(), _MM_SHUFFLE(1, 0, 1, 0)));

	if (assetIndex2 == -1)
	{
		v60 = (__m128)_mm_shuffle_ps(mins, mins, 68);
		v61 = _mm_max_ps(_mm_set1_ps(1.1754944e-38f), _mm_sub_ps((__m128)_mm_shuffle_ps(maxs, maxs, 68), v60));
		v64 = NRReciprocal(v61);
		v58 = _mm_mul_ps(v10, v64);
		v59 = _mm_mul_ps(_mm_sub_ps(v13, v60), v64);
	}
	else
	{
		maskRotation = _mm_set1_ps( input.maskRotation.value);

		maskCenter = _mm_set_ps(0, 0, input.maskCenter.value.y,input.maskCenter.value.x);
		v47 = _mm_mul_ps(
			_mm_add_ps(
				_mm_xor_ps(maskRotation, _mm_set_ps(0, -0.0, 0, 0)),
				_mm_set_ps(0.25, 0, 0, 0.25)),
			_mm_set1_ps(4));
		v48 = _mm_cvtps_epi32(v47);
		v49 = _mm_cmpeq_epi32(
			_mm_and_si128(_mm_set1_epi32(1), v48),
			_mm_setzero_si128());

		maskSize = _mm_set_ps(0, 0, input.maskSize.value.y,input.maskSize.value.x);

		v51 = _mm_shuffle_ps(imageAtlases[atlasIndex].offsets[assetIndex].m128_10, imageAtlases[atlasIndex].offsets[assetIndex2].m128_10, _MM_SHUFFLE(3, 2, 3, 2));
		v52 = _mm_sub_ps(v47, _mm_cvtepi32_ps(v48));
		v53 = _mm_add_ps(_mm_mul_ps(_mm_movelh_ps(maskCenter, maskCenter), texSize), texMins);
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
			_mm_movelh_ps(maskSize, maskSize));
		__m128 a = _mm_sub_ps(v39, v53);
		v56 = _mm_mul_ps(_mm_shuffle_ps(a, a, 216), v55);
		v57 = _mm_mul_ps(
			_mm_add_ps(
				_mm_add_ps(
					_mm_set_ps(0, 0, input.maskTranslate.value.y,input.maskTranslate.value.x),

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

	quad.xUvVector = _mm_movelh_ps(v42, v58);
	quad.yUvVector = _mm_movehl_ps(v58, v42);
	quad.UvBase = _mm_movelh_ps(v43, v59);
	quad.m128_30 = _mm_setzero_ps();
	quad.m128_40 = _mm_setzero_ps();
	quad.m128_50 = _mm_setzero_ps();
	__m128 ruiSize = _mm_set_ps(proto.elementHeightRatio,proto.elementWidthRatio,proto.elementHeight,proto.elementWidth);
	proto.sub_F9B80_rev(ruiSize, quad, a5, a6, a7, a8, a9, texMins, texSize);
	return 1;



}

struct fontRenderingStruct
{
	float float_0;
	short word_4;
	short word_6;
	Glyph_t *glyph;
};

uint64_t readUnicodeCharacter(const char** a1)
{
	char v2; // r9
	const char* v3; // rdx
	uint64_t result; // rax
	char v5; // cl
	const char* v6; // rdx
	char v7; // r11
	const char* v8; // rdx
	unsigned int v9; // r8d
	char v10; // cl

	v2 = **a1;
	v3 = *a1 + 1;
	if (v2 >= 0)
	{
		*a1 = v3;
		return (unsigned __int8)v2;
	}
	v5 = *v3;
	if ((*v3 & 0xC0) != 0x80)
		return 0LL;
	v6 = v3 + 1;
	if ((unsigned __int8)v2 < 0xE0u)
	{
		if ((unsigned __int8)v2 >= 0xC2u)
		{
			*a1 = v6;
			return v5 & 0x3F | ((unsigned __int8)(v2 & 0x3F) << 6);
		}
		return 0LL;
	}
	v7 = *v6;
	if ((*v6 & 0xC0) != 0x80)
		return 0LL;
	v8 = v6 + 1;
	v9 = v7 & 0x3F | ((v5 & 0x3F | ((v2 & 0xF) << 6)) << 6);
	if ((unsigned __int8)v2 >= 0xF0u)
	{
		v10 = *v8++;
		if ((v10 & 0xC0) != 0x80)
			return 0LL;
		v9 = (v9 << 6) | v10 & 0x3F;
		if (v9 > 0x10FFFF)
			return 0LL;
	}
	else if (v9 - 55296 <= 0x7FF)
	{
		return 0LL;
	}
	result = *(uint64_t*)*a1;
	*(uint32_t*)&result = v9;

	*a1 = v8;
	return result;
}

uint64_t getFontGlyphIndex(Font_t* font, int a2)
{

	__int64 v5; // r8
	char v6; // al
	__int64 v7; // r8
	__int64 v8; // r11

	v5 = (unsigned int)(a2 - font->unicodeIndex) >> 6;
	if ((unsigned int)v5 < font->unicodeChunk.size())
	{
		v6 = (a2 - font->unicodeIndex) & 0x3F;
		v7 = font->unicodeChunk[v5];
		v8 = font->glyphChunks[v7].mask;
		if (((1LL << v6) & v8) != 0)
			return __popcnt64((v8 & ((1LL << v6) - 1))) + (unsigned int)font->glyphChunks[v7].glyphIndex;
	}
	v5 = (unsigned int)(9633 - font->unicodeIndex) >> 6;
	if ((unsigned int)v5 < font->unicodeChunk.size())
	{
		v6 = (9633 - font->unicodeIndex) & 0x3F;
		v7 = font->unicodeChunk[v5];
		v8 = font->glyphChunks[v7].mask;
		if (((1LL << v6) & v8) != 0)
			return __popcnt64((v8 & ((1LL << v6) - 1))) + (unsigned int)font->glyphChunks[v7].glyphIndex;
	}


	return 0xFFFFFFFFLL;
}

const char* sub_F98F0(const char** a3, __int64 a4, const char* a5)
{
	const char* v5; // rsi
	const char* v10; // rax
	const char* v11; // rdi
	const char* result; // rax
	char v13; // cl
	const char* v14; // rdx
	size_t v15; // rdi
	int Asset; // eax
	unsigned int v17; // eax
	__int64 qword_8; // r13
	char assetName[112]; // [rsp+20h] [rbp-88h] BYREF

	v5 = *a3;
	v10 = strchr(*a3, '%');
	v11 = v10;
	if (!v10)
		return 0LL;
	*a3 = v10 + 1;
	v13 = *v5;
	if (*v5 != '$')
	{
		if (v13 == '<')
		{
			if (v10 == v5 + 1)
			{
				*(int*)a4 = 0x8080B2F3;
				result = (const char*)a4;
				*(char*)(a4 + 4) = 0;
				return result;
			}
			return 0LL;
		}
		if (v13 == 62)
		{
			if (v10 == v5 + 1)
			{
				*(int*)a4 = -2122272013;
				result = (const char*)a4;
				*(char*)(a4 + 4) = 0;
				return result;
			}
			return 0LL;
		}
		//qword_8 = a2->qword_8;
		//if (((unsigned __int8)v13 | 0x20) == 0x7B)
		//{
		//	if (*(v10 - 1) != v13 + 2)
		//		return 0LL;
		//	v19 = strchr(v5, 124);
		//	v20 = v19;
		//	if (!v19 || v19 > v11)
		//		return 0LL;
		//	if(false)//if (!*(_DWORD*)qword_8)
		//	{
		//		v5 = v19 + 1;
		//		v21 = v11 - (v19 + 1) - 1;
		//	LABEL_25:
		//		if (v21 - (unsigned __int64)*(unsigned __int16*)(qword_8 + 4) > *(unsigned __int16*)(qword_8 + 6)
		//			- (unsigned __int64)*(unsigned __int16*)(qword_8 + 4))
		//			return *(const CHAR**)(qword_8 + 24);
		//		v22 = *(_QWORD*)(qword_8 + 8);
		//		v23 = *(unsigned __int16*)(v22 + 2 * v21);
		//		v24 = *(unsigned __int16*)(v22 + 2 * v21 + 2);
		//		if (v23 >= v24)
		//			return *(const CHAR**)(qword_8 + 24);
		//		while (1)
		//		{
		//			v25 = *(_QWORD*)(qword_8 + 16) + 16LL * ((v24 + v23) >> 1);
		//			v26 = strncmp(v5, *(char**)v25, v21);
		//			if (v26 >= 0)
		//			{
		//				if (v26 <= 0)
		//					return *(const CHAR**)(v25 + 8);
		//				v23 = ((v24 + v23) >> 1) + 1;
		//			}
		//			else
		//			{
		//				v24 = (v24 + v23) >> 1;
		//			}
		//			if (v23 >= v24)
		//				return *(const CHAR**)(qword_8 + 24);
		//		}
		//	}
		//	++v5;
		//}
		//else
		//{
		//	v20 = v10;
		//}
		//v21 = v20 - v5;
		//goto LABEL_25;
	}
	v14 = v5 + 1;
	if (*(v10 - 1) == 91)
		v11 = v10 - 1;
	v15 = v11 - v14;
	if (v15 >= 0x64)
		return 0LL;
	memmove(assetName, v14, v15);
	assetName[v15] = 0;
	Asset = loadAsset(assetName);

	if (Asset == -1)
		return "";
	//curAsset = Asset;
	//Asset = 0;
	*(char*)a4 = ((unsigned int)(0xF0000) >> 18) | 0xF0;
	*(char*)(a4 + 1) = ((unsigned int)(0xF0000) >> 12) & 0x3F | 0x80;
	v17 = 0xF0000;
	*(char*)(a4 + 2) = (v17 >> 6) & 0x3F | 0x80;
	*(char*)(a4 + 3) = v17 & 0x3F | 0x80;
	*(uint32_t*)(a4 + 4) = Asset;


	return (const char*)a4;
}

bool Render_AssetSmall(RenderInstance& proto, AssetCircleInputData data) {
	__int16 uint8_18; // r9

	__int64 result; // rax
	__int64 transformIndex; // r14

	__m128 v11; // xmm2
	int v12; // ebx
	__m128 v13; // xmm6
	__m128 v14; // xmm1
	__m128 v15; // xmm7
	uint32_t assetHash; // rcx
	__int16 assetIndex; // r12
	__int16 pixelBufferElementCount; // cx
	__int16 v22; // r15
	__m128 v23; // xmm10
	__m128 v24; // xmm11
	__m128 v25; // xmm12
	__m128 v26; // xmm13
	__m128 v27; // xmm15
	__m128 v28; // xmm14
	float v30; // xmm9_4
	__m128 v35; // xmm4
	__m128 v36; // xmm3
	textureOffset* v37; // rcx
	__m128 v38; // xmm0
	__m128 v39; // xmm4
	__m128 v40; // xmm1
	__m128 v41; // xmm3
	__m128 v42; // xmm5
	__m128 v43; // xmm4
	__m128 v44; // xmm0
	__m128 v46; // xmm6
	__m128 v47; // xmm3
	__m128 v48; // xmm2
	__m128 v49; // xmm0
	__m128 v50; // xmm6
	__m128 v51; // xmm3
	__m128 v52; // xmm4
	__m128i si128; // xmm2
	__m128 v54; // xmm6
	__m128 v55; // xmm1
	__m128 v56; // xmm0
	__m128 v57; // xmm6

	float v59; // [rsp+20h] [rbp-198h]
	__m128 v60; // [rsp+30h] [rbp-188h]
	__m128 v61; // [rsp+40h] [rbp-178h]

	float v64; // [rsp+1C8h] [rbp+10h]
	float v65; // [rsp+1D0h] [rbp+18h]
	RenderQuad quad;

	const TransformResult& transform = data.transform;
	if ( data.mainColor.value.alpha <= 0.0 )
		return 1LL;
	v11 = _mm_sub_ps(
		_mm_mul_ps(_mm_shuffle_ps(transform.directionVector,transform.directionVector, _MM_SHUFFLE(3,3,3,3)),
			_mm_shuffle_ps(transform.directionVector,transform.directionVector, _MM_SHUFFLE(0,0,0,0))),
		_mm_mul_ps(_mm_shuffle_ps(transform.directionVector,transform.directionVector, _MM_SHUFFLE(2,2,2,2)),
			_mm_shuffle_ps(transform.directionVector,transform.directionVector, _MM_SHUFFLE(1,1,1,1))));
	if ( _mm_movemask_ps(_mm_cmpeq_ps(v11, _mm_setzero_ps())) )
		return 1LL;
	v12 = _mm_movemask_ps(v11) & 2;
	v13 = _mm_div_ps(_mm_xor_ps(_mm_shuffle_ps(transform.directionVector,transform.directionVector, _MM_SHUFFLE(0,2,1,3)), _mm_set_ps(0,-0.0,-0.0,0)), v11);
	v14 = _mm_mul_ps(_mm_xor_ps(v13, _mm_set1_ps(-0.0)), _mm_shuffle_ps(transform.position,transform.position, _MM_SHUFFLE(3,1,2,0)));
	v15 = _mm_add_ps(_mm_shuffle_ps(v14,v14, _MM_SHUFFLE(1,0,3,2)), v14);
	assetHash = data.mainAsset.hash;
	if ( assetHash == 0xFFFFFFFF )
		return 1LL;
	quad.assetIndex2 = -1;



	assetIndex = imageAssetMap[assetHash].imageIndex;//g_AssetIndexData[v16].assetIndex;
	//pixelBufferElementCount = a4->pixelBufferElementCount;
	v22 = data.flags | imageAssetMap[assetHash].flags;
	quad.assetIndex = assetIndex;
	quad.flags = v22;
	quad.styleDescriptorIndex = proto.styleDescriptor.size();// + pixelBufferElementCount;
	StyleDescriptorShader_t styleDesc;
	styleDesc.color0 = data.mainColor.value;
	styleDesc.color1 = data.scndColor.value;
	styleDesc.color2 = data.tertColor.value;
	styleDesc.blend = data.blend.value;
	styleDesc.premul = data.premul.value;
	styleDesc._anon_0 = data.innerSliceBlend.value;
	styleDesc._anon_1 = data.sliceBegin.value * TAU;
	styleDesc._anon_2 = data.sliceEnd.value * TAU;
	styleDesc._anon_3 = data.ellipseSize.value.x;
	styleDesc._anon_4 = data.ellipseSize.value.y;
	styleDesc._anon_5 = data.innerMask.value;
	styleDesc._anon_6 = 1.0f / fmaxf(1.1754944e-38f, data.vingette.value);
	proto.styleDescriptor.push_back(styleDesc);
	v23 = _mm_set_ps(0,0,0,data.mins.value.x);
	v24 = _mm_set_ps(0,0,0,data.mins.value.y);
	v25 = _mm_set_ps(0,0,0,data.maxs.value.x);
	v26 = _mm_set_ps(0,0,0,data.maxs.value.y);
	v64 = data.texMins.value.x;
	v65 = data.texMins.value.y;
	v27 = _mm_set_ps(0,0,0,data.texMaxs.value.x);
	v28 = _mm_set_ps(0,0,0,data.texMaxs.value.y);
	v59 = data.vingette.value;


	if ( fminf(transform.inputSize.m128_f32[0], transform.inputSize.m128_f32[2]) <= 0.0 )
		return 1LL;
	proto.AddImageAtlasSegment( &imageAtlases[imageAssetMap[assetHash].atlasIndex]);

	v35 = _mm_unpacklo_ps(v27, v28);
	v36 = _mm_setzero_ps();
	v37 = &imageAtlases[imageAssetMap[assetHash].atlasIndex].offsets[assetIndex];//&uiImageAtlases[g_AssetIndexData[v18].atlasIndex].textureOffsets[assetIndex];
	v38 = _mm_unpacklo_ps(_mm_set_ps(0,0,0,v64), _mm_set_ps(0,0,0,v65));
	v39 = _mm_movelh_ps(v35, v35);
	v28.m128_f32[0] = (float)(v28.m128_f32[0] - v65) * v59;
	v60 = _mm_movelh_ps(v38, v38);
	v61 = _mm_max_ps(_mm_sub_ps(v39, v60), _mm_set1_ps(1.1754944e-38f));
	v40 = xmm_12A4E830a[((__int64)v22 >> 4) & 3];
	v36.m128_f32[0] = (float)((float)(transform.inputSize.m128_f32[0] * v59) * (float)(v27.m128_f32[0] - v64)) / transform.inputSize.m128_f32[0];
	v41 = _mm_unpacklo_ps(v36, v28);
	v42 = _mm_div_ps(
		_mm_add_ps(
			_mm_sub_ps(v37->m128_0, _mm_xor_ps(_mm_and_ps(_mm_min_ps(v60, v39), v40), _mm_set_ps(0,0,-0.0,-0.0))),
			_mm_movelh_ps(v41, v41)),
		_mm_or_ps(
			_mm_and_ps(_mm_andnot_ps(_mm_set1_ps(-0.0), v61), v40),
			_mm_andnot_ps(v40, _mm_set1_ps(1))));
	if ( _mm_movemask_ps(_mm_cmplt_ps(v42, _mm_set1_ps(-std::numeric_limits<float>::max()))) )
		return 1LL;
	v43 = _mm_xor_ps(
		_mm_min_ps(
			_mm_movelh_ps(_mm_xor_ps(_mm_unpacklo_ps(v23, v24), _mm_set1_ps(-0.0)), _mm_unpacklo_ps(v25, v26)),
			v42),
		_mm_set_ps(0,0,-0.0,-0.0));
	if ( _mm_movemask_ps(_mm_cmple_ps(_mm_shuffle_ps(v43,v43, _MM_SHUFFLE(3,2,3,2)), (__m128)_mm_shuffle_ps(v43,v43, _MM_SHUFFLE(1,0,1,0)))) )
		return 1LL;
	v44 = _mm_shuffle_ps(v37->m128_10,v37->m128_10,_MM_SHUFFLE(3,2,3,2));
	v46 = _mm_mul_ps(v13, _mm_set1_ps(2));
	v47 = _mm_mul_ps(_mm_mul_ps(v13, v61), v44);


	v48 = _mm_add_ps(
		_mm_mul_ps(_mm_add_ps(_mm_mul_ps(v15, v61), v60), v44),
		_mm_shuffle_ps(v37->m128_10,_mm_setzero_ps(),_MM_SHUFFLE(1,0,0,0)));

	v49 = _mm_movelh_ps(v47, v46);
	v50 = _mm_movehl_ps(v46, v47);
	v51 = _mm_shuffle_ps(v43,v43,  _MM_SHUFFLE(1,3,3,1));
	v52 = _mm_shuffle_ps(v43,v43,  _MM_SHUFFLE(2,2,0,0));
	quad.xUvVector = v49;
	quad.yUvVector = v50;
	quad.UvBase = _mm_movelh_ps(
		v48,
		_mm_sub_ps(_mm_mul_ps(v15, _mm_set1_ps(2.f)), _mm_set1_ps(1.f)));

	quad.m128_30 = _mm_setzero_ps();
	quad.m128_40 = _mm_setzero_ps();
	quad.m128_50 = _mm_setzero_ps();
	v54 = _mm_add_ps(
		_mm_add_ps(
			_mm_mul_ps(_mm_shuffle_ps(transform.directionVector,transform.directionVector, _MM_SHUFFLE(2,2,2,2)), v51),
			_mm_mul_ps(_mm_shuffle_ps(transform.directionVector,transform.directionVector, _MM_SHUFFLE(0,0,0,0)), v52)),
		_mm_shuffle_ps(transform.position,transform.position, _MM_SHUFFLE(0,0,0,0)));
	v55 = _mm_add_ps(
		_mm_add_ps(
			_mm_mul_ps(_mm_shuffle_ps(transform.directionVector,transform.directionVector, _MM_SHUFFLE(3,3,3,3)), v51),
			_mm_mul_ps(_mm_shuffle_ps(transform.directionVector,transform.directionVector, _MM_SHUFFLE(1,1,1,1)), v52)),
		_mm_shuffle_ps(transform.position,transform.position, 85));
	v56 = _mm_unpacklo_ps(v54, v55);
	v57 = _mm_unpackhi_ps(v54, v55);
	if ( v12 == 2 )
	{
		v56 = _mm_shuffle_ps(v56,v56, _MM_SHUFFLE(1,0,3,2));
		v57 = _mm_shuffle_ps(v57,v56, _MM_SHUFFLE(1,0,3,2));
	}
	*(__m128 *)&quad.vert[0][0] = v56;
	*(__m128 *)&quad.vert[2][0] = v57;
	proto.AddQuad(quad);
	return true;
}


bool Text_Render(RenderInstance& proto, TextInputData data, TransformResult transform) {

	__m128 v11; // xmm2
	__m128 v13; // xmm0
	__m128 v14; // xmm12
	__m128 v15; // xmm1
	__m128 v16; // xmm13
	__m128 v31; // xmm15
	float v38; // xmm0_4
	float v39; // xmm2_4
	float v40; // xmm0_4
	float v42; // xmm0_4
	__m128 v46; // xmm0
	__m128 v47; // xmm6
	__m128 v49; // xmm7
	uint32_t assetHash; // rax
	__int64 assetIndex; // rdx
	Asset_t a7; // r10
	__m128 v53; // xmm0
	__m128 v54; // xmm2
	__m128 v55;
	textureOffset* v57; // r8
	__int64 uint16_2; // rax
	__m128 v60; // xmm0
	__m128 v62; // xmm0
	__m128 v63; // xmm1
	__m128 v64; // xmm2
	__m128 v65; // xmm0
	__int64 v66; // rax
	//unknownRuiListElement* qword_0; // rdx
	FontAtlas_t* fontAtlas; // r8
	unsigned int v76; // ecx
	unsigned __int8 v77; // bl
	const char* printString; // rdx MAPDST
	bool stringStartsWithEscapeChar; // zf
	char* currentCharPointer; // rax MAPDST
	__int64 byte_4; // rdx
	unsigned int v82; // eax
	unsigned int v83; // r13d
	float v84; // xmm14_4
	float v85; // xmm14_4
	unsigned int v86; // eax
	float v87; // xmm0_4
	float v88; // xmm12_4
	__int64 v89; // rax
	Glyph_t* fontGlyph; // rbx
	Font_t* v91; // rdx MAPDST
	float v93; // xmm11_4
	__m128 v94; // xmm12
	__m128 v95; // xmm13
	float v96; // xmm10_4
	__m128 v97; // xmm8
	__m128 v98; // xmm8
	float v101; // xmm6_4
	__m128 v102; // xmm9
	__m128 v103; // xmm3
	__m128 v104; // xmm12
	__m128 v105; // xmm0
	__m128 v106; // xmm3
	__m128 proportionScale; // xmm0
	float v108; // xmm9_4
	float v109; // xmm8_4
	int previousChar; // r14d
	unsigned int v111; // esi
	float v112; // xmm10_4
	__m128 v113; // xmm13
	__m128 v114; // xmm15
	unsigned int v115; // r15d
	uint64_t currentUnicodeChar; // eax
	char nextChar; // dl
	const char* v119; // rax
	__int64 v120; // rdx
	bool v121; // r12
	float v122; // xmm11_4
	int fontGlyphIndex; // eax
	Font_t* v124; // r10
	int kerningStartIndex; // edx
	int kerningEndIndex; // r8d

	float float_4; // xmm0_4
	Glyph_t* glyph; // rax
	float float_0; // xmm5_4
	bool v131; // r15
	float float_10; // xmm0_4
	__m128 v133; // xmm4
	__m128 v134; // xmm5
	__m128 v135; // xmm0
	__m128 v136; // xmm1
	Glyph_t* v137; // rdx
	Glyph_t* v138; // r8
	float v139; // xmm7_4
	__m128 v140; // xmm8
	__m128 v141; // xmm9
	__m128 v142; // xmm4
	__m128 float_14_low; // xmm2
	__m128 float_1C_low; // xmm3
	float v145; // xmm7_4
	__m128 v146; // xmm1
	__m128 v147; // xmm5
	__m128 v148; // xmm5
	__m128 v149; // xmm6
	__m128 v150; // xmm6
	__m128 v153; // xmm1
	__m128 v154; // xmm2
	//ruiDrawInfo* drawInfo; // rcx
	__int64 v157; // rax
	float v158; // xmm0_4
	float v159; // xmm0_4
	__m128 v160; // xmm1
	float v161; // xmm10_4
	__m128 v162; // xmm0
	__int64 v163; // r9
	float v164; // xmm1_4
	float* v165; // rdx
	float v166; // xmm1_4
	bool v167; // [rsp+60h] [rbp-A0h]
	unsigned int v169; // [rsp+70h] [rbp-90h]
	unsigned int v170; // [rsp+74h] [rbp-8Ch]
	__m128 v171; // [rsp+78h] [rbp-88h]
	float v172; // [rsp+7Ch] [rbp-84h]
	unsigned int v173; // [rsp+80h] [rbp-80h]
	__m128 a8; // [rsp+90h] [rbp-70h] BYREF
	fontRenderingStruct v175; // [rsp+90h] [rbp-70h] SPLIT
	int a6; // [rsp+A0h] [rbp-60h]
	float v177; // [rsp+A4h] [rbp-5Ch]
	__m128 a10; // [rsp+B0h] [rbp-50h] BYREF
	fontRenderingStruct v179; // [rsp+C0h] [rbp-40h]
	__m128 a9; // [rsp+D0h] [rbp-30h] BYREF
	fontRenderingStruct v181; // [rsp+E0h] [rbp-20h]
	unsigned int v182; // [rsp+F0h] [rbp-10h]
	float v183; // [rsp+F4h] [rbp-Ch]
	float v184; // [rsp+F8h] [rbp-8h]
	float v186; // [rsp+108h] [rbp+8h]
	__m128 a11; // [rsp+120h] [rbp+20h] BYREF
	char v191[8]; // [rsp+138h] [rbp+38h] BYREF

	//__m128 v195; // [rsp+180h] [rbp+80h]
	Font_t* fontArray[4]; // [rsp+190h] [rbp+90h]
	__m128 v197; // [rsp+1B0h] [rbp+B0h]
	__m128 v198; // [rsp+1C0h] [rbp+C0h]
	__m128 v199; // [rsp+1D0h] [rbp+D0h]
	__m128 v200; // [rsp+1E0h] [rbp+E0h]
	//ruiDrawTriangle tri;
	__m128 v204; // [rsp+220h] [rbp+120h]
	__m128 v206; // [rsp+240h] [rbp+140h]
	//ruiUnknownStruct_1 a4a; // [rsp+250h] [rbp+150h] BYREF
	//ruiUnknownStruct_1 v208; // [rsp+2C0h] [rbp+1C0h] BYREF
	__m128 v209[5]; // [rsp+330h] [rbp+230h] BYREF
	char* savedTextPointer[5];
	RenderQuad quad;

	if (_mm_movemask_ps(_mm_cmpeq_ps(_mm_setzero_ps(), transform.inputSize)))
		return 1LL;

	v11 = _mm_sub_ps(
		_mm_mul_ps(
			_mm_shuffle_ps(transform.directionVector, transform.directionVector, _MM_SHUFFLE(3, 3, 3, 3)),
			_mm_shuffle_ps(transform.directionVector, transform.directionVector, _MM_SHUFFLE(0, 0, 0, 0))),
		_mm_mul_ps(
			_mm_shuffle_ps(transform.directionVector, transform.directionVector, _MM_SHUFFLE(2, 2, 2, 2)),
			_mm_shuffle_ps(transform.directionVector, transform.directionVector, _MM_SHUFFLE(1, 1, 1, 1))));
	if (_mm_movemask_ps(_mm_cmpeq_ps(v11, _mm_setzero_ps())))
		return 1LL;

	v13 = (__m128)_mm_shuffle_ps(transform.position, transform.position, _MM_SHUFFLE(3, 1, 2, 0));
	v14 = _mm_div_ps(_mm_xor_ps((__m128)_mm_shuffle_ps(transform.directionVector, transform.directionVector, _MM_SHUFFLE(0, 2, 1, 3)), _mm_set_ps(0, -0.0, -0.0, 0)), v11);
	a6 = _mm_movemask_ps(v11) & 2;
	v197 = v14;
	v15 = _mm_mul_ps(_mm_xor_ps(v14, _mm_set1_ps(-0.0)), v13);
	v16 = _mm_add_ps((__m128)_mm_shuffle_ps(v15, v15, _MM_SHUFFLE(1, 0, 3, 2)), (__m128)v15);
	v198 = v16;


	v31 = NRReciprocal(transform.inputSize);



	float maxSize = -std::numeric_limits<float>::max();
	size_t styleDescriptorBase = proto.styleDescriptor.size();
	for (int i = 0; i < 4; i++) {
		fontArray[i] = getFontByIndex(data.styles[i].fontIndex);
		FontAtlas_t* atlas = getFontAtlasByIndex(data.styles[i].fontIndex);
		float v17 = (float)((float)atlas->width * fontArray[i]->proportionScaleX)
			/ data.styles[i].size.value;
		maxSize = fmax(maxSize,(float)(data.styles[i].size.value * fontArray[i]->float_24) - data.styles[i].style_32.value);

		StyleDescriptorShader_t style;
		style.color0 = data.styles[i].mainColor.value;
		style.color1 = data.styles[i].scndColor.value;
		style.color2 = data.styles[i].tertColor.value;
		style.blend = data.styles[i].blend.value;
		style.premul = data.styles[i].premul.value;
		style._anon_0 = v17 * data.styles[i].dropShadowOffset.value.x * atlas->widthRatio;
		style._anon_1 = v17 * data.styles[i].dropShadowOffset.value.y * atlas->heightRatio;
		style._anon_2 = data.styles[i].dropShadowHardness.value;
		style._anon_3 = 1.f / fmax(std::numeric_limits<float>::min(),data.styles[i].dropShadowBlur.value);
		style._anon_4 = v17 * data.styles[i].backgroundSize.value;
		style._anon_5 = v17 * data.styles[i].boltness.value;
		style._anon_6 = 1.f / fmaxf(1.1754944e-38f, v17 * data.styles[i].blur.value);

		proto.styleDescriptor.push_back(style);
	}


	v38 = _mm_shuffle_ps(v31, v31, _MM_SHUFFLE(3, 3, 3, 3)).m128_f32[0];



	v39 = v38 * maxSize;
	v171 = _mm_set_ps(0, 0, 0, v39);
	v40 = v38 * data.minSize.value.y;
	v184 = v40;
	v42 = v31.m128_f32[0] * data.minSize.value.x;
	//v43 = ((unsigned __int64)a3 - (unsigned __int64)a2->header->renderJobs) >> 4;
	v177 = v42;
	if (data.inlineImages.size())
	{

		v46 = _mm_set_ps(0, 0, 0, v39);
		v47 = (__m128)_mm_shuffle_ps(v31, v31, _MM_SHUFFLE(3, 1, 2, 0));
		v49 = (__m128)_mm_shuffle_ps(v46, v46, _MM_SHUFFLE(0, 1, 0, 1));
		a9 = _mm_set_ps(1, 1, 0, 0);
		for(auto& image:data.inlineImages)
		{

			assetHash = image.hash;
			assetIndex = imageAssetMap[assetHash].imageIndex;

			a7 = imageAssetMap[assetHash];
			v53 = _mm_mul_ps(_mm_set_ps(image.afloat_4[1], image.afloat_4[0], image.afloat_4[1], image.afloat_4[0]), v47);//_mm_loaddup_pd((const double *)v4->struct_28D0[byte_6].afloat_4)
			v54 = _mm_sub_ps(
				_mm_mul_ps(_mm_set_ps(image.afloat_4[3], image.afloat_4[2], image.afloat_4[3], image.afloat_4[2]), v47),
				v53);
			v55 = _mm_add_ps(v49, v53);

			v57 = &imageAtlases[imageAssetMap[assetHash].atlasIndex].offsets[assetIndex];
			uint16_2 = image.styleId;

			v60 = v57->m128_0;

			a8 = _mm_add_ps(_mm_mul_ps(_mm_xor_ps(v60, _mm_set_ps(0, 0, -0.0, -0.0)), v54), v55);
			a11 = NRReciprocal(v54);
			v62 = _mm_shuffle_ps(v57->m128_10, v57->m128_10, _MM_SHUFFLE(3, 2, 3, 2));//_mm_loaddup_pd((const double *)(v57 + 24));
			v63 = _mm_mul_ps(_mm_mul_ps(_mm_sub_ps(v16, v55), a11), v62);
			v64 = _mm_mul_ps(_mm_mul_ps(v14, a11), v62);
			a10 = _mm_xor_ps(_mm_mul_ps(a11, v55), _mm_set1_ps(-0.0));
			v65 = _mm_shuffle_ps(v57->m128_10, v57->m128_10, _MM_SHUFFLE(1, 0, 1, 0));//(__m128)_mm_loadl_epi64((const __m128i *)(v57 + 16));
			quad.UvBase = _mm_add_ps(v63, v65);
			quad.xUvVector = _mm_shuffle_ps(v64, v64, _MM_SHUFFLE(1, 0, 1, 0));
			quad.yUvVector = _mm_shuffle_ps(v64, v64, _MM_SHUFFLE(3, 2, 3, 2));
			quad.m128_30 = _mm_setzero_ps();
			quad.m128_40 = _mm_setzero_ps();
			quad.m128_50 = _mm_setzero_ps();
			quad.assetIndex = assetIndex;
			quad.assetIndex2 = -1;
			quad.styleDescriptorIndex = image.styleId + styleDescriptorBase;
			quad.flags = 0x1F00;
			__m128 ruiSize = _mm_set_ps(proto.elementHeightRatio,proto.elementWidthRatio,proto.elementHeight,proto.elementWidth);
			proto.sub_F9B80_rev( ruiSize, quad, transform, a6, a7, a8, a9, a10, a11);


		}
		v42 = v177;
	}
	v66 = data.styles[0].fontIndex;

	fontAtlas = getFontAtlasByIndex(v66);
	proto.AddFontAtlasSegment(fontAtlas);

	v76 = 0;
	v77 = 0;
	printString = data.text.value.c_str();
	v169 = 0;
	currentCharPointer = (char*)printString;
	stringStartsWithEscapeChar = *printString == '`';
	currentCharPointer = (char*)printString;
	if (stringStartsWithEscapeChar)
	{
		do
		{
			v77 = currentCharPointer[1] - 48;
			if (v77 >= 4u)
				break;
			currentCharPointer += 2;
			++v76;
		} while (*currentCharPointer == '`');
		v169 = v76;
	}
	byte_4 = 0;
	v82 = data.textLines.size();
	v83 = 0;
	v170 = -1;
	v84 = 0.0;
	v173 = byte_4;
	v182 = v82;
	if (data.textLines.size())
	{
		v85 = transform.inputSize.m128_f32[0] - data.textLines[0].float_4;
		v86 = data.textLines[0].length;;
		v173 = byte_4 + 1;
		v170 = v86;
		v84 = v85 * v42;
	}
	v87 = data.float_0;
	v172 = 0.0;
	v186 = v87;
	proto.sub_FFAE0(transform.directionVector, _mm_set_ps(proto.elementHeightRatio,proto.elementWidthRatio,proto.elementHeight,proto.elementWidth), v209);
	v88 = 0.0;
	a11 = _mm_setzero_ps();
	v204 = (__m128)_mm_shuffle_ps(transform.inputSize, transform.inputSize, 216);
	while (2)
	{
		v181 = (fontRenderingStruct)0LL;
		v179 = (fontRenderingStruct)0LL;
		v175 = (fontRenderingStruct)0LL;
		v89 = v77;
		fontGlyph = 0LL;
		v91 = fontArray[v89];

		quad.flags = 0;
		quad.styleDescriptorIndex = styleDescriptorBase + v89;
		v93 = fmaxf(data.styles[v89].boltness.value, v88);
		v94 = _mm_set_ps(0, 0, 0, v186);
		v95 = _mm_set_ps(0, 0, 0, data.styles[v89].size.value);
		v94.m128_f32[0] = (float)(v186 * data.styles[v89].stretchX.value) * v95.m128_f32[0];
		v96 = data.styles[v89].backgroundSize.value;
		v97 = _mm_unpacklo_ps(v94, v95);
		v98 = _mm_movelh_ps(v97, v97);
		v167 = fmaxf(
			data.styles[v89].mainColor.value.alpha,
			fminf(fmaxf(data.styles[v89].scndColor.value.alpha, data.styles[v89].tertColor.value.alpha), v96)) > 0.0;
		v101 = v31.m128_f32[0] * v94.m128_f32[0];
		v102 = _mm_unpacklo_ps(
			_mm_set_ps(0, 0, 0, data.styles[v89].dropShadowOffset.value.x),
			_mm_set_ps(0, 0, 0, data.styles[v89].dropShadowOffset.value.y));
		v103 = _mm_set_ps(0, 0, 0, data.styles[v89].dropShadowBlur.value);
		v104 = _mm_shuffle_ps(v31, v31, 255);
		v183 = v101;
		v104.m128_f32[0] = v104.m128_f32[0] * v95.m128_f32[0];
		v105 = _mm_mul_ps(
			_mm_shuffle_ps(
				_mm_set_ps(0, 0, 0, data.styles[v89].blur.value),
				_mm_set_ps(0, 0, 0, data.styles[v89].blur.value),
				0),
			_mm_set1_ps(0.5));
		v106 = _mm_max_ps(
			_mm_add_ps(
				_mm_mul_ps(_mm_shuffle_ps(v103, v103, 0), _mm_set1_ps(0.5)),
				_mm_xor_ps(_mm_movelh_ps(v102, v102), _mm_set_ps(0, 0, -0.0, -0.0))),
			v105);
		v105.m128_f32[0] = v96 + v93;
		a10 = _mm_mul_ps(
			_mm_xor_ps(_mm_add_ps(v106, _mm_shuffle_ps(v105, v105, 0)), _mm_set_ps(0, 0, -0.0, -0.0)),
			_mm_shuffle_ps(v31, v31, _MM_SHUFFLE(3, 1, 2, 0)));
		proportionScale = _mm_set_ps(v91->proportionScaleY, v91->proportionScaleX, v91->proportionScaleY, v91->proportionScaleX);//_mm_loaddup_pd((const double *)&v91->float_1C);
		v108 = a10.m128_f32[3];
		v109 = a10.m128_f32[1];
		previousChar = 0;
		v111 = 0;
		v112 = 0.0;
		v113 = _mm_setzero_ps();
		v114 = _mm_setzero_ps();
		v199 = _mm_mul_ps(
			_mm_mul_ps(NRReciprocal(v98), v204),
			proportionScale);
		v200 = _mm_mul_ps(v197, v199);
		v206 = _mm_mul_ps(v198, v199);
		a9 = _mm_set_ps(1, -1, 0, 0);
		while (1)
		{
			v115 = v169;
			while (1)
			{
				while (1)
				{
					currentUnicodeChar = readUnicodeCharacter((const char**)&currentCharPointer);
					++v115;
					;
					v169 = v115;
					if (currentUnicodeChar == '%')
						break;
					if (currentUnicodeChar || !v83)
						goto LABEL_35;
					currentCharPointer = savedTextPointer[--v83];
				}
				nextChar = *currentCharPointer;
				if (*currentCharPointer <= ' '
					|| nextChar <= '?' && ((1 << (nextChar - 32)) & 0x80005002) != 0)
				{
					goto LABEL_35;
				}
				if (nextChar == '%')
					break;
				v119 = sub_F98F0( (const char**)&currentCharPointer, (__int64)v191, printString);
				if (!v119)
					return 1LL;
				v120 = v83++;
				savedTextPointer[v120] = currentCharPointer;
				currentCharPointer = (char*)v119;
			}
			++currentCharPointer;
		LABEL_35:
			v121 = (unsigned int)(currentUnicodeChar - 1) >= 0xEFFFF || currentUnicodeChar == '`';
			v122 = 0.0;
			if (v121)
			{
				float_0 = v175.float_0;
				glyph = v175.glyph;
				v124 = v91;
			}
			else
			{
				fontGlyphIndex = getFontGlyphIndex(v91, currentUnicodeChar);
				v124 = v91;
				fontGlyph = &v91->glyphs[fontGlyphIndex];
				kerningStartIndex = fontGlyph->kerningStartIndex;
				kerningEndIndex = fontGlyph[1].kerningStartIndex;
				if (kerningStartIndex >= kerningEndIndex)
				{
				LABEL_46:
					float_4 = 0.0;
				}
				else
				{

					while (v91->kerningInfos[(unsigned __int16)kerningStartIndex].otherChar != previousChar)
					{
						kerningStartIndex = kerningStartIndex + 1;
						if ((unsigned __int16)kerningStartIndex >= kerningEndIndex)
							goto LABEL_46;
					}
					float_4 = v91->kerningInfos[(unsigned __int16)kerningStartIndex].kerningDistance;
				}

				v122 = v101 * fontGlyph->float_0;
				glyph = fontGlyph;

				v84 = v84 + (float)(float_4 * v101);
				float_0 = v84;
				v175.float_0 = v84;
				v175.word_4 = fontGlyphIndex;
				v175.glyph = fontGlyph;
			}
			v131 = v115 >= v170;
			if (v167)
			{
				if (v131 || v121)
				{
					if (v111 > 1)
					{
						v142 = a9;
					}
					else
					{
						if (!v111)
							goto LABEL_72;
						v142 = _mm_add_ps(a9, _mm_set_ps(0, 0, 0, 1));
						v181 = v179;
						a9 = v142;
					}
					v138 = v179.glyph;
					v137 = v181.glyph;
					v111 = 0;
					float_14_low = _mm_set_ps(0, 0, 0, v181.glyph->posMinY);
					float_1C_low = _mm_set_ps(0, 0, 0, v181.glyph->posMaxY);
					float_14_low.m128_f32[0] = fminf(float_14_low.m128_f32[0], v179.glyph->posMinY);
					float_1C_low.m128_f32[0] = fmaxf(float_1C_low.m128_f32[0], v179.glyph->posMaxY);
					v145 = (float)(v101 * v179.glyph->posMaxX) + v179.float_0;
					a9 = _mm_add_ps(v142, _mm_set_ps(0, 0, 1, 0));
					v139 = v145 + a10.m128_f32[2];
					float_14_low.m128_f32[0] = (float)(float_14_low.m128_f32[0] * v104.m128_f32[0]) + v109;
					float_1C_low.m128_f32[0] = (float)(float_1C_low.m128_f32[0] * v104.m128_f32[0]) + v108;
					v140 = float_14_low;
					v141 = float_1C_low;
				}
				else
				{
					float_10 = fontGlyph->posMinX;
					if (float_10 == fontGlyph->posMaxX)
					{
						v84 = v84 + v122;
						previousChar = currentUnicodeChar;
						fontGlyph = 0LL;
						continue;
					}
					if (v111 <= 1)
					{
						v133 = v104;
						v134 = v104;
						v133.m128_f32[0] = (float)(v104.m128_f32[0] * fontGlyph->posMinY) + v109;
						v134.m128_f32[0] = (float)(v104.m128_f32[0] * fontGlyph->posMaxY) + v108;
						if (v111)
						{
							v135 = _mm_setzero_ps();
							v136 = _mm_setzero_ps();
							v135.m128_f32[0] = v113.m128_f32[0];
							v136.m128_f32[0] = v114.m128_f32[0];
							v113 = v135;
							v114 = v136;
							v113.m128_f32[0] = fminf(v135.m128_f32[0], v133.m128_f32[0]);
							v114.m128_f32[0] = fmaxf(v136.m128_f32[0], v134.m128_f32[0]);
						}
						else
						{
							v113 = (__m128)v133;
							v114 = (__m128)v134;
							v112 = (float)((float)(v101 * float_10) + a10.m128_f32[0]) + v84;
							a9 = _mm_sub_ps(a9, _mm_set_ps(0, 0, 0, 1));
						}
						++v111;
						v84 = v84 + v122;
					LABEL_60:
						v181 = v179;
						v179 = v175;
					LABEL_61:
						previousChar = currentUnicodeChar;
						fontGlyph = 0LL;
						continue;
					}
					v137 = v181.glyph;
					v138 = v179.glyph;
					v140 = _mm_set_ps(0, 0, 0, v181.glyph->posMinY);
					v141 = _mm_set_ps(0, 0, 0, v181.glyph->posMaxY);
					v139 = (float)((float)((float)(v181.glyph->posMaxX + glyph->posMinX) * v101)
						+ (float)(v181.float_0 + float_0))
						* 0.5;
					v140.m128_f32[0] = (float)(fminf(fminf(v140.m128_f32[0], v179.glyph->posMinY), glyph->posMinY)
						* v104.m128_f32[0])
						+ a10.m128_f32[1];
					v141.m128_f32[0] = (float)(fmaxf(fmaxf(v141.m128_f32[0], v179.glyph->posMaxY), glyph->posMaxY)
						* v104.m128_f32[0])
						+ a10.m128_f32[3];
				}
				v146 = _mm_setzero_ps();
				v147 = _mm_setzero_ps();
				v146.m128_f32[0] = v139;
				v147.m128_f32[0] = v112;
				v148 = _mm_shuffle_ps(v147, v146, 0);
				v149 = _mm_shuffle_ps(
					_mm_set_ps(0, 0, 0, v124->proportions[v137->proportionIndex].scaleBounds),
					_mm_set_ps(0, 0, 0, v124->proportions[v138->proportionIndex].scaleBounds),
					0);
				__m128 t1 = _mm_set_ps(v138->posBaseY, v138->posBaseX, v137->posBaseY, v137->posBaseX);//v138(r8) & v137(rdx)
				__m128 t2 = _mm_unpacklo_ps(
					_mm_unpacklo_ps(_mm_set_ps(0, 0, 0, v181.float_0), _mm_set_ps(0, 0, 0, v179.float_0)),
					_mm_unpacklo_ps((__m128)v171, v171));
				quad.UvBase = _mm_add_ps(
					_mm_mul_ps(
						_mm_sub_ps(
							v206,
							_mm_mul_ps(
								t2,
								v199)),
						(__m128)v149),
					t1);
				//_mm_loadh_ps((const double *)v138->gap_8));
				quad.xUvVector = _mm_mul_ps((__m128)_mm_shuffle_ps(v200, v200, _MM_SHUFFLE(1, 0, 1, 0)), (__m128)v149);
				quad.m128_50 = (__m128)_mm_shuffle_ps(v149, v149, 216);
				quad.yUvVector = _mm_mul_ps((__m128)_mm_shuffle_ps(v200, v200, _MM_SHUFFLE(3, 2, 3, 2)), (__m128)v149);
				quad.m128_30 = _mm_setzero_ps();
				quad.m128_40 = _mm_setzero_ps();
				quad.assetIndex = v124->baseTextureIndex + v181.word_4;
				quad.assetIndex2 = v179.word_4 + v124->baseTextureIndex;
				v150 = _mm_add_ps(
					_mm_unpacklo_ps(_mm_unpacklo_ps(v113, v141), _mm_unpacklo_ps(v114, v140)),
					_mm_shuffle_ps(v171, v171, 0));
				//si128 = _mm_load_si128((const __m128i *)v10);
				TriData tri = transform.GenTri(v150, v148);
				proto.sub_FEF30( v209, &a9, tri);
				v153 = _mm_unpackhi_ps(tri.a, tri.b);
				v154 = _mm_unpacklo_ps(tri.a, tri.b);
				if (a6 == 2)
				{
					v154 = _mm_shuffle_ps(v154, v154, _MM_SHUFFLE(1, 0, 3, 2));
					v153 = _mm_shuffle_ps(v153, v153, _MM_SHUFFLE(1, 0, 3, 2));
				}
				*(__m128*)& quad.vert[0][0] = v154;
				*(__m128*)& quad.vert[2][0] = v153;
				proto.AddQuad(quad);
				v101 = v183;
				v112 = v139;
				v113 = v140;
				v114 = v141;
				v108 = a10.m128_f32[3];
				v109 = a10.m128_f32[1];
				a9 = _mm_and_ps(a9, _mm_castsi128_ps(_mm_set_epi32(0,0,~0,~0)));
			}
		LABEL_72:
			if (v131)
			{
				v171.m128_f32[0] = v171.m128_f32[0] + (float)(v104.m128_f32[0] + v184);
				if (v173 >= v182)
				{
					v170 = -1;
					v158 = data.lastLine;//v4->float_25C4[3 * v182];
				}
				else
				{
					v157 = 3LL * v173;
					v170 = data.textLines[v173].length;
					v158 = data.textLines[v173].float_4;
					v173++;
				}
				v84 = (float)(transform.inputSize.m128_f32[0] - v158) * v177;
				if (!fontGlyph || (v159 = fontGlyph->posMinX, v159 == fontGlyph->posMaxX))
				{
					v111 = 0;
				}
				else
				{
					v160 = v104;
					v175.float_0 = v84;
					v111 = 1;
					v161 = v101 * v159;
					v162 = v104;
					v160.m128_f32[0] = (float)(v104.m128_f32[0] * fontGlyph->posMaxY) + v108;
					v112 = (float)(v161 + a10.m128_f32[0]) + v84;
					v114 = (__m128)v160;
					v162.m128_f32[0] = (float)(v104.m128_f32[0] * fontGlyph->posMinY) + v109;
					v113 = (__m128)v162;
				}
			}
			v84 = v84 + v122;
			if (!v121)
				goto LABEL_60;
			if (!currentUnicodeChar)
				return 1LL;
			if (currentUnicodeChar == 96)
				break;
			if ((unsigned int)(currentUnicodeChar - 0xF0000) >= 0x2000)
			{
				if (currentUnicodeChar != 0xF2001)
					goto LABEL_61;
				v84 = v84 + v172;
				previousChar = 0xF2001;
				fontGlyph = 0LL;
				v172 = 0.0;
			}
			else
			{
				uint32_t assetIndex = ((uint32_t*)&currentUnicodeChar)[1];
				//v163 = (__int16)g_AssetIndexData[(unsigned __int16)v117].assetIndex;
				v163 = (__int16)imageAssetMap[assetIndex].imageIndex;
				//v164 = (float)uiImageAtlases[ g_AssetIndexData[(unsigned __int16)v117].atlasIndex].textureDimensions[v163].height;
				v164 = (float)imageAtlases[imageAssetMap[assetIndex].atlasIndex].dimentions[v163].height;
				if (previousChar == 0xF2000)
				{
					//if ( (unsigned __int16)v163 >= uiImageAtlases[ g_AssetIndexData[v117].atlasIndex].TextureCount )
					if ((unsigned __int16)v163 >= imageAtlases[imageAssetMap[assetIndex].atlasIndex].renderOffsets.size())
					{
						v172 = 0.0;
						previousChar = currentUnicodeChar;
						v84 = v84 + 0.0;
						fontGlyph = 0LL;
					}
					else
					{
						previousChar = currentUnicodeChar;
						//v165 = (float *)&uiImageAtlases[ g_AssetIndexData[(unsigned __int16)v117].atlasIndex].pointer_20[v163];
						v165 = (float*)&imageAtlases[imageAssetMap[assetIndex].atlasIndex].renderOffsets[v163];
						fontGlyph = 0LL;
						v166 = v31.m128_f32[0] * v164;
						v84 = v84 + (float)(v166 * *v165);
						v172 = v166 * v165[2];
					}
				}
				else
				{
					previousChar = currentUnicodeChar;
					fontGlyph = 0LL;
					v84 = v84
						+ (float)((float)(v164
							// / (float)uiImageAtlases[ g_AssetIndexData[(unsigned __int16)v117].atlasIndex].textureDimensions[v163].width)
							/ (float)imageAtlases[imageAssetMap[assetIndex].atlasIndex].dimentions[v163].width)
							* v101);
				}
			}
		}
		v77 = *currentCharPointer - 48;
		if (v77 < 4u)
		{
			++currentCharPointer;
			v88 = a11.m128_f32[0];
			continue;
		}

		return 1LL;
	}
}

__m128 GetTextSize(TextInputData& data) {



	//ruiStyleDescriptor* unk8Struct_1; // r10
	//ruiStyleDescriptor* unk8Struct_0; // r9
	//ruiStyleDescriptor* unk8Struct_2; // r11
	float v33; // xmm8_4
	float v35; // xmm6_4
	unsigned __int8 currentStyleId; // r15
	float v38; // xmm15_4
	float v39; // xmm10_4
	float v40; // xmm11_4
	float v42; // xmm7_4
	float v44; // xmm9_4
	int v45; // r12d
	//ruiStyleDescriptor* unk8Struct_3; // rbx
	float* v49; // r14
	float v50; // xmm8_4
	float v51; // xmm7_4
	float v52; // xmm15_4
	float fVar23; // xmm8_4
	const char* printText; // rdx MAPDST
	__int64 savedTextPointerIndex; // r13
	float v57; // xmm13_4
	float v58; // xmm12_4
	uint64_t currentUnicodeChar; // eax MAPDST
	char v62; // r8

	char* v64; // rax
	const char* v65; // rcx
	int FontGlyphIndex; // eax
	Glyph_t* fontGlyph; // r10
	int kerningIndex; // ecx
	int kerningEnd; // r8d
	float float_4; // xmm2_4
	float v72; // xmm1_4
	uint32_t v75; // ecx
	int v76; // r8d
	unsigned __int64 v77; // rdx
	float v78; // xmm3_4
	__int64 assetIndex; // r9
	ImageAtlasTextureDimention* v88; // rdx
	float* v89; // rcx
	float v90; // xmm1_4
	float v91; // xmm0_4
	float v92; // xmm1_4
	float v93; // xmm5_4
	float i; // xmm6_4
	__int64 v96; // rcx
	float v97; // xmm2_4
	__m128 v101; // xmm1
	float v102; // xmm5_4
	__m128 v104; // xmm2_4
	__m128 result; // xmm0
	uint32_t v108; // [rsp+34h] [rbp-CCh]
	const char* currentCharPointer; // [rsp+38h] [rbp-C8h] BYREF
	int v110; // [rsp+40h] [rbp-C0h]
	Font_t* a1a; // [rsp+50h] [rbp-B0h]
	float sizeX[4]; // [rsp+60h] [rbp-A0h]
	float v116[4]; // [rsp+70h] [rbp-90h]
	float sizeY[4]; // [rsp+80h] [rbp-80h]
	//float v118; // [rsp+84h] [rbp-7Ch]
	//float v119; // [rsp+88h] [rbp-78h]
	FontAtlas_t* fontAtlas; // [rsp+90h] [rbp-70h]

	char v124[8]; // [rsp+A8h] [rbp-58h] BYREF
	Font_t* v125[4]; // [rsp+B0h] [rbp-50h]
	const char* savedTextPointer[5]; // [rsp+D0h] [rbp-30h]
	unsigned __int8 v129; // [rsp+210h] [rbp+110h]
	int stringUnicodeLength; // [rsp+218h] [rbp+118h]
	unsigned int v130;


	bool setInlineImageData = false;







	for (int i = 0; i < 4; i++) {
		v125[i] = getFontByIndex(data.styles[i].fontIndex);
		sizeY[i] = data.styles[i].size.value;
		sizeX[i] = sizeY[i] * data.styles[i].stretchX.value;
		v116[i] = (float)(sizeY[i] * v125[i]->float_24) - data.styles[i].style_32.value;

	}
	v33 = fmaxf(v116[0], v116[1]);
	v50 = fmaxf(v33, fmaxf(v116[2], v116[3]));
	v38 = fmaxf(sizeY[0] - v116[0], sizeY[1] - v116[1]);
	v52 = fmaxf(v38, fmaxf(sizeY[2] - v116[2], sizeY[3] - v116[3])) + v50;

	a1a = v125[0];

	v108 = 0;

	v35 = 0.0;
	currentStyleId = 0;




	v39 = 0.0;
	v40 = 0.0;

	stringUnicodeLength = 0;
	v110 = 0;
	v42 = v116[2];


	v44 = 0.0;
	v129 = 0;
	v45 = 0;
	v130 = 0;


	//fontGlyphs = a1a->glyphs;
	v49 = 0LL;



	v51 = 0.0;

	fVar23 = 0.0;
	printText = data.text.value.c_str();

	savedTextPointerIndex = 0LL;
	v57 = data.maxSize.value.y;
	v58 = data.minSize.value.y;

	currentCharPointer = printText;
	fontAtlas = getFontAtlasByIndex(data.styles[0].fontIndex);
	while (1)
	{
		while (1)
		{
			while (1)
			{
				while (1)
				{
					currentUnicodeChar = readUnicodeCharacter((const char**)&currentCharPointer);
					++stringUnicodeLength;
					if ((unsigned int)(currentUnicodeChar - 1) >= 0xEFFFF || currentUnicodeChar == '`')
						break;
					if (currentUnicodeChar != '%'
						|| (v62 = *currentCharPointer, *currentCharPointer <= 32)
						|| v62 <= 63 && ((1 << (v62 - 32)) & 0x80005002) != 0)
					{
					LABEL_12:
						FontGlyphIndex = getFontGlyphIndex(a1a, currentUnicodeChar);
						fontGlyph = &a1a->glyphs[FontGlyphIndex];
						kerningIndex = fontGlyph->kerningStartIndex;
						kerningEnd = fontGlyph[1].kerningStartIndex;
						if (kerningIndex >= kerningEnd)
						{
						LABEL_16:
							float_4 = 0.0;
						}
						else
						{
							while (a1a->kerningInfos[kerningIndex].otherChar != v45)
							{
								kerningIndex++;
								if (kerningIndex >= kerningEnd)
									goto LABEL_16;
							}
							float_4 = a1a->kerningInfos[kerningIndex].kerningDistance;
						}
						v45 = currentUnicodeChar;
						v72 = (float)(sizeX[currentStyleId] * float_4) + v51;
						v51 = (float)(sizeX[currentStyleId] * fontGlyph->float_0) + v72;
						if (!v49)
						{
							if (currentUnicodeChar == ' ')
							{
								v129 = 1;
							}
							else if (currentUnicodeChar == '\n')
							{
								data.textLines.push_back({stringUnicodeLength,v35,0});
								v51 = 0.0;


								v129 = 0;
								v44 = fmaxf(v44, v35);
								fVar23 = fVar23 + (float)(v58 + v52);
								v35 = 0.0;
							}
							else
							{
								uint32_t bitIndex = (v129 + 2 * (fontGlyph->byte_6 * (fontAtlas->unk_2 + 1)));
								if ((fontAtlas->unk_18[bitIndex >> 3] & (1 << (bitIndex & 7))) != 0)
								{
									v75 = data.inlineImages.size();
									v39 = v35;
									v76 = stringUnicodeLength;
									v110 = stringUnicodeLength;
									v108 = v75;
									v40 = v72;
								}
								else
								{
									v75 = v108;
									v76 = v110;
								}
								if (v51 > v57)
								{
									v77 = v75;
									v78 = v58 + v52;

									for (uint32_t i = v75; i < data.inlineImages.size(); i++) {
										data.inlineImages[i].afloat_4[0] -= v40;
										data.inlineImages[i].afloat_4[1] += v78;
										data.inlineImages[i].afloat_4[2] -= v40;
										data.inlineImages[i].afloat_4[3] += v78;
									}
									data.textLines.push_back({stringUnicodeLength,v35,0});
									v51 = v51 - v40;
									fVar23 = fVar23 + v78;
									v108 = data.inlineImages.size();
									v44 = fmaxf(v44, v39);
								}
								v129 = 0;
								v35 = v51;

							}
						}
					}
					else
					{
						if (v62 == '%')
						{
							++currentCharPointer;
							goto LABEL_12;
						}

						v64 = (char*)sub_F98F0(
							&currentCharPointer,
							(__int64)v124,
							printText);
						if (!v64)
							goto LABEL_57;
						v65 = currentCharPointer;
						currentCharPointer = v64;
						savedTextPointer[savedTextPointerIndex] = v65;
						savedTextPointerIndex++;
					}
				}
				if (currentUnicodeChar)
					break;
				if (!(int)savedTextPointerIndex)
					goto LABEL_57;
				savedTextPointerIndex--;
				currentCharPointer = (char*)savedTextPointer[(unsigned int)savedTextPointerIndex];
			}
			if (currentUnicodeChar == '`')
				break;
			if ((unsigned int)(currentUnicodeChar - 0xF0000) >= 0x2000)
			{
				if (currentUnicodeChar == 0xF2001 && setInlineImageData)
				{
					v93 = fVar23;
					for (i = fVar23;
						_BitScanForward((unsigned long*)&v96, v130);
						i = fmaxf(i, v97 + sizeY[v96]))
					{
						v130 = (v130 - 1) & v130;
						v97 = fVar23 - *(float*)&v116[v96];
						v93 = fminf(v93, v97);
					}
					InlineImage_t& img = data.inlineImages[data.inlineImages.size()-1];
					img.afloat_4[3] = i;
					img.afloat_4[2] = v51;
					img.afloat_4[1] = v93;
					setInlineImageData = false;
					v35 = v51;

					v129 = 0;
				}
				v45 = currentUnicodeChar;
			}
			else
			{
				//v86 = v84 + 4 * (v84 + 653);
				uint32_t curAsset = ((uint32_t*)&currentUnicodeChar)[1];
				InlineImage_t img;
				img.hash = curAsset;
				img.styleId = currentStyleId;
				img.afloat_4[0] = v51;
				setInlineImageData = true;
				data.inlineImages.push_back(img);
				v130 = (1LL << currentStyleId);

				assetIndex = (__int16)imageAssetMap[curAsset].imageIndex;
				v88 = &imageAtlases[imageAssetMap[curAsset].atlasIndex].dimentions[assetIndex];
				if (v45 == 0xF2000)
				{
					if ((unsigned __int16)assetIndex >= imageAtlases[imageAssetMap[curAsset].atlasIndex].renderOffsets.size())
						goto LABEL_49;

					v89 = (float*)&imageAtlases[imageAssetMap[curAsset].atlasIndex].renderOffsets[assetIndex];

					v129 = 0;
					v45 = curAsset;
					v51 = v51 + (float)((float)(v89[2] + *v89) * (float)v88->width);
					v35 = v51;
				}
				else
				{
					v90 = (float)v88->width / (float)v88->height;
					v91 = fVar23 - *(float*)&v116[currentStyleId];
					{
						InlineImage_t& i = data.inlineImages[data.inlineImages.size() - 1];
						i.afloat_4[2] = v91;
						v92 = v90 * sizeX[currentStyleId];
						i.afloat_4[3] = v91 + sizeY[currentStyleId];
						v51 = v51 + v92;
						i.afloat_4[2] = v92 + i.afloat_4[0];
						setInlineImageData = false;
					}
				LABEL_49:

					v129 = 0;
					v35 = v51;
					v45 = curAsset;
				}
			}
		}
		currentStyleId = *currentCharPointer - 48;
		if (currentStyleId >= 4u)
			break;
		v45 = 0;
		++currentCharPointer;
		a1a = v125[currentStyleId];
		v130 = (1LL << currentStyleId) | v130;
	}
LABEL_57:

	if (data.textLines.size())
		data.lastLine = v51;

	v101 = _mm_set_ps(0,0,0,fVar23+v52);
	v102 = fmaxf(v44, v51);
	v104 = _mm_set_ps(0, 0, 0, data.maxSize.value.y);

	v104.m128_f32[0] = v104.m128_f32[0] / fmaxf(v104.m128_f32[0], v102);
	data.float_0 = v104.m128_f32[0];


	v104.m128_f32[0] = v104.m128_f32[0] * v102;

	result = _mm_shuffle_ps(v104, v101, 0);

	return result;
}
