#include "RuiNodeEditor.h"
#include "imgui/imgui.h"

#include "imgui/imgui_stdlib.h"

#include "Util.h"

#include "Nodes/ArgumentNodes.h"
#include "Nodes/TransformNodes.h"
#include "Nodes/RenderJobNodes.h"
#include "Nodes/ConstantVarNodes.h"
#include "Nodes/SplitMergeNodes.h"
#include "Nodes/MathNodes.h"
#include "Nodes/GlobalNodes.h"
#include "Nodes/ConditionalNodes.h"

#include "Thirdparty/rapidjson/istreamwrapper.h"
#include "ThirdParty/rapidjson/prettywriter.h"
#include "ThirdParty/rapidjson/stringbuffer.h"
#include "ThirdParty/nativefiledialog-extended/src/include/nfd.hpp"
#undef GetObject

NodeEditor::NodeEditor(RenderInstance& rend):render(rend) {
	mINF.rightClickPopUpContent([this](ImFlow::BaseNode* node) {
		RightClickPopup(node);
	});
	mINF.droppedLinkPopUpContent([this](ImFlow::Pin* pin) {
		LinkDroppedPopup(pin);
	});
	ImFlow::StyleManager& styles = mINF.getStyleManager();;
	SetStyles(styles);

}



void NodeEditor::Draw() {
	ImGui::Begin("Node Editor");
	
	mINF.update();

	ImGui::End();
}

void NodeEditor::Clear() {
	for (auto& [uid,node] : mINF.getNodes()) {
		node->destroy();
	}
}

void NodeEditor::Serialize() {

	if(!mINF.getNodesCount())return;

	NFD::Guard nfdGuard;
	nfdfilteritem_t filter("Graph", "json");
	NFD::UniquePath nfdPath;
	if(NFD::SaveDialog(nfdPath, &filter, 1) != NFD_OKAY) return;
	
	fs::path path(nfdPath.get());



	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::GenericValue<rapidjson::UTF8<>> nodeArray;
	nodeArray.SetArray();
	for (auto& [nodeId, nodePtr] : mINF.getNodes()) {

		rapidjson::GenericValue<rapidjson::UTF8<>> val;
		val.SetObject();
		std::dynamic_pointer_cast<RuiBaseNode>(nodePtr)->Serialize(val,doc.GetAllocator());
		nodeArray.PushBack(val,doc.GetAllocator());
	}
	doc.AddMember("Nodes",nodeArray,doc.GetAllocator());
	rapidjson::GenericValue<rapidjson::UTF8<>> linkArray;
	linkArray.SetArray();
	for (auto& link : mINF.getLinks()) {
		rapidjson::GenericValue<rapidjson::UTF8<>> val;
		val.SetObject();
		auto lnk = link.lock();
		val.AddMember("LeftNode",lnk->left()->getParent()->getUID(), doc.GetAllocator());
		val.AddMember("LeftPin",lnk->left()->getName(), doc.GetAllocator());
		val.AddMember("RightNode",lnk->right()->getParent()->getUID(),doc.GetAllocator());
		val.AddMember("RightPin",lnk->right()->getName(), doc.GetAllocator());
		linkArray.PushBack(val,doc.GetAllocator());
	}
	doc.AddMember("Links",linkArray,doc.GetAllocator());

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::ofstream outFile{path};
	outFile.write(buffer.GetString(),buffer.GetSize());
	outFile.close();

}

