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
	std::shared_ptr<ImFlow::PinStyle> textData;
	

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
	int globalId;
	Variable(bool isConst,int globId):isConstant(isConst),globalId(globId){}
};

struct IntVariable :Variable {
	int value;
	IntVariable(int val, bool isConst = true, int globId = -1):Variable(isConst,globId), value(val) {
		
	}
	IntVariable():Variable(true,-1),value(0){};
};

struct BoolVariable :Variable {
	int value;
	BoolVariable(bool val, bool isConst = true, int globId = -1):Variable(isConst,globId), value(val) {

	}
	BoolVariable():Variable(true,-1),value(false){};
};

struct FloatVariable :Variable {
	float value;
	FloatVariable(float val, bool isConst = true, int globId = -1):Variable(isConst,globId), value(val) {

	}
	FloatVariable():Variable(true,-1),value(1.f){};
};

struct Float2Variable :Variable {
	Vector2 value;
	Float2Variable(Vector2 val, bool isConst = true, int globId = -1):Variable(isConst,globId), value(val) {

	}
	Float2Variable(float x, float y, bool isConst = true, int globId = -1):Variable(isConst,globId), value(x,y) {

	}
	Float2Variable():Variable(true,-1),value(1.f,1.f){};
};

struct Float3Variable :Variable {
	Vector3 value;
	Float3Variable(Vector3 val, bool isConst = true, int globId = -1):Variable(isConst,globId), value(val) {

	}
	Float3Variable(float x, float y, float z, bool isConst = true, int globId = -1):Variable(isConst,globId), value(x,y,z) {

	}
	Float3Variable():Variable(true,-1),value(1.f,1.f,1.f){};
};

struct ColorVariable :Variable {
	Color value;
	ColorVariable(Color val, bool isConst = true, int globId = -1):Variable(isConst,globId), value(val) {

	}
	ColorVariable(float r, float g, float b,float a, bool isConst = true, int globId = -1):Variable(isConst,globId), value(r,g,b,a) {

	}
	ColorVariable():Variable(true,-1),value(1.f,1.f,1.f,1.f){};
};

struct StringVariable :Variable {
	std::string value;
	StringVariable(std::string val, bool isConst = true, int globId = -1):Variable(isConst,globId), value(val) {

	}
	StringVariable(const char* val, bool isConst = true, int globId = -1):Variable(isConst,globId), value(val) {

	}
	StringVariable():Variable(true,-1),value(""){}
};

struct AssetVariable :Variable {
	uint32_t hash;
	AssetVariable(std::string val, bool isConst = true, int globId = -1):Variable(isConst,globId) {
		hash = loadAsset(val.c_str());
	}
	AssetVariable(uint32_t val, bool isConst = true, int globId = -1):Variable(isConst,globId),hash(val) {

	}
	AssetVariable() :Variable(true, -1), hash(INVALID_ASSET) {}
};

