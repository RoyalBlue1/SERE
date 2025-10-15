#pragma once

#include <filesystem>
#include "Util.h"
#include "ShaderStructs.h"
namespace fs = std::filesystem;

struct Vertex_t
{
	float position[3];
	float float_C[3];
	float float_18[4];

	float float_28[2];
	uint16_t assetIndex;
	uint16_t assetIndex2;
	uint16_t word_34;
	uint16_t word_36;
};



struct StyleDescriptorShader_t
{
	Color color0 = Color(1.f,1.f,1.f,1.f);
	Color color1 = Color(0.f,0.f,0.f,0.f);
	Color color2 = Color(0.f,0.f,0.f,0.f);
	float blend = 1.f;
	float premul = 0.f;
	float _anon_0 = 0.f;
	float _anon_1 = 0.f;
	float _anon_2 = 0.f;
	float _anon_3 = 0.f;
	float _anon_4 = 0.f;
	float _anon_5 = 0.f;
	float _anon_6 = 0.f;
	uint8_t gap_54[12];

};

struct ShaderSizeData_t {
	float minX;
	float minY;
	float sizeX;
	float sizeY;
};


class RenderFramework {

public:

	RenderFramework() {};

	virtual bool ShouldMainLoopRun() = 0;

	virtual bool ImGuiStartFrame() = 0;
	virtual void ImGuiEndFrame() = 0;

	virtual void ImGuiDeInit() = 0;

	virtual size_t LoadTexture(fs::path& path) = 0;
	virtual size_t CreateShaderDataBuffer(std::vector<ShaderSizeData_t> data) = 0;

	virtual void RuiWriteIndexBuffer(std::vector<uint16_t>& data) = 0;
	virtual void RuiWriteVertexBuffer(std::vector<Vertex_t>& data) = 0;
	virtual void RuiWriteStyleBuffer(std::vector<StyleDescriptorShader_t>& data) = 0;

	virtual void RuiClearFrame() = 0;


	virtual void DrawIndexed(uint32_t count,uint32_t start,size_t* resources) = 0;

	virtual void RuiBindPipeline() = 0;
	virtual void RuiCreatePipeline(Vector2 size) = 0;

	virtual void* GetTextureView(size_t id) = 0;
	virtual void* GetRuiView() = 0;

};

