#pragma once
#include <memory>

#define RAPIDJSON_HAS_STDSTRING 1
#include "ThirdParty/rapidjson/document.h"

#include "Imgui/ImNodeFlow.h"
#include "RuiRendering/RenderManager.h"

class RuiBaseNode;




#include "RuiNodeEditor/RuiExportPrototype.h"


class RuiBaseNode : public ImFlow::BaseNode {
public:
	virtual void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
		ImVec2 pos = getPos();
		obj.AddMember("PosX", pos.x, allocator);
		obj.AddMember("PosY", pos.y, allocator);
		obj.AddMember("Id", static_cast<uint64_t>(getUID()), allocator);
		storeInputPinEmptyValues(obj,allocator);

	}
	virtual void Export(RuiExportPrototype&) = 0;
	void recreateInputPinEmptyValues(rapidjson::GenericObject<false, rapidjson::Value> obj);
	void storeInputPinEmptyValues(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator);
	template <typename T> void storePin(const char* name,std::unordered_map<std::string,std::any>& defaultVals)
	{
		std::string typeName = typeid(T).name();
		std::any value = getInVal<T>(name);
		defaultVals[typeName] = value;
	}
	template <typename T> std::shared_ptr<ImFlow::InPin<T>> recreatePin(const char* name,std::function<bool(const std::type_info&, const std::type_info&)> selectionFunc,std::unordered_map<std::string,std::any>& defaultVals)
	{
		std::string typeName = typeid(T).name();
		if (!defaultVals.contains(typeName))
			defaultVals[typeName] = T();
		auto info = std::make_shared<ImFlow::InPinProto<T>>(name, selectionFunc, std::any_cast<T>(defaultVals[typeName]));

		ImFlow::PinUID h = std::hash<std::string>{}(name);
		for (auto it = m_ins.begin(); it != m_ins.end(); it++)
		{
			if (it->get()->getUid() == h)
			{
				auto pin = std::make_shared<ImFlow::InPin<T>>(h,info,std::any_cast<T>(defaultVals[typeName]),styles.GetPinStyle(typeName),this,&m_inf);
				*it = pin;
				return pin;
			}
		}



		return addIN<T>(info,std::any_cast<T>(defaultVals[typeName]),styles.GetPinStyle(typeName));

	}
protected:
	RenderInstance& render;
	ImFlow::StyleManager& styles;
	RuiBaseNode(const std::string& name,
		const std::string& category,
		std::vector<std::shared_ptr<ImFlow::PinProto>> pinInfo,
		RenderInstance& rend,
		ImFlow::StyleManager& style
	) :render(rend), styles(style) {
		setTitle(name);
		setStyle(styles.GetNodeStyle(category));
		for (auto& pin : pinInfo) {
			pin->CreatePin(this, styles);
		}
	}
	FloatVariable getInNumeric(const char* id);
	MathVariable getInMath(const char* id);
	
};

struct NodeType {
	std::shared_ptr<RuiBaseNode>(*AddNode)(ImFlow::ImNodeFlow& mINF, RenderInstance& proto, ImFlow::StyleManager& style);
	std::shared_ptr<RuiBaseNode>(*RecreateNode)(ImFlow::ImNodeFlow& mINF, RenderInstance& proto, ImFlow::StyleManager& style, rapidjson::GenericObject<false, rapidjson::Value> obj);
	std::vector<std::shared_ptr<ImFlow::PinProto>>(*GetPinInfo)();
};


typedef std::map<std::string, NodeType> NodeCategory;


template<class T> std::shared_ptr<RuiBaseNode> AddNode(ImFlow::ImNodeFlow& mINF, RenderInstance& proto, ImFlow::StyleManager& styles) {
	return mINF.placeNode<T>(proto, styles);
}

template<class T> std::vector<std::shared_ptr<ImFlow::PinProto>> GetPinInfo() {
	return T::GetPinInfo();
}

template <class T>std::shared_ptr<RuiBaseNode> RecreateNode(ImFlow::ImNodeFlow& mINF, RenderInstance& proto, ImFlow::StyleManager& styles, rapidjson::GenericObject<false, rapidjson::Value> obj) {
	if (!obj.HasMember("Id"))return nullptr;
	if (!(obj.HasMember("PosX") && obj["PosX"].IsNumber()))return nullptr;
	if (!(obj.HasMember("PosY") && obj["PosY"].IsNumber()))return nullptr;

	ImVec2 pos;
	pos.x = obj["PosX"].GetFloat();
	pos.y = obj["PosY"].GetFloat();
	uint64_t uid = 0;
	if (obj["Id"].IsUint64()) {
		uid = obj["Id"].GetUint64();
	}
	else if (obj["Id"].IsInt64() && obj["Id"].GetInt64() >= 0) {
		uid = static_cast<uint64_t>(obj["Id"].GetInt64());
	}
	else {
		return nullptr;
	}

	std::shared_ptr<RuiBaseNode> res = mINF.recreateNode<T>(pos, uid, proto, styles, obj);

	res->recreateInputPinEmptyValues(obj);
	return res;
}

template<class T> NodeType CreateNodeType() {
	NodeType type;
	type.AddNode = AddNode<T>;
	type.GetPinInfo = GetPinInfo<T>;
	type.RecreateNode = RecreateNode<T>;
	return type;
};


bool isPinNumeric(const std::type_info& out, const std::type_info& in);
bool isPinMath(const std::type_info& out, const std::type_info& in);