void NodeEditor::Deserialize() {


	NFD::Guard nfdGuard;
	nfdfilteritem_t filter("Graph","json");
	NFD::UniquePath nfdPath;
	if(NFD::OpenDialog(nfdPath,&filter,1) != NFD_OKAY) return;
	

	fs::path path (nfdPath.get());


	std::ifstream file(path);
	if (!file.is_open()) {
		printf("Error Opening JSON File %s\n",path.string().c_str());
		return;
	}
	rapidjson::IStreamWrapper wrap(file);
	rapidjson::Document doc;
	doc.ParseStream(wrap);
	if (doc.HasParseError()) {
		printf("Error in JSON File %s\n",path.string().c_str());
		return;
	}
	rapidjson::GenericObject root = doc.GetObject();
	if(!(root.HasMember("Nodes")&&root["Nodes"].IsArray()))return;
	if(!(root.HasMember("Links")&&root["Links"].IsArray()))return;
	rapidjson::GenericArray nodes = root["Nodes"].GetArray();
	for (auto itr = nodes.Begin(); itr != nodes.End(); itr++) {
		if (!itr->IsObject()) {
			printf("Node is not object");
			continue;
		}
		rapidjson::GenericObject node = itr->GetObject();

		if(!(node.HasMember("Name")&&node["Name"].IsString()))continue;
		if(!(node.HasMember("Category")&&node["Category"].IsString()))continue;

		std::string name = node["Name"].GetString();
		std::string category = node["Category"].GetString();
		if (!nodeTypes.contains(category)){
			printf("Unknown Category %s",category.c_str());
			continue;
		}
		if (!nodeTypes[category].contains(name)) {
			printf("Unknown Node %s",name.c_str());
			continue;
		}
		nodeTypes[category][name].RecreateNode(mINF,render,mINF.getStyleManager(), node);
		
	}
	rapidjson::GenericArray links = root["Links"].GetArray();
	for (auto itr = links.Begin(); itr != links.End(); itr++) {
		if(!itr->IsObject())continue;
		rapidjson::GenericObject link = itr->GetObject();
		if(!(link.HasMember("LeftNode")&&link["LeftNode"].IsUint64()))continue;
		if(!(link.HasMember("LeftPin")&&link["LeftPin"].IsString()))continue;
		if(!(link.HasMember("RightNode")&&link["RightNode"].IsUint64()))continue;
		if(!(link.HasMember("RightPin")&&link["RightPin"].IsString()))continue;
		uint64_t leftId = link["LeftNode"].GetUint64();
		uint64_t rightId = link["RightNode"].GetUint64();
		std::string leftPinName = link["LeftPin"].GetString();
		std::string rightPinName = link["RightPin"].GetString();

		if(!mINF.getNodes().contains(leftId))continue;
		if(!mINF.getNodes().contains(rightId))continue;
		auto left = mINF.getNodes()[leftId];
		auto right = mINF.getNodes()[rightId];

		left->outPin(leftPinName)->createLink(right->inPin(rightPinName));

	}
}

void NodeEditor::Export() {

	nfdfilteritem_t filter("RuiPackage","ruip");
	NFD::UniquePath nfdPath;
	if(NFD::SaveDialog(nfdPath,&filter,1) != NFD_OKAY) return;
	fs::path path (nfdPath.get());
	std::string name = path.filename().replace_extension("").string();
	RuiExportPrototype proto(render,name);
	proto.Generate(mINF.getNodes(),render);
	NFD::Guard nfdGuard;
	

	
	proto.WriteToFile(path);
}

void NodeEditor::RightClickPopup(ImFlow::BaseNode* node) {
	if (node) {

		if (ImGui::MenuItem("Delete")) {
			node->destroy();
		}
		return;
	}

	for (const auto& [categoryName, category] : nodeTypes) {
		if (ImGui::BeginMenu(categoryName.c_str())) {
			for (const auto& [nodeName, nodeType] : category) {
				if (ImGui::MenuItem(nodeName.c_str())) {
					nodeType.AddNode(mINF,render,mINF.getStyleManager());
				}
			}

			ImGui::EndMenu();
		}

	}
#ifdef _DEBUG
	if (ImGui::BeginMenu("Debug")) {
		if (ImGui::MenuItem("Spawn All Node Types")) {
			for (auto& [catName, category] : nodeTypes) {
				for (auto& [name, node] : category) {
					node.AddNode(mINF,render,mINF.getStyleManager());
				}
			}

		}
		ImGui::EndMenu();
	}
#endif
}

