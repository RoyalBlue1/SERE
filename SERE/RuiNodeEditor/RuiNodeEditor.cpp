#include "RuiNodeEditor.h"
#include "Imgui/imgui.h"

#include "Imgui/imgui_stdlib.h"

#include "Util.h"

#include "Nodes/ArgumentNodes.h"
#include "Nodes/TransformNodes.h"
#include "Nodes/RenderJobNodes.h"
#include "Nodes/ConstantVarNodes.h"
#include "Nodes/SplitMergeNodes.h"
#include "Nodes/MathNodes.h"
#include "Nodes/GlobalNodes.h"
#include "Nodes/ConditionalNodes.h"

#include "ThirdParty/rapidjson/istreamwrapper.h"
#include "ThirdParty/rapidjson/prettywriter.h"
#include "ThirdParty/rapidjson/stringbuffer.h"
#include <SDL3/SDL_dialog.h>
#include <SDL3/SDL.h>
#include <random>
#include <set>

#undef GetObject

namespace {

bool TryGetUint64(const rapidjson::Value& obj, const char* name, uint64_t& value) {
	if (!obj.HasMember(name)) {
		return false;
	}
	const rapidjson::Value& member = obj[name];
	if (member.IsUint64()) {
		value = member.GetUint64();
		return true;
	}
	if (member.IsInt64() && member.GetInt64() >= 0) {
		value = static_cast<uint64_t>(member.GetInt64());
		return true;
	}
	return false;
}

ImFlow::Pin* FindPinByName(const std::vector<std::shared_ptr<ImFlow::Pin>>& pins, const std::string& name) {
	for (const auto& pin : pins) {
		if (pin->getName() == name) {
			return pin.get();
		}
	}
	return nullptr;
}
}

NodeEditor::NodeEditor(RenderInstance& rend):render(rend) {
	m_clipboard.SetObject();
	mINF.rightClickPopUpContent([this](ImFlow::BaseNode* node) {
		RightClickPopup(node);
	});
	mINF.droppedLinkPopUpContent([this](ImFlow::Pin* pin) {
		LinkDroppedPopup(pin);
	});
	ImFlow::StyleManager& styles = mINF.getStyleManager();;
	SetStyles(styles);

}
static const SDL_DialogFileFilter filters[] = {
	{ "Graph",  "json" }
};


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
void NodeEditor::CopyNodes() {
	m_clipboard.SetObject();
	auto& allocator = m_clipboard.GetAllocator();
	rapidjson::Value copiedNodes(rapidjson::kArrayType);
	rapidjson::Value copiedLinks(rapidjson::kArrayType);
	std::set<ImFlow::NodeUID> selectedNodeIds;

	for (auto& [nodeId, nodePtr] : mINF.getNodes()) {
		if (nodePtr->isSelected()) {
			selectedNodeIds.insert(nodeId);
		}
	}

	// Serialize currently selected nodes
	for (auto& [nodeId, nodePtr] : mINF.getNodes()) {
		if (!selectedNodeIds.contains(nodeId)) {
			continue;
		}

		std::shared_ptr<RuiBaseNode> baseNode = std::dynamic_pointer_cast<RuiBaseNode>(nodePtr);
		if (!baseNode) {
			continue;
		}

		// Convert node to JSON to preserve its internal state
		rapidjson::GenericValue<rapidjson::UTF8<>> val;
		val.SetObject();
		baseNode->Serialize(val, allocator);

		// Eventually rename arguments
		if (val.HasMember("ArgName") && val["ArgName"].IsString()) {
			val["ArgName"].SetString(std::format("{}(copy)", val["ArgName"].GetString()), allocator);
		}

		copiedNodes.PushBack(val, allocator);
	}

	for (auto& link : mINF.getLinks()) {
		auto lnk = link.lock();
		if (!lnk) {
			continue;
		}

		const ImFlow::NodeUID leftNodeId = lnk->left()->getParent()->getUID();
		const ImFlow::NodeUID rightNodeId = lnk->right()->getParent()->getUID();
		if (!selectedNodeIds.contains(leftNodeId) || !selectedNodeIds.contains(rightNodeId)) {
			continue;
		}

		rapidjson::GenericValue<rapidjson::UTF8<>> val;
		val.SetObject();
		val.AddMember("LeftNode", static_cast<uint64_t>(leftNodeId), allocator);
		val.AddMember("LeftPin", lnk->left()->getName(), allocator);
		val.AddMember("RightNode", static_cast<uint64_t>(rightNodeId), allocator);
		val.AddMember("RightPin", lnk->right()->getName(), allocator);
		copiedLinks.PushBack(val, allocator);
	}

	m_clipboard.AddMember("Nodes", copiedNodes, allocator);
	m_clipboard.AddMember("Links", copiedLinks, allocator);
}

