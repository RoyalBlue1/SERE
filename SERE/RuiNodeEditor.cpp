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

#include "Thirdparty/rapidjson/istreamwrapper.h"
#include "ThirdParty/rapidjson/prettywriter.h"
#include "ThirdParty/rapidjson/stringbuffer.h"
#undef GetObject

void NodeEditor::draw() {
	ImGui::Begin("Node Editor");
	
	mINF.update();

	ImGui::End();
}

void NodeEditor::Serialize(const fs::path& path) {
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

void NodeEditor::Deserialize(const fs::path& path) {
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
		nodeTypes[category][name].RecreateNode(mINF,proto,styles,node);
		
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
					nodeType.AddNode(mINF,proto,styles);
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
					node.AddNode(mINF,proto,styles);
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
					std::shared_ptr<ImFlow::BaseNode> node = nodeType.AddNode(mINF, proto, styles);
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

NodeStyles::NodeStyles() {
	pinStyles.emplace(
		typeid(TransformResult).name(),
		std::make_shared<ImFlow::PinStyle>(IM_COL32(191,90,90,255),1,4.f,4.67f,3.7f,1.f));
	pinStyles.emplace(
		typeid(TransformSize).name(),
		std::make_shared<ImFlow::PinStyle>(IM_COL32(90,191,93,255),1,4.f,4.67f,3.7f,1.f));

	pinStyles.emplace(
		typeid(IntVariable).name(),
		std::make_shared<ImFlow::PinStyle>(IM_COL32(87,155,185,255),0,4.f,4.67f,3.7f,1.f));
	pinStyles.emplace(
		typeid(BoolVariable).name(),
		std::make_shared<ImFlow::PinStyle>(IM_COL32(90,117,191,255),0,4.f,4.67f,3.7f,1.f));
	pinStyles.emplace(
		typeid(FloatVariable).name(),
		std::make_shared<ImFlow::PinStyle>(IM_COL32(247,229,113,255),0,4.f,4.67f,3.7f,1.f));
	pinStyles.emplace(
		typeid(Float2Variable).name(),
		std::make_shared<ImFlow::PinStyle>(IM_COL32(142,247,113,255),0,4.f,4.67f,3.7f,1.f));
	pinStyles.emplace(
		typeid(Float3Variable).name(),	
		std::make_shared<ImFlow::PinStyle>(IM_COL32(113,247,200,255),0,4.f,4.67f,3.7f,1.f));
	pinStyles.emplace(
		typeid(ColorVariable).name(),
		std::make_shared<ImFlow::PinStyle>(IM_COL32(113,178,247,255),0,4.f,4.67f,3.7f,1.f));
	pinStyles.emplace(
		typeid(StringVariable).name(),	
		std::make_shared<ImFlow::PinStyle>(IM_COL32(133,113,247,255),0,4.f,4.67f,3.7f,1.f));
	pinStyles.emplace(
		typeid(AssetVariable).name(),
		std::make_shared<ImFlow::PinStyle>(IM_COL32(218,113,247,255),0,4.f,4.67f,3.7f,1.f));




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

std::shared_ptr<ImFlow::NodeStyle> NodeStyles::GetNodeStyle(std::string name) {
	if(nodeStyles.contains(name))
		return nodeStyles[name];
	return defaultNode;
}