void NodeEditor::LinkDroppedPopup(ImFlow::Pin* pin) {
	if (!pin)
		return;

	ImFlow::PinType neededPinType = pin->getType() == ImFlow::PinType_Input ?
		ImFlow::PinType_Output : ImFlow::PinType_Input;

	static std::string searchString;

	// force keyboard focus to the InputText when first showing the window
	if (ImGui::GetCurrentWindow()->Appearing)
		ImGui::SetKeyboardFocusHere();
	// if enter is pressed, select the first thing we find
	bool selectFirstOption = ImGui::InputText("Search", &searchString, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);

	for (auto& [catName, category] : nodeTypes)
	{
		bool searchHit = caseInsensitiveSearch(catName, searchString);

		bool hasCategoryHeader = false;
		for (auto& [nodeName, nodeType] : category)
		{
			searchHit |= caseInsensitiveSearch(nodeName, searchString);
			for (auto& pinInfo : nodeType.GetPinInfo())
			{
				searchHit |= caseInsensitiveSearch(pinInfo->name, searchString);

				if (pin->getType() == pinInfo->GetPinType())
					continue;
				if (!pinInfo->CanCreateLink(pin->getProto()))
					continue;
				if (searchString.size() && !searchHit)
					continue;

				if (!hasCategoryHeader)
				{
					ImGui::MenuItem(catName.c_str(), nullptr, nullptr, false);
					hasCategoryHeader = true;
				}

				std::string menuName = std::format("{} > {}", nodeName, pinInfo->name);
				if (selectFirstOption || ImGui::MenuItem(menuName.c_str())) {
					std::shared_ptr<ImFlow::BaseNode> node = nodeType.AddNode(mINF, render, mINF.getStyleManager());
					// create the pin
					if (pinInfo->GetPinType() == ImFlow::PinType_Input) {
						node->inPin(pinInfo->name.c_str())->createLink(pin);
					}
					else {
						node->outPin(pinInfo->name.c_str())->createLink(pin);
					}

					// clear the search for next time and exit
					searchString = "";
					ImGui::CloseCurrentPopup(); // todo: return value for this function so the caller can handle this? we may not be a popup i guess
					return;
				}
			}
		}
	}
}


void NodeEditor::SetStyles(ImFlow::StyleManager& styles) {
	styles.AddPinStlye(
		typeid(TransformResult).name(),
		std::make_shared<ImFlow::PinStyle>(IM_COL32(191,90,90,255),1,4.f,4.67f,3.7f,1.f));
	styles.AddPinStlye(
		typeid(TransformSize).name(),
		std::make_shared<ImFlow::PinStyle>(IM_COL32(90,191,93,255),1,4.f,4.67f,3.7f,1.f));

	styles.AddPinStlye(
		typeid(IntVariable).name(),
		std::make_shared<ImFlow::PinStyle>(IM_COL32(87,155,185,255),0,4.f,4.67f,3.7f,1.f));
	styles.AddPinStlye(
		typeid(BoolVariable).name(),
		std::make_shared<ImFlow::PinStyle>(IM_COL32(90,117,191,255),0,4.f,4.67f,3.7f,1.f));
	styles.AddPinStlye(
		typeid(FloatVariable).name(),
		std::make_shared<ImFlow::PinStyle>(IM_COL32(247,229,113,255),0,4.f,4.67f,3.7f,1.f));
	styles.AddPinStlye(
		typeid(Float2Variable).name(),
		std::make_shared<ImFlow::PinStyle>(IM_COL32(142,247,113,255),0,4.f,4.67f,3.7f,1.f));
	styles.AddPinStlye(
		typeid(Float3Variable).name(),	
		std::make_shared<ImFlow::PinStyle>(IM_COL32(113,247,200,255),0,4.f,4.67f,3.7f,1.f));
	styles.AddPinStlye(
		typeid(ColorVariable).name(),
		std::make_shared<ImFlow::PinStyle>(IM_COL32(113,178,247,255),0,4.f,4.67f,3.7f,1.f));
	styles.AddPinStlye(
		typeid(StringVariable).name(),	
		std::make_shared<ImFlow::PinStyle>(IM_COL32(133,113,247,255),0,4.f,4.67f,3.7f,1.f));
	styles.AddPinStlye(
		typeid(AssetVariable).name(),
		std::make_shared<ImFlow::PinStyle>(IM_COL32(218,113,247,255),0,4.f,4.67f,3.7f,1.f));




	styles.AddNodeStlye(
		"Math",
		std::make_shared<ImFlow::NodeStyle>(IM_COL32(17, 61, 173, 255), ImColor(233, 241, 244, 255), 6.5f));
	styles.AddNodeStlye(
		"Transform", 
		std::make_shared<ImFlow::NodeStyle>(IM_COL32(27, 173, 17, 255), ImColor(233, 241, 244, 255), 6.5f));
	styles.AddNodeStlye(
		"Render", 
		std::make_shared<ImFlow::NodeStyle>(IM_COL32(173,40,17,255),ImColor(233,241,244,255),6.5f));
	styles.AddNodeStlye(
		"Constant", 
		std::make_shared<ImFlow::NodeStyle>(IM_COL32(173,17,170,255),ImColor(233,241,244,255),6.5f));
	styles.AddNodeStlye(
		"Argument", 
		std::make_shared<ImFlow::NodeStyle>(IM_COL32(113,17,173,255),ImColor(233,241,244,255),6.5f));
	styles.AddNodeStlye(
		"Split Merge", 
		std::make_shared<ImFlow::NodeStyle>(IM_COL32(17,149,173,255),ImColor(233,241,244,255),6.5f));
	styles.AddNodeStlye(
		"Global", 
		std::make_shared<ImFlow::NodeStyle>(IM_COL32(57,17,132,255),ImColor(233,241,244,255),6.5f));
	styles.AddNodeStlye(
		"Conditionals",
		std::make_shared<ImFlow::NodeStyle>(IM_COL32(173, 74, 17, 255), ImColor(233, 241, 244, 255), 6.5f));
	
	auto errorNodeStyle = std::make_shared<ImFlow::NodeStyle>(IM_COL32(173,25,17,255),ImColor(233,241,244,255),6.5f);
	errorNodeStyle->bg = IM_COL32(132,23,17,255);
	styles.SetNodeErrorStyle(errorNodeStyle);
}