void NodeEditor::PasteNodes() {
	if (!m_clipboard.IsObject() ||
		!m_clipboard.HasMember("Nodes") || !m_clipboard["Nodes"].IsArray() ||
		!m_clipboard.HasMember("Links") || !m_clipboard["Links"].IsArray() ||
		m_clipboard["Nodes"].GetArray().Empty()) {
		return;
	}

	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<uint64_t> dis;
	std::set<ImFlow::NodeUID> newNodeIds;
	std::map<ImFlow::NodeUID, ImFlow::NodeUID> remappedNodeIds;

	auto generateNodeId = [&]() {
		ImFlow::NodeUID uid = static_cast<ImFlow::NodeUID>(dis(gen));
		while (mINF.getNodes().contains(uid) || newNodeIds.contains(uid)) {
			uid = static_cast<ImFlow::NodeUID>(dis(gen));
		}
		return uid;
	};

	for (auto itr = m_clipboard["Nodes"].Begin(); itr != m_clipboard["Nodes"].End(); itr++) {
		if (!itr->IsObject()) {
			continue;
		}
		rapidjson::GenericObject node = itr->GetObject();
		if (!(node.HasMember("Name") && node["Name"].IsString()))continue;
		if (!(node.HasMember("Category") && node["Category"].IsString()))continue;
		if (!(node.HasMember("PosX") && node["PosX"].IsNumber()))continue;
		if (!(node.HasMember("PosY") && node["PosY"].IsNumber()))continue;

		uint64_t sourceId = 0;
		if (!TryGetUint64(*itr, "Id", sourceId))continue;

		std::string name = node["Name"].GetString();
		std::string category = node["Category"].GetString();
		if (!nodeTypes.contains(category))continue;
		if (!nodeTypes[category].contains(name))continue;

		const ImFlow::NodeUID newId = generateNodeId();
		rapidjson::Document pasteDoc;
		pasteDoc.SetObject();
		rapidjson::Value nodeCopy;
		nodeCopy.CopyFrom(*itr, pasteDoc.GetAllocator());
		nodeCopy["Id"].SetUint64(static_cast<uint64_t>(newId));
		nodeCopy["PosX"].SetFloat(node["PosX"].GetFloat() + 10.0f);
		nodeCopy["PosY"].SetFloat(node["PosY"].GetFloat() - 10.0f);

		auto newnode = nodeTypes[category][name].RecreateNode(mINF, render, mINF.getStyleManager(), nodeCopy.GetObject());
		if (!newnode)continue;

		ImVec2 pos;
		pos.x = nodeCopy["PosX"].GetFloat();
		pos.y = nodeCopy["PosY"].GetFloat();
		newnode->setPos(pos);

		remappedNodeIds[static_cast<ImFlow::NodeUID>(sourceId)] = newId;
		newNodeIds.insert(newId);
		node["PosX"].SetFloat(pos.x);
		node["PosY"].SetFloat(pos.y);
	}

	for (auto itr = m_clipboard["Links"].Begin(); itr != m_clipboard["Links"].End(); itr++) {
		if (!itr->IsObject()) {
			continue;
		}
		rapidjson::GenericObject link = itr->GetObject();
		if (!(link.HasMember("LeftPin") && link["LeftPin"].IsString()))continue;
		if (!(link.HasMember("RightPin") && link["RightPin"].IsString()))continue;

		uint64_t leftSourceId = 0;
		uint64_t rightSourceId = 0;
		if (!TryGetUint64(*itr, "LeftNode", leftSourceId))continue;
		if (!TryGetUint64(*itr, "RightNode", rightSourceId))continue;
		if (!remappedNodeIds.contains(static_cast<ImFlow::NodeUID>(leftSourceId)))continue;
		if (!remappedNodeIds.contains(static_cast<ImFlow::NodeUID>(rightSourceId)))continue;

		const ImFlow::NodeUID leftNodeId = remappedNodeIds[static_cast<ImFlow::NodeUID>(leftSourceId)];
		const ImFlow::NodeUID rightNodeId = remappedNodeIds[static_cast<ImFlow::NodeUID>(rightSourceId)];
		if (!mINF.getNodes().contains(leftNodeId))continue;
		if (!mINF.getNodes().contains(rightNodeId))continue;

		std::string leftPinName = link["LeftPin"].GetString();
		std::string rightPinName = link["RightPin"].GetString();
		auto left = mINF.getNodes()[leftNodeId];
		auto right = mINF.getNodes()[rightNodeId];
		ImFlow::Pin* leftPin = FindPinByName(left->getOuts(), leftPinName);
		ImFlow::Pin* rightPin = FindPinByName(right->getIns(), rightPinName);
		if (!leftPin || !rightPin)continue;

		leftPin->createLink(rightPin);
	}

	// Select only pasted nodes
	for (auto& [nodeId, nodePtr] : mINF.getNodes()) {
		nodePtr->selected(newNodeIds.contains(nodePtr->getUID()));
	}
}




