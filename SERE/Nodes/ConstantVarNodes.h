#pragma once

#include "RuiNodeEditor.h"

void AddConstantVarNodes(NodeEditor& editor);

class IntVarNode : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Integer Constant";
	static inline std::string category = "Constant";
private:
	RenderInstance& proto;
	int value;
	int minVal;
	int maxVal;
public:
	explicit IntVarNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class BoolVarNode : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Bool Constant";
	static inline std::string category = "Constant";
private:
	RenderInstance& proto;
	bool value;
public:
	explicit BoolVarNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class FloatVarNode : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Float Constant";
	static inline std::string category = "Constant";
private:
	RenderInstance& proto;
	float value;
	float minVal;
	float maxVal;
public:
	explicit FloatVarNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class Float2VarNode : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Vector2 Constant";
	static inline std::string category = "Constant";
private:
	RenderInstance& proto;
	float minVal;
	float maxVal;
	float value[2];
public:
	explicit Float2VarNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class Float3VarNode : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Vector3 Constant";
	static inline std::string category = "Constant";
private:
	RenderInstance& proto;
	float minVal;
	float maxVal;
	float value[3];
public:
	explicit Float3VarNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class ColorVarNode : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Color Constant";
	static inline std::string category = "Constant";
private:
	RenderInstance& proto;
	float value[4];
public:
	explicit ColorVarNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class StringVarNode : public ImFlow::BaseNode
{
public:
	static inline std::string name = "String Constant";
	static inline std::string category = "Constant";
private:
	RenderInstance& proto;
	std::string value;
public:
	explicit StringVarNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class AssetVarNode : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Asset Constant";
	static inline std::string category = "Constant";
private:
	RenderInstance& proto;
	uint32_t hash;
	bool showSelectionUi;
public:
	explicit AssetVarNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

class SizeVarNode : public ImFlow::BaseNode
{
public:
	static inline std::string name = "Size Constant";
	static inline std::string category = "Constant";
private:
	RenderInstance& proto;
	float minVal;
	float maxVal;
	float value[4];
public:
	explicit SizeVarNode(RenderInstance& prot,NodeStyles& styles);
	void draw() override;

	static std::vector<PinInfo> GetPinInfo();
};