void NodeEditor::CopyNodes() {
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<int> dis;

	// Reset copied nodes list
	rapidjson::Document doc;
	doc.SetObject();
	m_lCopiedNodes.SetArray();

	// Serialize currently selected nodes
	for (auto& [nodeId, nodePtr] : mINF.getNodes()) {
		if (!nodePtr->isSelected()) {
			continue;
		}

		// Convert node to JSON to preserve its internal state
		rapidjson::GenericValue<rapidjson::UTF8<>> val;
		val.SetObject();
		std::dynamic_pointer_cast<RuiBaseNode>(nodePtr)->Serialize(val, doc.GetAllocator());

		// Eventually rename arguments
		if (val.HasMember("ArgName") && val["ArgName"].IsString()) {
			val["ArgName"].SetString(std::format("{}(copy)", val["ArgName"].GetString()), doc.GetAllocator());
		}

		// Assign new id to node copy (stolen from ImNodeFlow::addNode)
		auto uid = dis(gen);
		while (mINF.getNodes().contains(uid))uid = dis(gen);
		val["Id"].SetInt(uid);
		m_lCopiedNodes.PushBack(val, doc.GetAllocator());
	}
}

void NodeEditor::PasteNodes() {
	if (m_lCopiedNodes.GetArray().Empty()) {
		return;
	}

	std::set<int> newNodeIds = {};

	for (auto itr = m_lCopiedNodes.Begin(); itr != m_lCopiedNodes.End(); itr++) {
		rapidjson::GenericObject node = itr->GetObject();
		std::string name = node["Name"].GetString();
		std::string category = node["Category"].GetString();
		auto newnode = nodeTypes[category][name].RecreateNode(mINF, render, mINF.getStyleManager(), node);

		ImVec2 pos;
		pos.x = node["PosX"].GetFloat() + 10;
		pos.y = node["PosY"].GetFloat() + 10;
		newnode->setPos(pos);

		newNodeIds.insert(node["Id"].GetInt());
	}

	// Select only pasted nodes
	for (auto& [nodeId, nodePtr] : mINF.getNodes()) {
		// Unselect nodes
		if (nodePtr->isSelected()) {
			nodePtr->selected(false);
			continue;
		}

		// Select new nodes
		if (newNodeIds.contains(nodePtr->getUID()))
		{
			nodePtr->selected(true);
		}
	}

	// Only allow one paste action
	m_lCopiedNodes.SetArray();
}