void NodeEditor::Serialize() {

	if(!mINF.getNodesCount())return;

	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::GenericValue<rapidjson::UTF8<>> nodeArray;
	nodeArray.SetArray();
	for (auto& [nodeId, nodePtr] : mINF.getNodes()) {

		rapidjson::GenericValue<rapidjson::UTF8<>> val;
		val.SetObject();
		std::dynamic_pointer_cast<RuiBaseNode>(nodePtr)->Serialize(val, doc.GetAllocator());
		nodeArray.PushBack(val, doc.GetAllocator());
	}
	doc.AddMember("Nodes", nodeArray, doc.GetAllocator());
	rapidjson::GenericValue<rapidjson::UTF8<>> linkArray;
	linkArray.SetArray();
	for (auto& link : mINF.getLinks()) {
		rapidjson::GenericValue<rapidjson::UTF8<>> val;
		val.SetObject();
		auto lnk = link.lock();
		val.AddMember("LeftNode", lnk->left()->getParent()->getUID(), doc.GetAllocator());
		val.AddMember("LeftPin", lnk->left()->getName(), doc.GetAllocator());
		val.AddMember("RightNode", lnk->right()->getParent()->getUID(), doc.GetAllocator());
		val.AddMember("RightPin", lnk->right()->getName(), doc.GetAllocator());
		linkArray.PushBack(val, doc.GetAllocator());
	}
	doc.AddMember("Links", linkArray, doc.GetAllocator());

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	struct SaveDialogData {
		std::string json;
	};

	auto* data = new SaveDialogData{
	   std::string(buffer.GetString(), buffer.GetSize())
	};


	SDL_ShowSaveFileDialog([](void* userdata, const char* const* filelist, int filter) {

		std::unique_ptr<SaveDialogData> data(
			static_cast<SaveDialogData*>(userdata)
		);

		if (!filelist) {
			SDL_Log("Save dialog error: %s", SDL_GetError());
			return;
		}

		if (!filelist[0]) {
			SDL_Log("Save cancelled");
			return;
		}

		const char* path = filelist[0];

		std::ofstream outFile{ path };
		outFile.write(data->json.c_str(), data->json.size());
		outFile.close();
		return;
		}, (void*)data, (SDL_Window*)g_renderFramework->GetWindow(), filters, 1, nullptr);



}

void NodeEditor::Deserialize() {

	static const SDL_DialogFileFilter filters[] = {
	  { "Graph", "json" }
	};

	SDL_ShowOpenFileDialog(
		[](void* userdata, const char* const* filelist, int filter) {
			NodeEditor* editor = static_cast<NodeEditor*>(userdata);

			if (!filelist) {
				SDL_Log("Open dialog error: %s", SDL_GetError());
				return;
			}

			if (!filelist[0]) {
				SDL_Log("Open cancelled");
				return;
			}

			editor->currentFilePath = std::filesystem::path(filelist[0]);
		},
		this,
		static_cast<SDL_Window*>(g_renderFramework->GetWindow()),
		filters,
		1,
		nullptr,
		false
	);

}

