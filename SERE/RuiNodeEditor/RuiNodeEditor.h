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
	fs::path editedGraph;
	int m_iSavedNodesCount = 0;
	bool m_bIsUnsaved = true;
	

	std::map<std::string,NodeCategory> nodeTypes;
public:
	NodeEditor(RenderInstance& rend, HWND& hwnd);
	void SetStyles(ImFlow::StyleManager& styles);
	void RightClickPopup(ImFlow::BaseNode* node);
	void LinkDroppedPopup(ImFlow::Pin* pin);
	void Draw();
	void Serialize(fs::path outPath = fs::path());
	void Deserialize();
	void Export();
	void Clear();
	void UpdateEditedPath(fs::path path, bool isUnsaved = true);

	/**
	 * Saves the current graph.
	 * 
	 * If no path has been previously selected, prompts the user to pick one;
	 * else, exports the graph using this path.
	 **/
	void Save();

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



	
