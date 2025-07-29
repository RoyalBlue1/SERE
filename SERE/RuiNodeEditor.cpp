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

static bool caseInsensitiveSearch(const std::string& strHaystack, const std::string& strNeedle)
{
	auto it = std::search(
		strHaystack.begin(), strHaystack.end(),
		strNeedle.begin(), strNeedle.end(),
		[](unsigned char ch1, unsigned char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
	);
	return (it != strHaystack.end());
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
