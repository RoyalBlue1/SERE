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
}
