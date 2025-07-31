#include "RenderManager.h"
#include <fstream>
#include "Util.h"

void RenderInstance::AddQuad(RenderQuad& quad) {
	indices.push_back((uint16_t)verts.size()+0);
	indices.push_back((uint16_t)verts.size()+1);
	indices.push_back((uint16_t)verts.size()+2);
	indices.push_back((uint16_t)verts.size()+2);
	indices.push_back((uint16_t)verts.size()+3);
	indices.push_back((uint16_t)verts.size()+0);

	for (int i = 0; i < 4; i++) {
		__m128 v18 = _mm_set1_ps(quad.vert[i][0]);
		__m128 v19 = _mm_set1_ps(quad.vert[i][1]);
		Vertex_t ruiVertex;
		ruiVertex.position[0] = quad.vert[i][0] * elementWidth;
		ruiVertex.position[1] = quad.vert[i][1] * elementHeight;
		ruiVertex.position[2] = 0;

		ruiVertex.float_C[0] = quad.vert[i][0] * elementWidth;
		ruiVertex.float_C[1] = quad.vert[i][1] * elementHeight;
		ruiVertex.float_C[2] = 0;
		__m128 a = _mm_add_ps(
			_mm_add_ps(_mm_mul_ps(quad.xUvVector, v18), _mm_mul_ps(v19, quad.yUvVector)),
			quad.UvBase);
		ruiVertex.float_18[0] = a.m128_f32[0];
		ruiVertex.float_18[1] = a.m128_f32[1];
		ruiVertex.float_18[2] = a.m128_f32[2];
		ruiVertex.float_18[3] = a.m128_f32[3];
		__m128 b = quad.m128_50;
		ruiVertex.float_28[0] = b.m128_f32[0];
		ruiVertex.float_28[1] = b.m128_f32[1];
		ruiVertex.assetIndex = quad.assetIndex;
		ruiVertex.assetIndex2 = quad.assetIndex2;
		ruiVertex.word_34 = quad.styleDescriptorIndex;
		ruiVertex.word_36 = quad.flags;
		verts.push_back(ruiVertex);
	}

}


void __fastcall RenderInstance::sub_FEF30( __m128 *a3, __m128 *a4, TriData& a5)
{


	__m128 unk3Float_10; // xmm10
	__m128 unk3screenHeight; // xmm11
	__m128 unk3Float_18; // xmm13
	__m128 unk3ScreenWidth; // xmm7
	__m128 unk3Float_4; // xmm8
	__m128 unk3Float_8; // xmm9
	__m128 v16;
	__m128 v17;
	__m128 v72;
	__m128 v19;
	__m128 v20;
	__m128 v25;
	__m128 v26;
	__m128 v30;
	__m128 v32;
	__m128 v33;
	__m128 v77;
	__m128 v73;
	__m128 v36;
	__m128 v37;
	__m128 v59;
	__m128 v62;
	__m128 v63;
	__m128 v65;
	__m128 v66;
	__m128 v67;
	__m128 v68;
	__m128 v69;
	__m128 v70;
	__m128 v71;







	unk3Float_10 = _mm_set1_ps(drawInfo.ruiUnk3[0].float_10);
	unk3screenHeight = _mm_set1_ps(drawInfo.ruiUnk3[0].screenHeight);
	unk3Float_18 = _mm_set1_ps(drawInfo.ruiUnk3[0].float_18);
	unk3ScreenWidth = _mm_set1_ps(drawInfo.ruiUnk3[0].screenWidth);
	unk3Float_4 = _mm_set1_ps(drawInfo.ruiUnk3[0].float_4);
	unk3Float_8 = _mm_set1_ps(drawInfo.ruiUnk3[0].float_8);
	v16 = _mm_add_ps(
		_mm_add_ps(
			_mm_set1_ps(drawInfo.float_10),
			_mm_mul_ps(unk3ScreenWidth, a5.a)),
		_mm_mul_ps(unk3Float_10, a5.b));
	v17 = _mm_add_ps(
		_mm_add_ps(
			_mm_set1_ps(drawInfo.float_14),
			_mm_mul_ps(unk3Float_4, a5.a)),
		_mm_mul_ps(unk3screenHeight, a5.b));
	v72 = _mm_add_ps(
		_mm_add_ps(
			_mm_set1_ps(drawInfo.float_18),
			_mm_mul_ps(unk3Float_8, a5.a)),
		_mm_mul_ps(unk3Float_18, a5.b));

	v19 = (__m128)_mm_shuffle_ps(*a4,*a4, _MM_SHUFFLE(1,1,0,0));
	v20 = (__m128)_mm_shuffle_ps(*a4,*a4, _MM_SHUFFLE(2,3,3,2));



	v25 = _mm_add_ps(_mm_mul_ps(a3[0], v19), _mm_mul_ps(a3[2], v20));
	v26 = _mm_mul_ps(a3[1], v19);

	v30 = _mm_mul_ps(a3[3], v20);

	v32 = _mm_add_ps(v26, v30);

	v33 = _mm_mul_ps(unk3Float_18, v32);
	v77 = _mm_add_ps(_mm_mul_ps(unk3ScreenWidth, v25), _mm_mul_ps(unk3Float_10, v32));

	v73 = _mm_add_ps(_mm_mul_ps(unk3Float_8, v25), v33);
	v36 = _mm_mul_ps(unk3screenHeight, v32);
	v37 = _mm_mul_ps(unk3Float_4, v25);


	v33.m128_f32[0] = 1 * 2.0;


	v65 = a5.b;
	v66 = a5.a;
	v67 = _mm_shuffle_ps(v65,v65, _MM_SHUFFLE(0,3,2,1));
	v68 = _mm_sub_ps(v67, (__m128)v65);
	v69 = _mm_shuffle_ps(v66,v66, _MM_SHUFFLE(0,3,2,1));
	v70 = _mm_sub_ps(v69, (__m128)v66);
	v71 = _mm_cmple_ps(
		_mm_mul_ps(
			_mm_sub_ps(
				_mm_mul_ps((__m128)_mm_shuffle_ps(v68,v68, _MM_SHUFFLE(2,1,0,3)), (__m128)v70),
				_mm_mul_ps((__m128)_mm_shuffle_ps(v70,v70, _MM_SHUFFLE(2,1,0,3)), (__m128)v68)),
			_mm_sub_ps(_mm_mul_ps(a3[3], a3[0]), _mm_mul_ps(a3[1], a3[2]))),
		_mm_setzero_ps());
	a5.a = _mm_or_ps(_mm_andnot_ps(v71, (__m128)v66), _mm_and_ps(v71, v69));
	a5.b = _mm_or_ps(_mm_andnot_ps(v71, (__m128)v65), _mm_and_ps(v71, v67));
}

