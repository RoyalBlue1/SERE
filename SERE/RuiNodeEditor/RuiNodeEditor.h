#pragma once
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <filesystem>

#include "imgui/ImNodeFlow.h"
#include "RuiRendering/RenderManager.h"
#define RAPIDJSON_HAS_STDSTRING 1
#include "ThirdParty/rapidjson/document.h"

#include "RuiNodeEditor/RuiBaseNode.h"
#include "RuiNodeEditor/RuiVariables.h"
#include "RuiNodeEditor/RuiExportPrototype.h"

namespace fs = std::filesystem;



class NodeEditor{
private:
	ImFlow::ImNodeFlow mINF;
	RenderInstance& render;
	rapidjson::GenericValue<rapidjson::UTF8<>> m_lCopiedNodes;
	std::map<std::string,NodeCategory> nodeTypes;
public:
	NodeEditor(RenderInstance& rend);
	void SetStyles(ImFlow::StyleManager& styles);
	void RightClickPopup(ImFlow::BaseNode* node);
	void LinkDroppedPopup(ImFlow::Pin* pin);
	void Draw();
	void Serialize();
	void Deserialize();
	void Export();
	void Clear();

	void CopyNodes();
	void PasteNodes();

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



	
