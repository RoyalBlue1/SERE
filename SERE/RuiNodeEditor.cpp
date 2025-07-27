#include "RuiNodeEditor.h"
#include "imgui/imgui.h"

#include "imgui/imgui_stdlib.h"

#include "Nodes/ArgumentNodes.h"
#include "Nodes/TransformNodes.h"
#include "Nodes/RenderJobNodes.h"
#include "Nodes/ConstantVarNodes.h"
#include "Nodes/SplitMergeNodes.h"
#include "Nodes/MathNodes.h"
#include "Nodes/GlobalNodes.h"

void NodeEditor::draw() {
	ImGui::Begin("Node Editor");
	
	mINF.update();

	ImGui::End();
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
#ifdef DEBUG
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
	if(!pin)return;
	ImFlow::PinType neededPinType = pin->getType()==ImFlow::PinType_Input?
		ImFlow::PinType_Output:ImFlow::PinType_Input;
	std::string searchString;
	ImGui::InputText("Search",&searchString,ImGuiInputTextFlags_AutoSelectAll);
	for (auto& [catName, category] : nodeTypes) {
		bool searchHit = catName.find(searchString) != std::string::npos;
		for (auto& [nodeName, nodeType] : category) {
			searchHit |= nodeName.find(searchString) != std::string::npos;
			for (auto& pinInfo : nodeType.GetPinInfo()) {
				searchHit |= pinInfo->name.find(searchString) != std::string::npos;
				if(pin->getType()==pinInfo->GetPinType())continue;
				if(!pinInfo->CanCreateLink(pin->getProto()))continue;

				if(searchString.size()&&(!searchHit))continue;
				std::string menuName = std::format("{} > {}",nodeName,pinInfo->name);
				if (ImGui::MenuItem(menuName.c_str())) {
					std::shared_ptr<ImFlow::BaseNode> node = nodeType.AddNode(mINF,proto,styles);
					if (pinInfo->GetPinType() == ImFlow::PinType_Input) {
						node->inPin(pinInfo->name.c_str())->createLink(pin);
					}
					else {
						node->outPin(pinInfo->name.c_str())->createLink(pin);
					}
				}
			}
		}
	}


}