void RenderInstance::sub_FFAE0(__m128 a1,__m128 a2, __m128* a3)
{
	__m128 v3; // xmm1
	__m128 v4; // xmm1
	__m128 v5; // xmm1
	__m128 v6; // xmm1
	__m128 v7; // xmm2
	__m128 v8; // xmm3

	v3 = _mm_shuffle_ps(a2, _mm_setzero_ps(), _MM_SHUFFLE(1, 0, 0, 0));
	v4 = _mm_mul_ps(_mm_unpacklo_ps(v3, v3), a1);
	v5 = _mm_mul_ps(v4, v4);
	v6 = _mm_max_ps(_mm_set1_ps(1.1754944e-38f), _mm_add_ps((__m128)_mm_shuffle_ps(v5, v5, _MM_SHUFFLE(2,3,0,1)), v5));
	v7 = _mm_rsqrt_ps(v6);
	v8 = _mm_mul_ps(
		_mm_mul_ps(_mm_sub_ps(_mm_set1_ps(1.5f), _mm_mul_ps(_mm_mul_ps(_mm_mul_ps(_mm_set1_ps(0.5f), v6), v7), v7)), v7),
		a1);
	a3[0] = _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(0, 0, 0, 0));
	a3[1] = _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(1, 1, 1, 1));
	a3[2] = _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(2, 2, 2, 2));
	a3[3] = _mm_shuffle_ps(v8, v8, _MM_SHUFFLE(3, 3, 3, 3));
	a3[4] = _mm_set1_ps(0.41421357f);
}

void RenderInstance::generateDrawTriangle(RenderQuad* v180,bool v24,__m128 a8,__m128* a3a, int a6,const TransformResult& a5, __m128* triangleType ) {
	
	__m128 v27 = _mm_shuffle_ps(a8,a8, _MM_SHUFFLE(1,3,3,1));
	__m128 v28 = _mm_shuffle_ps(a8,a8, _MM_SHUFFLE(2,2,0,0));
	
	TriData v184= a5.GenTri(v27,v28);

	if ( v24 )
	{
		sub_FEF30(  a3a, triangleType, v184);
	}

	__m128 v32 = _mm_unpackhi_ps(v184.a, v184.b);
	__m128 v33 = _mm_unpacklo_ps(v184.a, v184.b);
	if ( a6 == 2 )
	{
		v33 = _mm_shuffle_ps(v33,v33, _MM_SHUFFLE(1,0,3,2));
		v32 = _mm_shuffle_ps(v32,v32, _MM_SHUFFLE(1,0,3,2));
	}

	*(__m128 *)&v180->vert[0][0] = v33;
	*(__m128 *)&v180->vert[2][0] = v32;
}

__m128 xmmword_12A146A0a = _mm_castsi128_ps(_mm_set_epi32(0,0,0xFFFFFFFF,0));
__m128 xmmword_12A146D0a = _mm_castsi128_ps(_mm_set_epi32(0,0,0,0xFFFFFFFF));
__m128 xmmword_12A14650a = _mm_castsi128_ps(_mm_set_epi32(0,0,0xFFFFFFFF,0xFFFFFFFF));

__m128 stru_5F4740a[] = {
	_mm_setr_ps(0.0,0.0,0.0,0.0),
	_mm_setr_ps(-1.0, 0.0, 0.0, 0.0),
	_mm_setr_ps(0.0, 1.0, 0.0, 0.0),
	_mm_setr_ps(-1.0, 1.0, 0.0, 0.0),
	_mm_setr_ps(0.0, 0.0, -1.0, 0.0),
	_mm_setr_ps(-1.0, 0.0, -1.0, 0.0),
	_mm_setr_ps(0.0, 1.0, -1.0, 0.0),
	_mm_setr_ps(-1.0, 1.0, -1.0, 0.0),
	_mm_setr_ps(0.0, 0.0, 0.0, 1.0),
	_mm_setr_ps(-1.0, 0.0, 0.0, 1.0),
	_mm_setr_ps(0.0, 1.0, 0.0, 1.0),
	_mm_setr_ps(-1.0, 1.0, 0.0, 1.0),
	_mm_setr_ps(0.0, 0.0, -1.0, 1.0),
	_mm_setr_ps(-1.0, 0.0, -1.0, 1.0),
	_mm_setr_ps(0.0, 1.0, -1.0, 1.0),
	_mm_setr_ps(-1.0, 1.0, -1.0, 1.0)
};

__m128 xmm_12A4E830a[] = {
	_mm_castsi128_ps(_mm_set1_epi32(0xFFFFFFFF)),
	_mm_castsi128_ps(_mm_set_epi32(0,0xFFFFFFFF,0,0xFFFFFFFF)),
	_mm_castsi128_ps(_mm_set_epi32(0xFFFFFFFF,0,0xFFFFFFFF,0)),
	_mm_setzero_ps(),
};



