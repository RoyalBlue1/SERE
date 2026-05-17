#version 450

layout(location = 0) in vec3  in_position;   // POSITION
layout(location = 1) in vec4  in_texcoord1;  // TEXCOORD1
layout(location = 2) in vec2  in_texcoord2;  // TEXCOORD2
layout(location = 3) in ivec4 in_texcoord3;  // TEXCOORD3

layout(location = 0) out vec4       out_texcoord0;  // TEXCOORD
layout(location = 1) out vec2       out_texcoord1;  // TEXCOORD1
layout(location = 2) flat out ivec4 out_texcoord2;  // TEXCOORD2 (flat = no interp)

struct c_fogParams_t
{
    vec4 k0;
    vec4 k1;
    vec4 k2;
    vec4 k3;
    vec4 k4;
};

struct c_csm_t
{
    vec3 shadowRelConst;
    uint enableShadows;
    vec3 shadowRelForX;
    float unused_1;
    vec3 shadowRelForY;
    float cascadeWeightScale;
    vec3 shadowRelForZ;
    float cascadeWeightBias;
    vec4 laterCascadeScale;
    vec4 laterCascadeBias;
    vec2 normToAtlasCoordsScale0;
    vec2 normToAtlasCoordsBias0;
    vec4 normToAtlasCoordsScale12;
    vec4 normToAtlasCoordsBias12;
};

struct c_modelInst_lighting_t
{
    vec4 ambientSH[3];
    vec4 skyDirSunVis;
    uint packedLightData;
};

struct CBufCommonPerCamera_t
{
    float c_zNear;
    vec3 c_cameraOrigin;
    mat4x4 c_cameraRelativeToClip;
    int c_frameNum;
    vec3 c_cameraOriginPrevFrame;
    mat4x4 c_cameraRelativeToClipPrevFrame;
    vec4 c_clipPlane;
    c_fogParams_t c_fogParams;
    vec3 c_skyColor;
    float c_shadowBleedFudge;
    float c_envMapLightScale;
    vec3 c_sunColor;
    vec3 c_sunDir;
    float c_gameTime;
    c_csm_t c_csm;
    uint c_lightTilesX;
    float c_minShadowVariance;
    vec2 c_renderTargetSize;
    vec2 c_rcpRenderTargetSize;
    float c_numCoverageSamples;
    float c_rcpNumCoverageSamples;
    vec2 c_cloudRelConst;
    vec2 c_cloudRelForX;
    vec2 c_cloudRelForY;
    vec2 c_cloudRelForZ;
    float c_sunHighlightSize;
    uint c_globalLightingFlags;
    uint c_useRealTimeLighting;
    float c_forceExposure;
    int c_debugInt;
    float c_debugFloat;
    float c_maxLightingValue;
    float c_viewportMaxZ;
    vec2 c_viewportScale;
    vec2 c_rcpViewportScale;
    vec2 c_framebufferViewportScale;
    vec2 c_rcpFramebufferViewportScale;
};

layout(std430,binding = 2) readonly buffer Uni2 {
    CBufCommonPerCamera_t CBufCommonPerCamera;
};

struct CBufModelInstance_t {
    mat3x4 objectToCameraRelative;
    mat3x4 objectToCameraRelativePrevFrame;
    vec4 diffuseModulation;
    int cubemapID;
    int lightmapID;
    vec2 unused;
    c_modelInst_lighting_t lighting;
};

layout(std430,binding = 3) readonly buffer Uni3 {
    CBufModelInstance_t CBufModelInstance;
};


void main() {
    out_texcoord0 = in_texcoord1;
    out_texcoord1 = in_texcoord2;
    out_texcoord2 = in_texcoord3;
    mat4x3 objectToCameraRelative = transpose(CBufModelInstance.objectToCameraRelative);
    mat4x4 cameraRelativeToClip = transpose(CBufCommonPerCamera.c_cameraRelativeToClip);
    vec4 pos = vec4(in_position, 1.0);

    vec4 cameraRelative = vec4(
        objectToCameraRelative * pos,
        1.0
    );

    gl_Position = cameraRelativeToClip * cameraRelative;
}