bool NodeEditor::DeserializeFromPath(const fs::path& path)
{
	std::ifstream file(path);
	if (!file.is_open()) {
		printf("Error Opening JSON File %s\n", path.string().c_str());
		return false;
	}
	rapidjson::IStreamWrapper wrap(file);
	rapidjson::Document doc;
	doc.ParseStream(wrap);
	if (doc.HasParseError()) {
		printf("Error in JSON File %s\n", path.string().c_str());
		printf("Parse Error: %d\n", doc.GetParseError());
		return false;
	}
	if (!doc.IsObject()) {
		printf("JSON root is not object %s\n", path.string().c_str());
		return false;
	}
	if (!doc.IsObject()) {
		printf("JSON root is not object %s\n", path.string().c_str());
		return false;
	}
	rapidjson::GenericObject root = doc.GetObject();
	if (!(root.HasMember("Nodes") && root["Nodes"].IsArray())) 
		return false;
	if (!(root.HasMember("Links") && root["Links"].IsArray())) 
		return false;
	rapidjson::GenericArray nodes = root["Nodes"].GetArray();
	for (auto itr = nodes.Begin(); itr != nodes.End(); itr++) {
		if (!itr->IsObject()) {
			printf("Node is not object");
			continue;
		}
		rapidjson::GenericObject node = itr->GetObject();

		if (!(node.HasMember("Name") && node["Name"].IsString()))continue;
		if (!(node.HasMember("Category") && node["Category"].IsString()))continue;

		std::string name = node["Name"].GetString();
		std::string category = node["Category"].GetString();
		if (!nodeTypes.contains(category)) {
			printf("Unknown Category %s", category.c_str());
			continue;
		}
		if (!nodeTypes[category].contains(name)) {
			printf("Unknown Node %s", name.c_str());
			continue;
		}
		nodeTypes[category][name].RecreateNode(mINF, render, mINF.getStyleManager(), node);

	}
	rapidjson::GenericArray links = root["Links"].GetArray();
	for (auto itr = links.Begin(); itr != links.End(); itr++) {
		if (!itr->IsObject())continue;
		rapidjson::GenericObject link = itr->GetObject();
		if (!(link.HasMember("LeftPin") && link["LeftPin"].IsString()))continue;
		if (!(link.HasMember("RightPin") && link["RightPin"].IsString()))continue;
		uint64_t leftId = 0;
		uint64_t rightId = 0;
		if (!TryGetUint64(*itr, "LeftNode", leftId))continue;
		if (!TryGetUint64(*itr, "RightNode", rightId))continue;
		std::string leftPinName = link["LeftPin"].GetString();
		std::string rightPinName = link["RightPin"].GetString();

		if (!mINF.getNodes().contains(leftId))continue;
		if (!mINF.getNodes().contains(rightId))continue;
		auto left = mINF.getNodes()[leftId];
		auto right = mINF.getNodes()[rightId];
		auto hasOutPin = [](const std::shared_ptr<ImFlow::BaseNode>& node, const std::string& name) {
			for (auto& pin : node->getOuts()) {
				if (pin->getName() == name)
					return true;
			}
			return false;
		};
		if (leftPinName == "Vector2" && hasOutPin(left, "Vector2 Res"))
			leftPinName = "Vector2 Res";
		if (leftPinName == "Vector3" && hasOutPin(left, "Vector3 Res"))
			leftPinName = "Vector3 Res";
		ImFlow::Pin* leftPin = FindPinByName(left->getOuts(), leftPinName);
		ImFlow::Pin* rightPin = FindPinByName(right->getIns(), rightPinName);
		if (!leftPin || !rightPin)continue;
		leftPin->createLink(rightPin);

	}
	return true;
}

void NodeEditor::Export() {

	static const SDL_DialogFileFilter filters[] = {
	  { "RuiPackage", "ruip" }
	};

	SDL_ShowSaveFileDialog(
		[](void* userdata, const char* const* filelist, int filter) {
			NodeEditor* editor = static_cast<NodeEditor*>(userdata);

			if (!filelist) {
				SDL_Log("Save dialog error: %s", SDL_GetError());
				return;
			}

			if (!filelist[0]) {
				SDL_Log("Save cancelled");
				return;
			}

			editor->currentFilePath = std::filesystem::path(filelist[0]);
		},
		this,
		static_cast<SDL_Window*>(g_renderFramework->GetWindow()),
		filters,
		1,
		nullptr
	);

	
}

void NodeEditor::ExportToPath(const fs::path& path)
{
	std::string name = path.filename().replace_extension("").string();
	RuiExportPrototype proto(render, name);
	proto.Generate(mINF.getNodes(), render);
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
#ifdef _DEBUG
			if (ImGui::MenuItem("Spawn All Nodes Form Category"))
			{
				for (auto& [name,node]:category)
				{
					node.AddNode(mINF,render,mINF.getStyleManager());
				}
			}
#endif
			ImGui::EndMenu();
		}

	}
#ifdef _DEBUG
	if (ImGui::BeginMenu("Debug")) {
		if (ImGui::MenuItem("Spawn All Node")) {
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
		typeid(MathVariable).name(),
		std::make_shared<ImFlow::PinStyle>(IM_COL32(202,247,113,255),0,4.f,4.67f,3.7f,1.f));
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
	styles.AddNodeStlye(
		"Functions",
		std::make_shared<ImFlow::NodeStyle>(IM_COL32(91, 91, 214, 255), ImColor(233, 241, 244, 255), 6.5f));
	
	auto errorNodeStyle = std::make_shared<ImFlow::NodeStyle>(IM_COL32(173,25,17,255),ImColor(233,241,244,255),6.5f);
	errorNodeStyle->bg = IM_COL32(132,23,17,255);
	styles.SetNodeErrorStyle(errorNodeStyle);
}
