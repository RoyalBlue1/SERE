#pragma once
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <filesystem>

#include "imgui/ImNodeFlow.h"
#include "RenderManager.h"
#define RAPIDJSON_HAS_STDSTRING 1
#include "ThirdParty/rapidjson/document.h"

namespace fs = std::filesystem;


struct NodeStyles {
	std::map<std::string, std::shared_ptr<ImFlow::PinStyle>> pinStyles;

	std::map<std::string, std::shared_ptr<ImFlow::NodeStyle>> nodeStyles;
	std::shared_ptr<ImFlow::NodeStyle> defaultNode;
	std::shared_ptr<ImFlow::NodeStyle> errorNode;
	NodeStyles();
	std::shared_ptr<ImFlow::NodeStyle> GetNodeStyle(std::string name);
};

class RuiBaseNode : public ImFlow::BaseNode {
public:
	virtual void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
		ImVec2 pos = getPos();
		obj.AddMember("PosX",pos.x,allocator);
		obj.AddMember("PosY",pos.y,allocator);
		obj.AddMember("Id",getUID(),allocator);
	}
	//virtual void Export() = 0;
protected:
	RenderInstance& render;
	NodeStyles& styles;
	RuiBaseNode( std::string name,
		std::string category,
		std::vector<std::shared_ptr<ImFlow::PinProto>> pinInfo,
		RenderInstance& rend,
		NodeStyles& style
	):render(rend),styles(style) {
		setTitle(name);
		setStyle(styles.GetNodeStyle(category));
		for (auto& pin : pinInfo) {
			pin->CreatePin(this,styles.pinStyles);
		}
	}
};

struct NodeType {
	std::shared_ptr<RuiBaseNode> (*AddNode)(ImFlow::ImNodeFlow& mINF, RenderInstance& proto, NodeStyles& style);
	std::shared_ptr<RuiBaseNode> (*RecreateNode)(ImFlow::ImNodeFlow& mINF, RenderInstance& proto, NodeStyles& style, rapidjson::GenericObject<false,rapidjson::Value> obj);
	std::vector<std::shared_ptr<ImFlow::PinProto>> (*GetPinInfo)();
};


typedef std::map<std::string,NodeType> NodeCategory;


template<class T> std::shared_ptr<RuiBaseNode> AddNode(ImFlow::ImNodeFlow& mINF, RenderInstance& proto, NodeStyles& styles) {
	return mINF.placeNode<T>(proto,styles);
}

template<class T> std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo() {
	return T::GetPinInfo();
}

template <class T>std::shared_ptr<RuiBaseNode> RecreateNode(ImFlow::ImNodeFlow& mINF, RenderInstance& proto, NodeStyles& styles, rapidjson::GenericObject<false,rapidjson::Value> obj){
	if(!(obj.HasMember("Id")&&obj["Id"].IsUint64()))return nullptr;
	if(!(obj.HasMember("PosX")&&obj["PosX"].IsNumber()))return nullptr;
	if(!(obj.HasMember("PosY")&&obj["PosY"].IsNumber()))return nullptr;

	ImVec2 pos;
	pos.x = obj["PosX"].GetFloat();
	pos.y = obj["PosY"].GetFloat();
	uint64_t uid = obj["Id"].GetUint64();

	return mINF.recreateNode<T>(pos,uid,proto,styles,obj);
}

template<class T> NodeType CreateNodeType() {
	NodeType type;
	type.AddNode = AddNode<T>;
	type.GetPinInfo = GetPinInfo<T>;
	type.RecreateNode = RecreateNode<T>;
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
	void Serialize(const fs::path& path);
	void Deserialize(const fs::path& path);

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


	
