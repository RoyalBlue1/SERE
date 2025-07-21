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


void NodeEditorPopup(ImFlow::ImNodeFlow& mINF,RenderInstance& proto,NodeStyles& styles,ImFlow::BaseNode* node) {
	
		if (node == nullptr) {
			
			if (ImGui::BeginMenu("Argument")) {
				if (ImGui::MenuItem("Argument Int")) {
					mINF.placeNode<IntArgNode>(proto,styles);
				}
				if (ImGui::MenuItem("Argument Bool")) {
					mINF.placeNode<BoolArgNode>(proto,styles);
				}
				if (ImGui::MenuItem("Argument Float")) {
					mINF.placeNode<FloatArgNode>(proto,styles);
				}
				if (ImGui::MenuItem("Argument Vector2")) {
					mINF.placeNode<Float2ArgNode>(proto,styles);
				}
				if (ImGui::MenuItem("Argument Vector3")) {
					mINF.placeNode<Float3ArgNode>(proto,styles);
				}
				if (ImGui::MenuItem("Argument Color")) {
					mINF.placeNode<ColorArgNode>(proto,styles);
				}
				if (ImGui::MenuItem("Argument String")) {
					mINF.placeNode<StringArgNode>(proto,styles);
				}
				if (ImGui::MenuItem("Argument Asset")) {
					mINF.placeNode<AssetArgNode>(proto,styles);
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Constant Variable")) {
				if (ImGui::MenuItem("Constant Int")) {
					mINF.placeNode<IntVarNode>(proto,styles);
				}
				if (ImGui::MenuItem("Constant Bool")) {
					mINF.placeNode<BoolVarNode>(proto,styles);
				}
				if (ImGui::MenuItem("Constant Float")) {
					mINF.placeNode<FloatVarNode>(proto,styles);
				}
				if (ImGui::MenuItem("Constant Vector2")) {
					mINF.placeNode<Float2VarNode>(proto,styles);
				}
				if (ImGui::MenuItem("Constant Vector3")) {
					mINF.placeNode<Float3VarNode>(proto,styles);
				}
				if (ImGui::MenuItem("Constant Color")) {
					mINF.placeNode<ColorVarNode>(proto,styles);
				}
				if (ImGui::MenuItem("Constant String")) {
					mINF.placeNode<StringVarNode>(proto,styles);
				}
				if (ImGui::MenuItem("Constant Asset")) {
					mINF.placeNode<AssetVarNode>(proto,styles);
				}
				if (ImGui::MenuItem("Constant Size")) {
					mINF.placeNode<SizeVarNode>(proto,styles);
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Image Nodes")) {
				if (ImGui::MenuItem("Image Render")) {
					mINF.placeNode<AssetRenderNode>(proto,styles);
				}
				if (ImGui::MenuItem("Other Image Render")) {

				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Text Nodes")) {
				if (ImGui::MenuItem("Text Style")) {
					mINF.placeNode<TextStyleNode>(proto,styles);
				}
				if (ImGui::MenuItem("Text Size")) {
					mINF.placeNode<TextSizeNode>(proto,styles);
				}
				if (ImGui::MenuItem("Text Render")) {
					mINF.placeNode<TextRenderNode>(proto,styles);
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Transforms")) {
				if (ImGui::MenuItem("Transform_0")) {
					mINF.placeNode<Transform0Node>(proto,styles);
				}
				if (ImGui::MenuItem("Transform_1")) {
					mINF.placeNode<Transform1Node>(proto,styles);
				}
				if (ImGui::MenuItem("Transform_2")) {
					mINF.placeNode<Transform2Node>(proto,styles);
				}
				if (ImGui::MenuItem("Transform_3")) {
					mINF.placeNode<Transform3Node>(proto,styles);
				}
				if (ImGui::MenuItem("Transform_4")) {
					mINF.placeNode<Transform4Node>(proto,styles);
				}
				if (ImGui::MenuItem("Transform_5")) {
					mINF.placeNode<Transform5Node>(proto,styles);
				}
				if (ImGui::MenuItem("Transform_6")) {
					mINF.placeNode<Transform6Node>(proto,styles);
				}
				if (ImGui::MenuItem("Two Pin Scale")) {
					mINF.placeNode<Transform7Node>(proto,styles);
				}
				if (ImGui::MenuItem("Two Pin Pinch")) {
					mINF.placeNode<Transform8Node>(proto,styles);
				}
				if (ImGui::MenuItem("Two Pin Stretch")) {
					mINF.placeNode<Transform9Node>(proto,styles);
				}
				if (ImGui::MenuItem("Three Pin")) {
					mINF.placeNode<Transform10Node>(proto,styles);
				}
				//if (ImGui::MenuItem("Transform_11")) {

				//}
				//if (ImGui::MenuItem("Transform_12")) {

				//}
				//if (ImGui::MenuItem("Transform_13")) {

				//}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Split/Merge")) {
				if (ImGui::MenuItem("Split Vector2")) {
					mINF.placeNode<SplitFloat2Node>(proto,styles);
				}
				if (ImGui::MenuItem("Merge Vector2")) {
					mINF.placeNode<MergeFloat2Node>(proto,styles);
				}
				if (ImGui::MenuItem("Split Vector3")) {
					mINF.placeNode<SplitFloat3Node>(proto,styles);
				}
				if (ImGui::MenuItem("Merge Vector3")) {
					mINF.placeNode<MergeFloat3Node>(proto,styles);
				}
				if (ImGui::MenuItem("Split Color")) {
					mINF.placeNode<SplitColorNode>(proto,styles);
				}
				if (ImGui::MenuItem("RGB to Color")) {
					mINF.placeNode<RGBToColorNode>(proto,styles);
				}
				if (ImGui::MenuItem("HSV to Color")) {
					mINF.placeNode<HSVToColorNode>(proto,styles);
				}
				if (ImGui::MenuItem("Split Size")) {

				}
				if (ImGui::MenuItem("Merge Size")) {

				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Math")) {
				if (ImGui::MenuItem("Add")) {
					mINF.placeNode<AdditionNode>(proto,styles);
				}
				if (ImGui::MenuItem("Subtract")) {
					mINF.placeNode<SubtractNode>(proto,styles);
				}
				if (ImGui::MenuItem("Multiply")) {
					mINF.placeNode<MultiplyNode>(proto,styles);
				}
				if (ImGui::MenuItem("Divide")) {
					mINF.placeNode<DivideNode>(proto,styles);
				}
				if (ImGui::MenuItem("Modulo")) {
					mINF.placeNode<ModuloNode>(proto,styles);
				}
				if (ImGui::MenuItem("Absolute")) {
					mINF.placeNode<AbsoluteNode>(proto,styles);
				}
				if (ImGui::MenuItem("Sine")) {
					mINF.placeNode<SineNode>(proto,styles);
				}
				if (ImGui::MenuItem("Exponent")) {
					mINF.placeNode<ExponentNode>(proto,styles);
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Globals")) {
				if (ImGui::MenuItem("CurrentTime")) {
					mINF.placeNode<TimeNode>(proto,styles);
				}
				ImGui::EndMenu();
			}
			
		}
		else {
			if (ImGui::MenuItem("Delete")) {
				node->destroy();
			}
		}
	
}