void RenderInstance::sub_F9B80_rev(
	__m128 ruiSize,
	RenderQuad &a4,
	const TransformResult& a5,
	int a6,
	Asset_t &a7,
	const __m128& a8,
	const __m128& a9,
	const __m128& texMin,
	const __m128& texSize)
{
	__int64 unsigned_int_8; // rax

	__int64 v17; // rdx
	ImageAtlas *imageAtlas; // r13
	ImageAtlas *uiImageAtlas_10; // rax
	uint32_t vertexIndicesElementCount; // eax
	int dword_0; // eax
	int word_66; // r14d
	__int16 v24; // r14
	uint16_t assetIndex; // cx

	__m128 v27; // xmm2
	__m128 v28; // xmm3
	__m128 v29; // xmm5

	__m128 v31; // xmm0
	__m128 v32; // xmm5
	__m128 v33; // xmm0

	uiImageAtlasUnk_ *v35; // rcx
	__m128 v37; // xmm0
	__m128 v38; // xmm1
	__m128 v39; // xmm5
	__m128 v40; // xmm13
	__m128 v41; // xmm4
	__m128 v42; // xmm6
	__m128 texSizeRcp; // xmm10
	__m128 v44; // xmm1
	__m128 v45; // xmm1
	__m128 v46; // xmm9
	__m128 v47; // xmm1
	__m128 v48; // xmm3
	__m128 v49; // xmm2
	__m128 v50; // xmm0
	__m128 v51; // xmm15
	__m128 v52; // xmm12
	__m128 v53; // xmm7
	__m128 v54; // xmm1
	__m128 v55; // xmm11
	__m128 v56; // xmm8
	__m128 v57; // xmm10
	__m128 v58; // xmm14
	__m128 v59; // xmm13
	__m128 v60; // xmm0
	__m128 v61; // xmm9
	__m128 v62; // xmm7
	__m128 v63; // xmm9
	__m128 v64; // xmm13
	char v65; // al

	__m128 v67; // xmm1
	__m128 v68; // xmm2
	__m128 *v69; // r9
	__m128 v70; // xmm4
	__m128 v71; // xmm5
	__m128 v72; // xmm0
	__m128 v73; // xmm0
	__m128 v74; // xmm4
	__m128 v75; // xmm0
	__int64 v76; // rax
	DrawInfo *drawInfo; // rcx

	__m128 v79; // xmm2
	__m128 v80; // xmm3
	__m128 *v81; // r9
	__m128 v82; // xmm5
	__m128 v83; // xmm6
	__m128 v84; // xmm0
	__m128 v85; // xmm0
	__m128 v86; // xmm5
	__m128 v87; // xmm0
	__int64 v88; // rax
	DrawInfo *v89; // rcx
	char v90; // al
	__m128i v91; // xmm1
	__m128 v92; // xmm2
	__m128 v93; // xmm3
	__m128 *v94; // r9
	__m128 v95; // xmm5
	__m128 v96; // xmm6
	__m128 v97; // xmm0
	__m128 v98; // xmm0
	__m128 v99; // xmm5
	__m128 v100; // xmm0
	__int16 word_60; // ax
	__int16 word_62; // eax^2



	__m128i v106; // xmm1
	__m128 v107; // xmm2
	__m128 v108; // xmm3
	__m128 *v109; // r9
	__m128 v110; // xmm5
	__m128 v111; // xmm6
	__m128 v112; // xmm0
	__m128 v113; // xmm0
	__m128 v114; // xmm5
	__m128 v115; // xmm0


	__m128i v118; // xmm2
	__m128 v119; // xmm3
	__m128 v120; // xmm4
	__m128 v121; // xmm6
	__m128 v122; // xmm1
	__m128 v123; // xmm0
	__m128 v124; // xmm6
	DrawInfo *v126; // rcx
	__m128i v127; // xmm1
	__m128 v128; // xmm2
	__m128 v129; // xmm3
	__m128 *v130; // r9
	__m128 v131; // xmm5
	__m128 v132; // xmm6
	__m128 v133; // xmm0
	__m128 v134; // xmm0
	__m128 v135; // xmm5
	__m128 v136; // xmm0
	__int64 v137; // rax

	char v139; // al
	__m128i v140; // xmm1
	__m128 v141; // xmm2
	__m128 v142; // xmm3
	__m128 *v143; // r9
	__m128 v144; // xmm5
	__m128 v145; // xmm6
	__m128 v146; // xmm0
	__m128 v147; // xmm0
	__m128 v148; // xmm5
	__m128 v149; // xmm0

	DrawInfo *v151; // rcx
	__m128 v152; // xmm2
	__m128 v153; // xmm15
	__m128i v154; // xmm1
	__m128 v155; // xmm2
	__m128 v156; // xmm3
	__m128 *v157; // r9
	__m128 v158; // xmm5
	__m128 v159; // xmm6
	__m128 v160; // xmm0
	__m128 v161; // xmm0
	__m128 v162; // xmm5
	__m128 v163; // xmm0
	__int64 v164; // rax

	__m128i v166; // xmm1
	__m128 v167; // xmm2
	__m128 v168; // xmm3
	__m128 *v169; // r9
	__m128 v170; // xmm5
	__m128 v171; // xmm6
	__m128 v172; // xmm0
	__m128 v173; // xmm0
	__m128 v174; // xmm5
	__m128 v175; // xmm0


	__m128 v178[2]; // [rsp+30h] [rbp-D0h] BYREF
	//__m128 v179; // [rsp+40h] [rbp-C0h]
	//ruiDrawTriangle v180; // [rsp+50h] [rbp-B0h] BYREF
	//ruiUnknownStruct_1 v181; // [rsp+80h] [rbp-80h] BYREF
	RenderQuad quad;

	int v182; // [rsp+F0h] [rbp-10h]
	int v183; // [rsp+F4h] [rbp-Ch]
	__m128 v184[2]; // [rsp+100h] [rbp+0h] BYREF
	// [rsp+110h] [rbp+10h]
	__m128 a3a[5]; // [rsp+120h] [rbp+20h] BYREF
	int v187; // [rsp+250h] [rbp+150h]




	//unsigned_int_8 = a3->unsigned_int_8;
	//qword_0 = a3->qword_0;
	//v17 = unsigned_int_8;
	imageAtlas = &imageAtlases[a7.atlasIndex];
	AddImageAtlasSegment(imageAtlas);


	v24 = (~a4.flags >> 8) & 0xF;
	if ( v24 )
		sub_FFAE0(a5.directionVector, ruiSize, a3a);
	assetIndex = a7.imageIndex;
	quad = a4;
	if (assetIndex >= imageAtlas->renderOffsets.size()) {
		generateDrawTriangle(&quad,v24!=0,a8,a3a,a6,a5,&stru_5F4740a[v24]);
		AddQuad(quad);
		return;
	}

	v35 = &imageAtlas->renderOffsets[(__int16)assetIndex];


	v38 = _mm_mul_ps(_mm_set_ps(0,0,1,1),ruiSize);
	v39 = _mm_xor_ps(v35->m128_0, _mm_set_ps(-0.0,-0.0,0,0));
	v40 = _mm_and_ps(v35->m128_0, (__m128)xmmword_12A14650a);
	v41 = _mm_add_ps(_mm_shuffle_ps(v35->m128_0,v35->m128_0, _MM_SHUFFLE(3,2,3,2)), v39);
	v42 = _mm_sub_ps(_mm_set1_ps(1), v41);
	texSizeRcp = NRReciprocal(texSize);
	v44 = _mm_mul_ps(
		_mm_mul_ps(_mm_unpacklo_ps(v38, v38), _mm_shuffle_ps(a5.directionVector,a5.directionVector, _MM_SHUFFLE(3,1,2,0))),
		texSizeRcp);
	v45 = _mm_mul_ps(v44, v44);
	v46 = _mm_max_ps(
		_mm_mul_ps(
			_mm_sqrt_ps(_mm_add_ps((__m128)_mm_shuffle_ps(v45,v45, 78), (__m128)v45)),
			_mm_shuffle_ps(v35->m128_10,v35->m128_10,_MM_SHUFFLE(1,0,1,0))),
		_mm_shuffle_ps(v35->m128_10,v35->m128_10,_MM_SHUFFLE(3,2,3,2)));
	v47 = _mm_sub_ps(v46, v41);
	v50 = _mm_movelh_ps(v46, _mm_set1_ps(1));
	v51 = _mm_mul_ps(v50, a4.UvBase);
	v52 = _mm_mul_ps(v50, a4.yUvVector);
	v184[0] = _mm_add_ps(_mm_sub_ps(_mm_set1_ps(1), v50), v51);
	v53 = NRReciprocal(v47);
	v54 = _mm_movelh_ps(_mm_mul_ps(_mm_mul_ps(v42, v46), v53), _mm_set1_ps(1));
	v55 = _mm_mul_ps(v54, a4.xUvVector);
	v56 = _mm_mul_ps(
		_mm_sub_ps(_mm_add_ps(_mm_mul_ps((__m128)_mm_shuffle_ps(v53,v53, 238), v39), _mm_set_ps(1,1,0,0)), texMin),
		texSizeRcp);
	v57 = _mm_mul_ps(v50, a4.xUvVector);
	v58 = _mm_mul_ps(v54, a4.yUvVector);
	v59 = _mm_sub_ps(v40, _mm_mul_ps(_mm_mul_ps(v40, v42), (__m128)v53));
	v60 = _mm_shuffle_ps(v56,v56, _MM_SHUFFLE(3,1,2,0));
	v61 = _mm_shuffle_ps(a8,a8, _MM_SHUFFLE(3,1,2,0));
	v62 = _mm_unpackhi_ps(v61, v60);
	v63 = _mm_unpacklo_ps(v61, v60);
	v64 = _mm_add_ps(v59, _mm_mul_ps(v54, a4.UvBase));
	v65 = _mm_movemask_ps(_mm_cmple_ps(a9, _mm_xor_ps((__m128)v56, _mm_set_ps(-0.0,-0.0,0,0))));

	v187 = _mm_movemask_ps(_mm_cmple_ps(a9, _mm_xor_ps((__m128)_mm_shuffle_ps(v56,v56, 78),_mm_set_ps(-0.0,-0.0,0,0))));
	if ( (v65 & 3) == 0 )
	{

		v67 = (__m128)_mm_shuffle_ps(v62,v62, 20);
		v68 = (__m128)_mm_shuffle_ps(v63,v63, 80);
		v69 = &stru_5F4740a[v24 & 5];
		TriData v178 = a5.GenTri(v67,v68);
		v72 = _mm_cmpneq_ps(*v69, _mm_setzero_ps());
		if ( _mm_movemask_ps(v72) )
		{
			sub_FEF30(  a3a, v69, v178);

		}
		v74 = _mm_unpackhi_ps(v178.a, v178.b);
		v75 = _mm_unpacklo_ps(v178.a, v178.b);
		if ( a6 == 2 )
		{
			v75 = _mm_shuffle_ps(v75,v75, 78);
			v74 = _mm_shuffle_ps(v75,v75, 78);
		}
		//v76 = *(_QWORD *)&a4->assetIndex;

		quad.UvBase = v51;
		quad.xUvVector = v57;
		quad.yUvVector = v52;
		quad.m128_30 = _mm_setzero_ps();
		quad.m128_40 = _mm_setzero_ps();
		quad.m128_50 = _mm_setzero_ps();
		quad.assetIndex = a4.assetIndex;
		quad.assetIndex2 = a4.assetIndex2;
		quad.styleDescriptorIndex = a4.styleDescriptorIndex;
		quad.flags = a4.flags;
		*(__m128 *)&quad.vert[0][0] = v75;
		*(__m128 *)&quad.vert[2][0] = v74;

		AddQuad(quad);


	}
	v183 = v187 & 5;
	if ( (v183 | v65 & 2) == 0)
	{
		v79 = (__m128)_mm_shuffle_ps(v62, v62, 20);
		v80 = (__m128)_mm_shuffle_ps(v63, v63, 245);
		v81 = &stru_5F4740a[v24 & 4];
		TriData v178 = a5.GenTri(v79,v80);
		v84 = _mm_cmpneq_ps(_mm_setzero_ps(), *v81);

		if (_mm_movemask_ps(v84))
		{
			sub_FEF30(a3a, v81, v178);
		}
		v86 = _mm_unpackhi_ps(v178.a, v178.b);
		v87 = _mm_unpacklo_ps(v178.a, v178.b);
		if (a6 == 2)
		{
			v87 = _mm_shuffle_ps(v87, v87, 78);
			v86 = _mm_shuffle_ps(v86, v86, 78);
		}
		quad.UvBase = _mm_or_ps(_mm_andnot_ps((__m128)xmmword_12A146D0a, v51), _mm_and_ps(v64, (__m128)xmmword_12A146D0a));
		quad.xUvVector = _mm_or_ps(_mm_andnot_ps((__m128)xmmword_12A146D0a, v57), _mm_and_ps(v55, (__m128)xmmword_12A146D0a));
		quad.yUvVector = _mm_or_ps(_mm_andnot_ps((__m128)xmmword_12A146D0a, v52), _mm_and_ps(v58, (__m128)xmmword_12A146D0a));
		quad.m128_30 = _mm_setzero_ps();
		quad.m128_40 = _mm_setzero_ps();
		quad.m128_50 = _mm_setzero_ps();
		quad.assetIndex = a4.assetIndex;
		quad.assetIndex2 = a4.assetIndex2;
		quad.styleDescriptorIndex = a4.styleDescriptorIndex;
		quad.flags = a4.flags;
		*(__m128*)& quad.vert[0][0] = v87;
		*(__m128*)& quad.vert[2][0] = v86;
		AddQuad(quad);
	}

	if ( (v65 & 6) == 0 )
	{

		v92 = (__m128)_mm_shuffle_ps(v62,v62, 20);
		v93 = (__m128)_mm_shuffle_ps(v63,v63, 175);
		v94 = &stru_5F4740a[v24 & 6];
		TriData v178 = a5.GenTri(v92,v93);
		v97 = _mm_cmpneq_ps(*v94, _mm_setzero_ps());
		if ( _mm_movemask_ps(v97) )
		{
			sub_FEF30(  a3a, v94, v178);

		}
		v99 = _mm_unpackhi_ps(v178.a, v178.b);
		v100 = _mm_unpacklo_ps(v178.a, v178.b);
		if ( a6 == 2 )
		{
			v100 = _mm_shuffle_ps(v100,v100, 78);
			v99 = _mm_shuffle_ps(v99,v99, 78);
		}



		quad.xUvVector = v57;
		quad.yUvVector = v52;
		quad.UvBase = _mm_or_ps(
			_mm_andnot_ps((__m128)xmmword_12A146D0a, v51),
			_mm_and_ps(v184[0], (__m128)xmmword_12A146D0a));
		quad.m128_30 = _mm_setzero_ps();
		quad.m128_40 = _mm_setzero_ps();
		quad.m128_50 = _mm_setzero_ps();
		quad.assetIndex = a4.assetIndex;
		quad.assetIndex2 = a4.assetIndex2;
		quad.styleDescriptorIndex = a4.styleDescriptorIndex;
		quad.flags = a4.flags;
		*(__m128 *)&quad.vert[0][0] = v100;
		*(__m128 *)&quad.vert[2][0] = v99;


		AddQuad(quad);


	}
	v182 = v187 & 0xA;
	if ( (v182 | v65 & 1) == 0 )
	{
		v107 = (__m128)_mm_shuffle_ps(v62, v62, _MM_SHUFFLE(1, 3, 3, 1));
		v108 = (__m128)_mm_shuffle_ps(v63, v63, _MM_SHUFFLE(1, 1, 0, 0));
		v109 = &stru_5F4740a[v24 & 1];
		TriData tri = a5.GenTri(v107,v108);
		v112 = _mm_cmpneq_ps(*v109, _mm_setzero_ps());

		if (_mm_movemask_ps(v112))
		{
			sub_FEF30(a3a, v109, tri);

		}
		v113 = v110;
		v114 = _mm_unpackhi_ps(tri.a, tri.b);
		v115 = _mm_unpacklo_ps(tri.a, tri.b);
		if (a6 == 2)
		{
			v115 = _mm_shuffle_ps(v115, v115, _MM_SHUFFLE(1, 0, 3, 2));
			v114 = _mm_shuffle_ps(v114, v114, _MM_SHUFFLE(1, 0, 3, 2));
		}



		quad.m128_30 = _mm_setzero_ps();
		quad.m128_40 = _mm_setzero_ps();
		quad.m128_50 = _mm_setzero_ps();
		quad.assetIndex = a4.assetIndex;
		quad.assetIndex2 = a4.assetIndex2;
		quad.styleDescriptorIndex = a4.styleDescriptorIndex;
		quad.flags = a4.flags;
		quad.UvBase = _mm_or_ps(_mm_andnot_ps((__m128)xmmword_12A146A0a, v51), _mm_and_ps(v64, (__m128)xmmword_12A146A0a));
		quad.xUvVector = _mm_or_ps(_mm_andnot_ps((__m128)xmmword_12A146A0a, v57), _mm_and_ps(v55, (__m128)xmmword_12A146A0a));
		quad.yUvVector = _mm_or_ps(_mm_andnot_ps((__m128)xmmword_12A146A0a, v52), _mm_and_ps(v58, (__m128)xmmword_12A146A0a));
		*(__m128*)& quad.vert[0][0] = v115;
		*(__m128*)& quad.vert[2][0] = v114;
		AddQuad(quad);
	}

	if ( v187 == 0 )		
	{
		v119 = (__m128)_mm_shuffle_ps(v62, v62, _MM_SHUFFLE(1, 3, 3, 1));
		v120 = (__m128)_mm_shuffle_ps(v63, v63, _MM_SHUFFLE(3, 3, 1, 1));
		TriData tri = a5.GenTri(v119,v120);
		v123 = _mm_unpacklo_ps(tri.a, tri.b);
		v124 = _mm_unpackhi_ps(tri.a, tri.b);
		if (a6 == 2)
		{
			v123 = _mm_shuffle_ps(v123, v123, 78);
			v124 = _mm_shuffle_ps(v124, v124, 78);
		}



		quad.UvBase = v64;
		quad.yUvVector = v58;
		quad.xUvVector = v55;
		quad.m128_30 = _mm_setzero_ps();
		quad.m128_40 = _mm_setzero_ps();
		quad.m128_50 = _mm_setzero_ps();
		quad.assetIndex = a4.assetIndex;
		quad.assetIndex2 = a4.assetIndex2;
		quad.styleDescriptorIndex = a4.styleDescriptorIndex;
		quad.flags = a4.flags;
		*(__m128*)& quad.vert[2][0] = v124;
		*(__m128*)& quad.vert[0][0] = v123;
		AddQuad(quad);
	}
	if ( (v182 | v65 & 4)==0 )
	{
		v128 = (__m128)_mm_shuffle_ps(v62, v62, 125);
		v129 = (__m128)_mm_shuffle_ps(v63, v63, 175);
		v130 = &stru_5F4740a[v24 & 2];
		TriData tri = a5.GenTri(v128,v129);
		v133 = _mm_cmpneq_ps(_mm_setzero_ps(), *v130);
		if (_mm_movemask_ps(v133))
		{
			sub_FEF30(a3a, v130, tri);
		}
		v135 = (__m128)_mm_unpackhi_ps(tri.a, tri.b);
		v136 = (__m128)_mm_unpacklo_ps(tri.a, tri.b);
		if (a6 == 2)
		{
			v136 = _mm_shuffle_ps(v136, v136, 78);
			v135 = _mm_shuffle_ps(v135, v135, 78);
		}

		quad.UvBase = _mm_or_ps(
			_mm_andnot_ps((__m128)xmmword_12A146A0a, v184[0]),
			_mm_and_ps(v64, (__m128)xmmword_12A146A0a));
		quad.xUvVector = _mm_or_ps(
			_mm_andnot_ps((__m128)xmmword_12A146A0a, v57),
			_mm_and_ps(v55, (__m128)xmmword_12A146A0a));
		quad.yUvVector = _mm_or_ps(
			_mm_andnot_ps((__m128)xmmword_12A146A0a, v52),
			_mm_and_ps(v58, (__m128)xmmword_12A146A0a));
		quad.m128_30 = _mm_setzero_ps();
		quad.m128_40 = _mm_setzero_ps();
		quad.m128_50 = _mm_setzero_ps();
		quad.assetIndex = a4.assetIndex;
		quad.assetIndex2 = a4.assetIndex2;
		quad.styleDescriptorIndex = a4.styleDescriptorIndex;
		quad.flags = a4.flags;

		*(__m128*)& quad.vert[0][0] = v136;
		*(__m128*)& quad.vert[2][0] = v135;
		AddQuad(quad);
	}


	v139 = v65;
	if ( (v65 & 9) != 0 )
	{
		v153 = v184[0];
	}
	else
	{

		v141 = (__m128)_mm_shuffle_ps(v62,v62, 235);
		v142 = (__m128)_mm_shuffle_ps(v63,v63, 80);
		v143 = &stru_5F4740a[v24 & 9];
		TriData tri = a5.GenTri(v141,v142);
		v146 = _mm_cmpneq_ps(*v143, _mm_setzero_ps());

		if ( _mm_movemask_ps(v146) )
		{
			sub_FEF30(a3a, v143, tri);
		}

		v148 = _mm_unpackhi_ps(tri.a, tri.b);
		v149 = _mm_unpacklo_ps(tri.a, tri.b);
		if ( a6 == 2 )
		{
			v149 = _mm_shuffle_ps(v149,v149, 78);
			v148 = _mm_shuffle_ps(v148,v148, 78);
		}



		v152 = _mm_andnot_ps((__m128)xmmword_12A146A0a, v51);
		v153 = v184[0];
		quad.xUvVector = v57;
		quad.yUvVector = v52;

		quad.UvBase = _mm_or_ps(v152, _mm_and_ps(v184[0], (__m128)xmmword_12A146A0a));
		quad.m128_30 = _mm_setzero_ps();
		quad.m128_40 = _mm_setzero_ps();
		quad.m128_50 = _mm_setzero_ps();
		quad.assetIndex = a4.assetIndex;
		quad.assetIndex2 = a4.assetIndex2;
		quad.styleDescriptorIndex = a4.styleDescriptorIndex;
		quad.flags = a4.flags;
		*(__m128 *)&quad.vert[0][0] = v149;
		*(__m128 *)&quad.vert[2][0] = v148;
		AddQuad(quad);
		v139 = v65;
	}
	if (( v183 | v139 & 8)==0 )
	{
		v155 = (__m128)_mm_shuffle_ps(v62, v62, 235);
		v156 = (__m128)_mm_shuffle_ps(v63, v63, 245);
		v157 = &stru_5F4740a[v24 & 8];
		TriData tri = a5.GenTri(v155,v156);
		v160 = _mm_cmpneq_ps(*v157, _mm_setzero_ps());

		if (_mm_movemask_ps(v160))
		{
			sub_FEF30(a3a, v157, tri);
		}
		v161 = v158;
		v162 = _mm_unpackhi_ps(tri.a, tri.b);
		v163 = _mm_unpacklo_ps(tri.a, tri.b);
		if (a6 == 2)
		{
			v163 = _mm_shuffle_ps(v163, v163, 78);
			v162 = _mm_shuffle_ps(v162, v162, 78);
		}

		quad.UvBase = _mm_or_ps(
			_mm_andnot_ps((__m128)xmmword_12A146D0a, v153),
			_mm_and_ps(v64, (__m128)xmmword_12A146D0a));
		quad.xUvVector = _mm_or_ps(
			_mm_andnot_ps((__m128)xmmword_12A146D0a, v57),
			_mm_and_ps(v55, (__m128)xmmword_12A146D0a));
		quad.yUvVector = _mm_or_ps(
			_mm_andnot_ps((__m128)xmmword_12A146D0a, v52),
			_mm_and_ps(v58, (__m128)xmmword_12A146D0a));
		quad.m128_30 = _mm_setzero_ps();
		quad.m128_40 = _mm_setzero_ps();
		quad.m128_50 = _mm_setzero_ps();
		quad.assetIndex = a4.assetIndex;
		quad.assetIndex2 = a4.assetIndex2;
		quad.styleDescriptorIndex = a4.styleDescriptorIndex;
		quad.flags = a4.flags;


		*(__m128*)& quad.vert[0][0] = v163;
		*(__m128*)& quad.vert[2][0] = v162;
		AddQuad(quad);
	}

	if ( (v65 & 0xC) == 0 )
	{
		//v166 = _mm_load_si128((const __m128i *)a5);
		v167 = (__m128)_mm_shuffle_ps(v62,v62, 235);
		v168 = (__m128)_mm_shuffle_ps(v63,v63, 175);
		v169 = &stru_5F4740a[v24 & 0xA];
		TriData tri = a5.GenTri(v167,v168);
		v172 = _mm_cmpneq_ps(_mm_setzero_ps(), *v169);

		if ( _mm_movemask_ps(v172) )
		{
			sub_FEF30(a3a, v169, tri);
			v171 = v178[1];
			v170 = v178[0];
		}
		v173 = v170;
		v174 = _mm_unpackhi_ps(tri.a, tri.b);
		v175 = _mm_unpacklo_ps(tri.a, tri.b);
		if ( a6 == 2 )
		{
			v175 = _mm_shuffle_ps(v175,v175, 78);
			v174 = _mm_shuffle_ps(v174,v174, 78);
		}

		quad.UvBase = v153;
		quad.xUvVector = v57;
		quad.yUvVector = v52;
		quad.m128_30 = _mm_setzero_ps();
		quad.m128_40 = _mm_setzero_ps();
		quad.m128_50 = _mm_setzero_ps();
		quad.assetIndex = a4.assetIndex;
		quad.assetIndex2 = a4.assetIndex2;
		quad.styleDescriptorIndex = a4.styleDescriptorIndex;
		quad.flags = a4.flags;
		*(__m128 *)&quad.vert[0][0] = v175;
		*(__m128 *)&quad.vert[2][0] = v174;
		AddQuad(quad);
	}
}




