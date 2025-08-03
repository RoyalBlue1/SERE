#pragma once
#include <memory>

#include "RuiNodeEditor/RuiNodeStyle.h"

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
	ImFlow::StyleManager& styles;
	RuiBaseNode( std::string name,
		std::string category,
		std::vector<std::shared_ptr<ImFlow::PinProto>> pinInfo,
		RenderInstance& rend,
		ImFlow::StyleManager& style
	):render(rend),styles(style) {
		setTitle(name);
		setStyle(styles.GetNodeStyle(category));
		for (auto& pin : pinInfo) {
			pin->CreatePin(this,styles);
		}
	}
};

struct NodeType {
	std::shared_ptr<RuiBaseNode> (*AddNode)(ImFlow::ImNodeFlow& mINF, RenderInstance& proto, ImFlow::StyleManager& style);
	std::shared_ptr<RuiBaseNode> (*RecreateNode)(ImFlow::ImNodeFlow& mINF, RenderInstance& proto, ImFlow::StyleManager& style, rapidjson::GenericObject<false,rapidjson::Value> obj);
	std::vector<std::shared_ptr<ImFlow::PinProto>> (*GetPinInfo)();
};


typedef std::map<std::string,NodeType> NodeCategory;


template<class T> std::shared_ptr<RuiBaseNode> AddNode(ImFlow::ImNodeFlow& mINF, RenderInstance& proto, ImFlow::StyleManager& styles) {
	return mINF.placeNode<T>(proto,styles);
}

template<class T> std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo() {
	return T::GetPinInfo();
}

template <class T>std::shared_ptr<RuiBaseNode> RecreateNode(ImFlow::ImNodeFlow& mINF, RenderInstance& proto, ImFlow::StyleManager& styles, rapidjson::GenericObject<false,rapidjson::Value> obj){
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


struct Variable {
	bool IsConstant() const {
		return name.size() == 0;
	}
	std::string name;
	Variable(std::string name_):name(name_){}
	static std::string UniqueName(){
		return std::format("var_{:X}",0);//TODO add random
	}
};

struct IntVariable :Variable {
	int value;
	IntVariable(int val, std::string name = "") :Variable(name), value(val) {

	}
	IntVariable():Variable(""),value(0){};
};

struct BoolVariable :Variable {
	int value;
	BoolVariable(bool val, std::string name = "") :Variable(name), value(val) {

	}
	BoolVariable():Variable(""),value(false){};
};

struct FloatVariable :Variable {
	float value;
	FloatVariable(float val, std::string name = ""):Variable(name), value(val) {

	}
	FloatVariable():Variable(""),value(1.f){};
};

struct Float2Variable :Variable {
	Vector2 value;
	Float2Variable(Vector2 val, std::string name = ""):Variable(name), value(val) {

	}
	Float2Variable(float x, float y, std::string name = ""):Variable(name), value(x,y) {

	}
	Float2Variable():Variable(""),value(1.f,1.f){};
};

struct Float3Variable :Variable {
	Vector3 value;
	Float3Variable(Vector3 val, std::string name = ""):Variable(name), value(val) {

	}
	Float3Variable(float x, float y, float z,std::string name = "") :Variable(name), value(x, y, z) {

	}
	Float3Variable():Variable(""),value(1.f,1.f,1.f){};
};

struct ColorVariable :Variable {
	Color value;
	ColorVariable(Color val, std::string name = ""):Variable(name), value(val) {

	}
	ColorVariable(float r, float g, float b,float a, std::string name = ""):Variable(name), value(r,g,b,a) {

	}
	ColorVariable() :Variable(""), value(1.f, 1.f, 1.f, 1.f) {};
};

struct StringVariable :Variable {
	std::string value;
	StringVariable(std::string val, std::string name = ""):Variable(name), value(val) {

	}
	StringVariable(const char* val, std::string name = ""):Variable(name), value(val) {

	}
	StringVariable():Variable(""),value(""){}
};

struct AssetVariable :Variable {
	uint32_t hash;
	AssetVariable(std::string val, std::string name = ""):Variable(name) {
		hash = loadAsset(val.c_str());
	}
	AssetVariable(uint32_t val, std::string name = ""):Variable(name),hash(val) {

	}
	AssetVariable() :Variable(""), hash(INVALID_ASSET) {}
};

