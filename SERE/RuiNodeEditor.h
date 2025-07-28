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
	
	std::map<std::string,std::shared_ptr<ImFlow::PinStyle>> pinStyles;
	
	std::map<std::string,std::shared_ptr<ImFlow::NodeStyle>> nodeStyles;
	std::shared_ptr<ImFlow::NodeStyle> defaultNode;
	std::shared_ptr<ImFlow::NodeStyle> errorNode;
	NodeStyles() {
		transformResult = std::make_shared<ImFlow::PinStyle>(IM_COL32(191,90,90,255),1,4.f,4.67f,3.7f,1.f);
		transformSize = std::make_shared<ImFlow::PinStyle>(IM_COL32(90,191,93,255),1,4.f,4.67f,3.7f,1.f);

		intVariable = std::make_shared<ImFlow::PinStyle>(IM_COL32(87,155,185,255),0,4.f,4.67f,3.7f,1.f);
		boolVariable = std::make_shared<ImFlow::PinStyle>(IM_COL32(90,117,191,255),0,4.f,4.67f,3.7f,1.f);
		floatVariable = std::make_shared<ImFlow::PinStyle>(IM_COL32(255,255,255,255),0,4.f,4.67f,3.7f,1.f);
		float2Variable = std::make_shared<ImFlow::PinStyle>(IM_COL32(255,255,255,255),0,4.f,4.67f,3.7f,1.f);
		float3Variable = std::make_shared<ImFlow::PinStyle>(IM_COL32(255,255,255,255),0,4.f,4.67f,3.7f,1.f);
		colorVariable = std::make_shared<ImFlow::PinStyle>(IM_COL32(255,255,255,255),0,4.f,4.67f,3.7f,1.f);
		stringVariable = std::make_shared<ImFlow::PinStyle>(IM_COL32(255,255,255,255),0,4.f,4.67f,3.7f,1.f);
		assetVariable = std::make_shared<ImFlow::PinStyle>(IM_COL32(255,255,255,255),0,4.f,4.67f,3.7f,1.f);


		

		nodeStyles.emplace(
			"Math",
			std::make_shared<ImFlow::NodeStyle>(IM_COL32(17, 61, 173, 255), ImColor(233, 241, 244, 255), 6.5f));
		nodeStyles.emplace(
			"Transform", 
			std::make_shared<ImFlow::NodeStyle>(IM_COL32(27, 173, 17, 255), ImColor(233, 241, 244, 255), 6.5f));
		nodeStyles.emplace(
			"Render", 
			std::make_shared<ImFlow::NodeStyle>(IM_COL32(173,40,17,255),ImColor(233,241,244,255),6.5f));
		nodeStyles.emplace(
			"Constant", 
			std::make_shared<ImFlow::NodeStyle>(IM_COL32(173,17,170,255),ImColor(233,241,244,255),6.5f));
		nodeStyles.emplace(
			"Argument", 
			std::make_shared<ImFlow::NodeStyle>(IM_COL32(113,17,173,255),ImColor(233,241,244,255),6.5f));
		nodeStyles.emplace(
			"Split Merge", 
			std::make_shared<ImFlow::NodeStyle>(IM_COL32(17,149,173,255),ImColor(233,241,244,255),6.5f));
		nodeStyles.emplace(
			"Global", 
			std::make_shared<ImFlow::NodeStyle>(IM_COL32(57,17,132,255),ImColor(233,241,244,255),6.5f));

		defaultNode = std::make_shared<ImFlow::NodeStyle>(IM_COL32(173,40,17,255),ImColor(233,241,244,255),6.5f);
		errorNode = std::make_shared<ImFlow::NodeStyle>(IM_COL32(173,25,17,255),ImColor(233,241,244,255),6.5f);
		errorNode->bg = IM_COL32(132,23,17,255);
	}

	std::shared_ptr<ImFlow::NodeStyle> GetNodeStyle(std::string name) {
		if(nodeStyles.contains(name))
			return nodeStyles[name];
		return defaultNode;
	}
};




struct NodeType {
	std::shared_ptr<ImFlow::BaseNode> (*AddNode)(ImFlow::ImNodeFlow& mINF, RenderInstance& proto, NodeStyles& style);
	std::vector<std::shared_ptr<ImFlow::PinProto>> (*GetPinInfo)();
};


typedef std::map<std::string,NodeType> NodeCategory;



template<class T> std::shared_ptr<ImFlow::BaseNode> AddNode(ImFlow::ImNodeFlow& mINF, RenderInstance& proto, NodeStyles& styles) {
	return mINF.placeNode<T>(proto,styles);
}

template<class T> std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo() {
	return T::GetPinInfo();
}


template<class T> NodeType CreateNodeType() {
	NodeType type;
	type.AddNode = AddNode<T>;
	type.GetPinInfo = GetPinInfo<T>;
	return type;
}




struct NodeEditor{
	ImFlow::ImNodeFlow mINF;
	RenderInstance& proto;
	NodeStyles styles;

	std::map<std::string,NodeCategory> nodeTypes;

	NodeEditor(RenderInstance& prot):proto(prot) {
		mINF.rightClickPopUpContent([this](ImFlow::BaseNode* node) {
			RightClickPopup(node);
		});
		mINF.droppedLinkPopUpContent([this](ImFlow::Pin* pin) {
			LinkDroppedPopup(pin);
		});
		
	}

	void RightClickPopup(ImFlow::BaseNode* node);
	void LinkDroppedPopup(ImFlow::Pin* pin);
	void draw();


	template<class T> void AddNodeType() {
		
		
		const std::string& category = T::category;
		const std::string& name = T::name;
		NodeType type = CreateNodeType<T>();
		if (nodeTypes.contains(category)) {
			nodeTypes[category].emplace(name,type);
			return;
		}
		std::map<std::string,NodeType> newCategory;
		newCategory.emplace(name,type);
		nodeTypes.emplace(category,newCategory);
		
	}
};


struct Variable {
	bool isConstant;
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

