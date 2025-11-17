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
		ruiVertex.position[0] = quad.vert[i][0];
		ruiVertex.position[1] = quad.vert[i][1];
		ruiVertex.position[2] = 0;

		ruiVertex.float_C[0] = quad.vert[i][0];
		ruiVertex.float_C[1] = quad.vert[i][1];
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
	ImageAtlas *imageAtlas; // r13
	__int16 v24; // r14
	uint16_t assetIndex; // cx
	uiImageAtlasUnk *v35; // rcx
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
	__m128 v72; // xmm0
	__m128 v74; // xmm4
	__m128 v75; // xmm0
	DrawInfo *drawInfo; // rcx
	__m128 v79; // xmm2
	__m128 v80; // xmm3
	__m128 *v81; // r9
	__m128 v84; // xmm0
	__m128 v86; // xmm5
	__m128 v87; // xmm0
	__m128 v92; // xmm2
	__m128 v93; // xmm3
	__m128 *v94; // r9
	__m128 v97; // xmm0
	__m128 v99; // xmm5
	__m128 v100; // xmm0
	__m128 v107; // xmm2
	__m128 v108; // xmm3
	__m128 *v109; // r9
	__m128 v110; // xmm5
	__m128 v112; // xmm0
	__m128 v113; // xmm0
	__m128 v114; // xmm5
	__m128 v115; // xmm0
	__m128 v119; // xmm3
	__m128 v120; // xmm4
	__m128 v123; // xmm0
	__m128 v124; // xmm6
	__m128 v128; // xmm2
	__m128 v129; // xmm3
	__m128 *v130; // r9
	__m128 v133; // xmm0
	__m128 v135; // xmm5
	__m128 v136; // xmm0
	char v139; // al
	__m128 v141; // xmm2
	__m128 v142; // xmm3
	__m128 *v143; // r9
	__m128 v146; // xmm0
	__m128 v148; // xmm5
	__m128 v149; // xmm0
	__m128 v152; // xmm2
	__m128 v153; // xmm15
	__m128 v155; // xmm2
	__m128 v156; // xmm3
	__m128 *v157; // r9
	__m128 v160; // xmm0
	__m128 v162; // xmm5
	__m128 v163; // xmm0
	__m128 v167; // xmm2
	__m128 v168; // xmm3
	__m128 *v169; // r9
	__m128 v172; // xmm0
	__m128 v174; // xmm5
	__m128 v175; // xmm0
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
		}
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



void RenderInstance::StartFrame(float time) {
	segments.clear();
	verts.clear();
	indices.clear();
	transformResults.clear();
	static uint64_t transformHashes[3] = {randomInt64(),randomInt64(),randomInt64()};
	transformResults.push_back(TransformResult(_mm_set_ps(1,0,0,1),_mm_setzero_ps(),_mm_set_ps(elementHeight,elementHeight,elementWidth,elementWidth),transformHashes[0]));
	transformResults.push_back(TransformResult(_mm_set_ps(1,0,0,1),_mm_setzero_ps(),_mm_set_ps(elementHeight,elementHeight,elementWidth,elementWidth),transformHashes[1]));
	globals.currentTime = time;
	float v17 = elementHeightRpc;
	v17 = v17 * elementHeight;
	float v18 = elementWidth;
	v18 = v18 * elementWidthRpc;
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
	transformResults.push_back(TransformResult(directionVector,position,inputSize,transformHashes[2]));
	styleDescriptor.clear();
	g_renderFramework->RuiClearFrame();
}



void RenderInstance::EndFrame() {

	std::sort(jobs.begin(), jobs.end(), [](RenderJob& a, RenderJob& b) {
		return a.layer < b.layer;
	});

	for (auto& job : jobs) {
		job.func(*this);
	}
	jobs.clear();

	g_renderFramework->RuiBindPipeline();

	g_renderFramework->RuiWriteIndexBuffer(indices);
	g_renderFramework->RuiWriteStyleBuffer(styleDescriptor);
	g_renderFramework->RuiWriteVertexBuffer(verts);
	

	for (size_t i = 0; i < segments.size(); i++) {
		size_t endIndex;
		if ((i + 1) == segments.size()) {

			endIndex = indices.size();
		}
		else {
			endIndex = segments[i+1].indexStart;
		}
		size_t resources[5];
		memset(resources,0xFF,sizeof(resources));
		if (segments[i].fontAtlas) {
			resources[0] = segments[i].fontAtlas->textureId;
			resources[2] = segments[i].fontAtlas->shaderDataId;
		}
		if (segments[i].imageAtlas) {
			resources[1] = segments[i].imageAtlas->textureId;
			resources[3] = segments[i].imageAtlas->shaderDataId;
		}
		g_renderFramework->DrawIndexed((uint32_t)(endIndex-segments[i].indexStart),(uint32_t)segments[i].indexStart,resources);
	}


}