#pragma once
#include <vector>
#include <memory>
#include <string>
#include <map>

#include "imgui/ImNodeFlow.h"
#include "RenderManager.h"




struct NodeStyles {
	std::shared_ptr<ImFlow::PinStyle> transformResult;
	std::shared_ptr<ImFlow::PinStyle> transformSize;

	std::shared_ptr<ImFlow::PinStyle> intVariable;
	std::shared_ptr<ImFlow::PinStyle> boolVariable;
	std::shared_ptr<ImFlow::PinStyle> floatVariable;
	std::shared_ptr<ImFlow::PinStyle> float2Variable;
	std::shared_ptr<ImFlow::PinStyle> float3Variable;
	std::shared_ptr<ImFlow::PinStyle> colorVariable;
	std::shared_ptr<ImFlow::PinStyle> stringVariable;
	std::shared_ptr<ImFlow::PinStyle> assetVariable;

	std::shared_ptr<ImFlow::NodeStyle> mathNode;
	std::shared_ptr<ImFlow::NodeStyle> renderNode;
	std::shared_ptr<ImFlow::NodeStyle> argumentNode;
	std::shared_ptr<ImFlow::NodeStyle> constantNode;
	std::shared_ptr<ImFlow::NodeStyle> splitMergeNode;
	std::shared_ptr<ImFlow::NodeStyle> transformNode;
	std::shared_ptr<ImFlow::NodeStyle> globalNode;
	std::shared_ptr<ImFlow::NodeStyle> errorNode;



};


void NodeEditorPopup(ImFlow::ImNodeFlow& mINF, RenderInstance& proto, NodeStyles& styles, ImFlow::BaseNode* node);


struct NodeEditor{
	ImFlow::ImNodeFlow mINF;
	RenderInstance& proto;
	NodeStyles styles;
	NodeEditor(RenderInstance& prot):proto(prot) {
		mINF.rightClickPopUpContent([this](ImFlow::BaseNode* node) {
			NodeEditorPopup(mINF,proto,styles, node);
		});


		styles.transformResult = std::make_shared<ImFlow::PinStyle>(IM_COL32(191,90,90,255),1,4.f,4.67f,3.7f,1.f);
		styles.transformSize = std::make_shared<ImFlow::PinStyle>(IM_COL32(90,191,93,255),1,4.f,4.67f,3.7f,1.f);
		
		styles.intVariable = std::make_shared<ImFlow::PinStyle>(IM_COL32(87,155,185,255),0,4.f,4.67f,3.7f,1.f);
		styles.boolVariable = std::make_shared<ImFlow::PinStyle>(IM_COL32(90,117,191,255),0,4.f,4.67f,3.7f,1.f);
		styles.floatVariable = std::make_shared<ImFlow::PinStyle>(IM_COL32(255,255,255,255),0,4.f,4.67f,3.7f,1.f);
		styles.float2Variable = std::make_shared<ImFlow::PinStyle>(IM_COL32(255,255,255,255),0,4.f,4.67f,3.7f,1.f);
		styles.float3Variable = std::make_shared<ImFlow::PinStyle>(IM_COL32(255,255,255,255),0,4.f,4.67f,3.7f,1.f);
		styles.colorVariable = std::make_shared<ImFlow::PinStyle>(IM_COL32(255,255,255,255),0,4.f,4.67f,3.7f,1.f);
		styles.stringVariable = std::make_shared<ImFlow::PinStyle>(IM_COL32(255,255,255,255),0,4.f,4.67f,3.7f,1.f);
		styles.assetVariable = std::make_shared<ImFlow::PinStyle>(IM_COL32(255,255,255,255),0,4.f,4.67f,3.7f,1.f);
	
		styles.mathNode = std::make_shared<ImFlow::NodeStyle>(IM_COL32(17,61,173,255),ImColor(233,241,244,255),6.5f);
		styles.transformNode = std::make_shared<ImFlow::NodeStyle>(IM_COL32(27,173,17,255),ImColor(233,241,244,255),6.5f);
		styles.renderNode = std::make_shared<ImFlow::NodeStyle>(IM_COL32(173,40,17,255),ImColor(233,241,244,255),6.5f);
		styles.constantNode = std::make_shared<ImFlow::NodeStyle>(IM_COL32(173,17,170,255),ImColor(233,241,244,255),6.5f);
		styles.argumentNode = std::make_shared<ImFlow::NodeStyle>(IM_COL32(113,17,173,255),ImColor(233,241,244,255),6.5f);
		styles.splitMergeNode = std::make_shared<ImFlow::NodeStyle>(IM_COL32(17,149,173,255),ImColor(233,241,244,255),6.5f);
		styles.globalNode = std::make_shared<ImFlow::NodeStyle>(IM_COL32(57,17,132,255),ImColor(233,241,244,255),6.5f);
		styles.errorNode = std::make_shared<ImFlow::NodeStyle>(IM_COL32(173,25,17,255),ImColor(233,241,244,255),6.5f);
		styles.errorNode->bg = IM_COL32(132,23,17,255);
		
		
	}
	void draw();
};


struct Variable {
	bool isConstant = true;
	bool isGlobal = false;
};

struct IntVariable :Variable {
	int value;
};

struct BoolVariable :Variable {
	int value;
};

struct FloatVariable :Variable {
	float value;
};

struct Float2Variable :Variable {
	float value[2];
};

struct Float3Variable :Variable {
	float value[3];
};

struct ColorVariable :Variable {
	float value[4];
};

struct StringVariable :Variable {
	std::string value;
};

struct AssetVariable :Variable {
	std::string value;
};