void RenderInstance::initBuffers_v30() {
	D3D11_BUFFER_DESC bufferInit{};
	bufferInit.ByteWidth = 576;
	bufferInit.Usage = D3D11_USAGE_DEFAULT;
	bufferInit.BindFlags = 4;
	bufferInit.CPUAccessFlags = 0;
	bufferInit.MiscFlags = 0;
	bufferInit.StructureByteStride = 0;
	device->CreateBuffer(&bufferInit, 0i64, &commonPerCameraBuffer);
	bufferInit.ByteWidth = 208;
	bufferInit.Usage = D3D11_USAGE_DYNAMIC;
	bufferInit.BindFlags = 4;
	bufferInit.CPUAccessFlags = 0x10000;
	bufferInit.MiscFlags = 0;
	bufferInit.StructureByteStride = 0;
	device->CreateBuffer(&bufferInit, 0i64, &modelInstanceBuffer);



	CBufCommonPerCamera cam{};

	cam.c_cameraRelativeToClip.a.x = 0.0010416672f;
	cam.c_cameraRelativeToClip.a.w = -1.0f;
	cam.c_cameraRelativeToClip.b.y = -0.00185185182f;
	cam.c_cameraRelativeToClip.b.w = 1.0f;
	cam.c_cameraRelativeToClip.c.z = -0.5f;
	cam.c_cameraRelativeToClip.c.w = 0.5f;
	cam.c_cameraRelativeToClip.d.w = 1.0f;
	cam.c_cameraRelativeToClipPrevFrame.a.x = 1.0f;
	cam.c_cameraRelativeToClipPrevFrame.b.y = 1.0f;
	cam.c_cameraRelativeToClipPrevFrame.c.z = 1.0f;
	cam.c_cameraRelativeToClipPrevFrame.d.w = 1.0f;
	cam.c_envMapLightScale = 1.0f;
	cam.c_renderTargetSize.x = 1920.0f;
	cam.c_renderTargetSize.y = 1080.0f;
	cam.c_rcpRenderTargetSize.x = 0.000520833360f;
	cam.c_rcpRenderTargetSize.y = 0.000925925910f;
	cam.c_numCoverageSamples = 1.0f;
	cam.c_rcpNumCoverageSamples = 1.0f;
	cam.c_cloudRelConst.x = 0.5f;
	cam.c_cloudRelConst.y = 0.5f;
	cam.c_useRealTimeLighting = 1.0f;
	cam.c_maxLightingValue = 5.0f;
	cam.c_viewportMaxZ = 1.0f;
	cam.c_viewportScale.x = 1.0f;
	cam.c_viewportScale.y = 1.0f;
	cam.c_rcpViewportScale.x = 1.0f;
	cam.c_rcpViewportScale.y = 1.0f;
	cam.c_framebufferViewportScale.x = 1.0f;
	cam.c_framebufferViewportScale.y = 1.0f;
	cam.c_rcpFramebufferViewportScale.x = 1.0f;
	cam.c_rcpFramebufferViewportScale.y = 1.0f;
	deviceContext->UpdateSubresource(commonPerCameraBuffer, 0, 0, &cam, sizeof(cam), sizeof(cam));
	D3D11_MAPPED_SUBRESOURCE map;
	deviceContext->Map(modelInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	ModelInstance* inst = (ModelInstance*)map.pData;
	memset(inst, 0, sizeof(ModelInstance));
	inst->objectToCameraRelative.a.x = 1.0f;
	inst->objectToCameraRelative.b.y = 1.0f;
	inst->objectToCameraRelative.c.z = 1.0f;
	inst->objectToCameraRelativePrevFrame.a.x = 1.0f;
	inst->objectToCameraRelativePrevFrame.b.y = 1.0f;
	inst->objectToCameraRelativePrevFrame.c.z = 1.0f;
	inst->diffuseModulation.x = 0.999999940f;
	inst->diffuseModulation.y = 0.999999940f;
	inst->diffuseModulation.z = 0.999999940f;
	inst->diffuseModulation.w = 1.0f;
	deviceContext->Unmap(modelInstanceBuffer, 0);


	bufferInit.ByteWidth = sizeof(uint16_t) * 0x6000;
	bufferInit.Usage = D3D11_USAGE_DYNAMIC;
	bufferInit.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferInit.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferInit.MiscFlags = 0;
	bufferInit.StructureByteStride = 0;
	device->CreateBuffer(&bufferInit, 0, &indexBuffer);
	bufferInit.ByteWidth = sizeof(Vertex_t) * 0x4000;
	bufferInit.Usage = D3D11_USAGE_DYNAMIC;
	bufferInit.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferInit.MiscFlags = 0;
	bufferInit.StructureByteStride = 0;
	device->CreateBuffer(&bufferInit, 0, &vertexBuffer);
	bufferInit.ByteWidth = sizeof(StyleDescriptorShader_t) * 0x200;
	bufferInit.Usage = D3D11_USAGE_DYNAMIC;
	bufferInit.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bufferInit.MiscFlags = 0x40;
	bufferInit.StructureByteStride = sizeof(StyleDescriptorShader_t);
	device->CreateBuffer(&bufferInit, 0, &styleDescBuffer);
	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewInit{};
	resourceViewInit.Format = DXGI_FORMAT_UNKNOWN;
	resourceViewInit.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
	resourceViewInit.Buffer.FirstElement = 0;
	resourceViewInit.Buffer.NumElements = 0x200;
	device->CreateShaderResourceView(styleDescBuffer, &resourceViewInit, &styleDescriptorResourceView);
	D3D11_SAMPLER_DESC samplerInit{};
	samplerInit.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samplerInit.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerInit.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerInit.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerInit.MipLODBias = 0.0f;
	samplerInit.MaxAnisotropy = 0;
	samplerInit.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerInit.BorderColor[0] = 0.0f;
	samplerInit.BorderColor[1] = 0.0f;
	samplerInit.BorderColor[2] = 0.0f;
	samplerInit.BorderColor[3] = 0.0f;
	samplerInit.MinLOD = 0.0f;
	samplerInit.MaxLOD = 3.40282347e+38f;
	device->CreateSamplerState(&samplerInit, &samplerState);

	D3D11_BLEND_DESC blendDesc;
	memset(&blendDesc,0,sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	device->CreateBlendState(&blendDesc,&blendState);


	std::vector<char> vertexByteCode;

	std::ifstream vertexFile{"./Assets/Shader//ui_vs.fxc",std::ios::binary};
	vertexFile.seekg(0,std::ios::end);
	vertexByteCode.resize(vertexFile.tellg());
	vertexFile.seekg(0);
	vertexFile.read(vertexByteCode.data(),vertexByteCode.size());
	vertexFile.close();



	HRESULT res = device->CreateVertexShader(vertexByteCode.data(), vertexByteCode.size(), 0, &vertexShader);
	if (res) {
		printf("Errror creating Vertex Shader\n");
		return;
	}
	ID3DBlob *inputSignatureBlob;
	D3DGetBlobPart(vertexByteCode.data(), vertexByteCode.size(), D3D_BLOB_INPUT_SIGNATURE_BLOB, 0, &inputSignatureBlob);

	D3D11_INPUT_ELEMENT_DESC elementDescriptors[4];
	memset(elementDescriptors,0,sizeof(elementDescriptors));

	elementDescriptors[0].SemanticName = "POSITION";
	elementDescriptors[0].SemanticIndex = 0;
	elementDescriptors[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elementDescriptors[0].InputSlot = 0;
	elementDescriptors[0].AlignedByteOffset = 0;
	elementDescriptors[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elementDescriptors[0].InstanceDataStepRate = 0;

	elementDescriptors[1].SemanticName = "TEXCOORD";
	elementDescriptors[1].SemanticIndex = 1;
	elementDescriptors[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elementDescriptors[1].InputSlot = 0;
	elementDescriptors[1].AlignedByteOffset = 0x18;
	elementDescriptors[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elementDescriptors[1].InstanceDataStepRate = 0;

	elementDescriptors[2].SemanticName = "TEXCOORD";
	elementDescriptors[2].SemanticIndex = 2;
	elementDescriptors[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	elementDescriptors[2].InputSlot = 0;
	elementDescriptors[2].AlignedByteOffset = 0x28;
	elementDescriptors[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elementDescriptors[2].InstanceDataStepRate = 0;

	elementDescriptors[3].SemanticName = "TEXCOORD";
	elementDescriptors[3].SemanticIndex = 3;
	elementDescriptors[3].Format = DXGI_FORMAT_R16G16B16A16_SINT;
	elementDescriptors[3].InputSlot = 0;
	elementDescriptors[3].AlignedByteOffset = 0x30;
	elementDescriptors[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elementDescriptors[3].InstanceDataStepRate = 0;


	device->CreateInputLayout( elementDescriptors, 4, inputSignatureBlob->GetBufferPointer(), inputSignatureBlob->GetBufferSize(), &shaderLayout);

	//pixel shader

	std::vector<char> pixelByteCode;
	std::ifstream pixelFile{"./Assets/Shader/ui_ps.fxc",std::ios::binary};
	pixelFile.seekg(0,std::ios::end);
	pixelByteCode.resize(pixelFile.tellg());
	pixelFile.seekg(0);
	pixelFile.read(pixelByteCode.data(),pixelByteCode.size());
	pixelFile.close();

	res = device->CreatePixelShader(pixelByteCode.data(), pixelByteCode.size(),0,&pixelShader);
	if (res) {
		printf("Error creating Pixel shader\n");
		return;
	}

	D3D11_TEXTURE2D_DESC texture_desc = {};
	texture_desc.Width = 1920;
	texture_desc.Height = 1080;
	texture_desc.MipLevels = 1;
	texture_desc.ArraySize = 1;
	texture_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texture_desc.SampleDesc.Count = 1;
	texture_desc.SampleDesc.Quality = 0;
	texture_desc.Usage = D3D11_USAGE_DEFAULT;
	texture_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texture_desc.CPUAccessFlags = 0;
	texture_desc.MiscFlags = 0;

	device->CreateTexture2D(&texture_desc, 0, &targetTexture);
	
	D3D11_RENDER_TARGET_VIEW_DESC target_view_desc = {};
	target_view_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	target_view_desc.Texture2D.MipSlice = 0;

	device->CreateRenderTargetView(targetTexture, &target_view_desc, &targetView);

	// Depth stencil
	D3D11_TEXTURE2D_DESC depth_texture_desc = {};
	depth_texture_desc.Width = 1920;
	depth_texture_desc.Height = 1080;
	depth_texture_desc.MipLevels = 1;
	depth_texture_desc.ArraySize = 1;
	depth_texture_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_texture_desc.SampleDesc.Count = 1;
	depth_texture_desc.SampleDesc.Quality = 0;
	depth_texture_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;


	device->CreateTexture2D(&depth_texture_desc, nullptr, &depthTexture);
	device->CreateDepthStencilView(depthTexture, nullptr, &depthStencil);

	D3D11_SHADER_RESOURCE_VIEW_DESC view_desc = {};
	view_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	view_desc.Texture2D.MostDetailedMip = 0;
	view_desc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(targetTexture, &view_desc, &targetResourceView);

	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = false;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	device->CreateRasterizerState(&rasterDesc, &rasterState);

	viewport.Width = (float)1920;
	viewport.Height = (float)1080;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xff;
	depthStencilDesc.StencilWriteMask = 0xff;

	// stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);


}

void RenderInstance::StartFrame(float time) {
	segments.clear();
	verts.clear();
	indices.clear();
	transformResults.clear();
	transformResults.push_back(TransformResult(0,_mm_set_ps(1,0,0,1),_mm_setzero_ps(),_mm_set_ps(elementHeight,elementHeight,elementWidth,elementWidth)));
	transformResults.push_back(TransformResult(1,_mm_set_ps(1,0,0,1),_mm_setzero_ps(),_mm_set_ps(elementHeight,elementHeight,elementWidth,elementWidth)));
	globals.currentTime = time;
	float v17 = elementHeightRatio;
	v17 = v17 * elementHeight;
	float v18 = elementWidth;
	v18 = v18 * elementWidthRatio;
	if (v18 <= v17)
	{
		v18 = v18 / v17;
		v17 = 1;
	}
	else
	{
		v17 = v17 / v18;
		v18 = 1;
	}
	__m128 v19 = _mm_set_ps(elementHeight, elementHeight, elementWidth, elementWidth);
	__m128 directionVector = _mm_set_ps(v17, 0, 0, v18);
	__m128 position = _mm_sub_ps(
		_mm_set1_ps(0.5),
		_mm_mul_ps(_mm_shuffle_ps(directionVector, directionVector, _MM_SHUFFLE(3, 0, 3, 0)), _mm_set1_ps(0.5)));
	__m128 inputSize = _mm_mul_ps(v19, _mm_shuffle_ps(directionVector, directionVector, _MM_SHUFFLE(3, 3, 0, 0)));
	transformResults.push_back(TransformResult(2,directionVector,position,inputSize));
	//transformSizes.clear();
	//transformSizes.resize(0x200);
	styleDescriptor.clear();
}



void RenderInstance::EndFrame() {
	float color[] = {.1f,.1f,.1f,1.f};
	deviceContext->ClearRenderTargetView(targetView,color);
	deviceContext->ClearDepthStencilView(depthStencil,D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	deviceContext->OMSetDepthStencilState(depthStencilState, 1);
	deviceContext->RSSetState(rasterState);
	deviceContext->RSSetViewports(1,&viewport);
	if(indices.size()==0)return;
	deviceContext->OMSetRenderTargets(1,&targetView,depthStencil);
	deviceContext->OMSetBlendState(blendState,NULL,0xFFFFFFFF);

	deviceContext->VSSetConstantBuffers(2i64, 1i64, &commonPerCameraBuffer);
	deviceContext->PSSetConstantBuffers(2i64, 1i64, &commonPerCameraBuffer);

	deviceContext->VSSetConstantBuffers(3i64, 1i64, &modelInstanceBuffer);
	deviceContext->PSSetConstantBuffers(3i64, 1i64, &modelInstanceBuffer);


	D3D11_MAPPED_SUBRESOURCE map;
	deviceContext->Map(vertexBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&map);
	memcpy(map.pData,verts.data(),verts.size()*sizeof(Vertex_t));
	deviceContext->Unmap(vertexBuffer,0);

	//deviceContext->UpdateSubresource(indexBuffer,0,0,indices.data(),sizeof(uint16_t),sizeof(uint16_t)*indices.size());
	deviceContext->Map(indexBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&map);
	memcpy(map.pData,indices.data(),indices.size()*sizeof(uint16_t));
	deviceContext->Unmap(indexBuffer,0);
	
	//deviceContext->UpdateSubresource(styleDescBuffer,0,0,styleDescriptor.data(),sizeof(StyleDescriptorShader_t),sizeof(StyleDescriptorShader_t)*styleDescriptor.size());
	deviceContext->Map(styleDescBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&map);
	memcpy(map.pData,styleDescriptor.data(),styleDescriptor.size()*sizeof(StyleDescriptorShader_t));
	deviceContext->Unmap(styleDescBuffer,0);
	
	deviceContext->IASetInputLayout(shaderLayout);
	deviceContext->VSSetShader(vertexShader,0,0);
	deviceContext->PSSetShader(pixelShader,0,0);
	deviceContext->PSSetSamplers(0,1,&samplerState);
	deviceContext->IASetIndexBuffer(indexBuffer,DXGI_FORMAT_R16_UINT,0);
	UINT strides = 56;
	UINT offsets = 0;
	deviceContext->IASetVertexBuffers(0,1,&vertexBuffer,&strides,&offsets);
	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (size_t i = 0; i < segments.size(); i++) {
		uint32_t endIndex;
		if ((i + 1) == segments.size()) {
			endIndex = indices.size();
		}
		else {
			endIndex = segments[i+1].indexStart;
		}
		ID3D11ShaderResourceView* resources[5];
		memset(resources,0,sizeof(resources));
		if (segments[i].fontAtlas) {
			resources[0] = segments[i].fontAtlas->imageResourceView;
			resources[2] = segments[i].fontAtlas->boundsResourceView;
		}
		if (segments[i].imageAtlas) {
			resources[1] = segments[i].imageAtlas->imageResourceView;
			resources[3] = segments[i].imageAtlas->boundsResourceView;
		}
		resources[4] = styleDescriptorResourceView;
		deviceContext->PSSetShaderResources(0,5,resources);
		deviceContext->DrawIndexed(endIndex - segments[i].indexStart,segments[i].indexStart,0);
	}


}