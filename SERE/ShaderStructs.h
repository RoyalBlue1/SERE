#pragma once

typedef unsigned int uint;

struct float2 {
	float x;
	float y;
};

struct float3 {
	float x;
	float y;
	float z;
};

struct float4 {
	float x;
	float y;
	float z;
	float w;
};

struct row_major_float4x4 {
	float4 a;
	float4 b;
	float4 c;
	float4 d;
};

struct row_major_float3x4 {
	float4 a;
	float4 b;
	float4 c;

};

struct FogUnion
{

	float4 k0;                     // Offset:  176
	float4 k1;                     // Offset:  192
	float4 k2;                     // Offset:  208
	float4 k3;                     // Offset:  224
	float4 k4;                     // Offset:  240

};
static_assert(sizeof(FogUnion) == 80);

struct CSMConstants_t
{

	float3 shadowRelConst;         // Offset:  304
	bool enableShadows;            // Offset:  316
	float3 shadowRelForX;          // Offset:  320
	float unused_1;                // Offset:  332
	float3 shadowRelForY;          // Offset:  336
	float cascadeWeightScale;      // Offset:  348
	float3 shadowRelForZ;          // Offset:  352
	float cascadeWeightBias;       // Offset:  364
	float4 laterCascadeScale;      // Offset:  368
	float4 laterCascadeBias;       // Offset:  384
	float2 normToAtlasCoordsScale0;// Offset:  400
	float2 normToAtlasCoordsBias0; // Offset:  408
	float4 normToAtlasCoordsScale12;// Offset:  416
	float4 normToAtlasCoordsBias12;// Offset:  432

};
static_assert(sizeof(CSMConstants_t) == 144);

struct CBufCommonPerCamera
{

	float c_zNear;                     // Offset:    0 Size:     4 [unused]
	float3 c_cameraOrigin;             // Offset:    4 Size:    12 [unused]
	row_major_float4x4 c_cameraRelativeToClip;// Offset:   16 Size:    64
	int c_frameNum;                    // Offset:   80 Size:     4 [unused]
	float3 c_cameraOriginPrevFrame;    // Offset:   84 Size:    12 [unused]
	row_major_float4x4 c_cameraRelativeToClipPrevFrame;// Offset:   96 Size:    64 [unused]
	float4 c_clipPlane;                // Offset:  160 Size:    16 [unused]

	FogUnion c_fogParams;                     // Offset:  176 Size:    80 [unused]
	float3 c_skyColor;                 // Offset:  256 Size:    12 [unused]
	float c_shadowBleedFudge;          // Offset:  268 Size:     4 [unused]
	float c_envMapLightScale;          // Offset:  272 Size:     4 [unused]
	float3 c_sunColor;                 // Offset:  276 Size:    12 [unused]
	float3 c_sunDir;                   // Offset:  288 Size:    12 [unused]
	float c_gameTime;                  // Offset:  300 Size:     4 [unused]

	CSMConstants_t c_csm;                           // Offset:  304 Size:   144 [unused]
	uint c_lightTilesX;                // Offset:  448 Size:     4 [unused]
	float c_minShadowVariance;         // Offset:  452 Size:     4 [unused]
	float2 c_renderTargetSize;         // Offset:  456 Size:     8 [unused]
	float2 c_rcpRenderTargetSize;      // Offset:  464 Size:     8 [unused]
	float c_numCoverageSamples;        // Offset:  472 Size:     4 [unused]
	float c_rcpNumCoverageSamples;     // Offset:  476 Size:     4 [unused]
	float2 c_cloudRelConst;            // Offset:  480 Size:     8 [unused]
	float2 c_cloudRelForX;             // Offset:  488 Size:     8 [unused]
	float2 c_cloudRelForY;             // Offset:  496 Size:     8 [unused]
	float2 c_cloudRelForZ;             // Offset:  504 Size:     8 [unused]
	float c_sunHighlightSize;          // Offset:  512 Size:     4 [unused]
	uint c_globalLightingFlags;        // Offset:  516 Size:     4 [unused]
	uint c_useRealTimeLighting;        // Offset:  520 Size:     4 [unused]
	float c_forceExposure;             // Offset:  524 Size:     4 [unused]
	int c_debugInt;                    // Offset:  528 Size:     4 [unused]
	float c_debugFloat;                // Offset:  532 Size:     4 [unused]
	float c_maxLightingValue;          // Offset:  536 Size:     4 [unused]
	float c_viewportMaxZ;              // Offset:  540 Size:     4 [unused]
	float2 c_viewportScale;            // Offset:  544 Size:     8 [unused]
	float2 c_rcpViewportScale;         // Offset:  552 Size:     8 [unused]
	float2 c_framebufferViewportScale; // Offset:  560 Size:     8 [unused]
	float2 c_rcpFramebufferViewportScale;// Offset:  568 Size:     8 [unused]

};
static_assert(sizeof(CBufCommonPerCamera) == 576);

struct ModelInstanceLighting
{

	float4 ambientSH[3];       // Offset:  128
	float4 skyDirSunVis;       // Offset:  176
	uint packedLightData[4];     // Offset:  192

};



struct ModelInstance
{

	row_major_float3x4 objectToCameraRelative;// Offset:    0
	row_major_float3x4 objectToCameraRelativePrevFrame;// Offset:   48
	float4 diffuseModulation;      // Offset:   96
	int cubemapID;                 // Offset:  112
	int lightmapID;                // Offset:  116
	float2 unused;                 // Offset:  120

	ModelInstanceLighting lighting;                    // Offset:  128

};                     // Offset:    0 Size:   208

static_assert(sizeof(ModelInstance) == 208);