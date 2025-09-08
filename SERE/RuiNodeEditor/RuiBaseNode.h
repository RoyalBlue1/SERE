#pragma once
#include <memory>
#include <set>
#include "Util.h"

#include "imgui/ImNodeFlow.h"


class RuiBaseNode;

#define RAPIDJSON_HAS_STDSTRING 1
#include "ThirdParty/rapidjson/document.h"
#include "RuiRendering/RenderManager.h"
#include "RuiNodeEditor/Mapping.h"
#include "RuiNodeEditor/RuiExportPrototype.h"


class RuiBaseNode : public ImFlow::BaseNode {
public:
	virtual void Serialize(rapidjson::GenericValue<rapidjson::UTF8<>>& obj, rapidjson::Document::AllocatorType& allocator) {
		ImVec2 pos = getPos();
		obj.AddMember("PosX", pos.x, allocator);
		obj.AddMember("PosY", pos.y, allocator);
		obj.AddMember("Id", getUID(), allocator);
	}
	virtual void Export(RuiExportPrototype&) = 0;
protected:
	RenderInstance& render;
	ImFlow::StyleManager& styles;
	RuiBaseNode(std::string name,
		std::string category,
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
	if (!(obj.HasMember("Id") && obj["Id"].IsUint64()))return nullptr;
	if (!(obj.HasMember("PosX") && obj["PosX"].IsNumber()))return nullptr;
	if (!(obj.HasMember("PosY") && obj["PosY"].IsNumber()))return nullptr;

	ImVec2 pos;
	pos.x = obj["PosX"].GetFloat();
	pos.y = obj["PosY"].GetFloat();
	uint64_t uid = obj["Id"].GetUint64();

	return mINF.recreateNode<T>(pos, uid, proto, styles, obj);
}

template<class T> NodeType CreateNodeType() {
	NodeType type;
	type.AddNode = AddNode<T>;
	type.GetPinInfo = GetPinInfo<T>;
	type.RecreateNode = RecreateNode<T>;
	return type;
};


