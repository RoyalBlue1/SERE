#include "CustomImGuiWidgets.h"
#include "Imgui/imgui_stdlib.h"

#include "Util.h"

#include "Imgui/implot.h"
#undef min;
#undef max;


bool AtlasImageButton(const char* id, uint32_t hash,ImVec2 maxSize) {
	return AtlasImageButton(id,imageAssetMap[hash],maxSize);
}

bool AtlasImageButton(const char* id, const Asset_t& asset,ImVec2 maxSize) {
	const ImageAtlas& atlas = imageAtlases[asset.atlasIndex];
	const auto& dim = atlas.dimentions[asset.imageIndex];
	const auto& shd = atlas.shaderData[asset.imageIndex];
	const ImVec2 mins(shd.minX, shd.minY);
	const ImVec2 maxs(shd.minX + shd.sizeX, shd.minY + shd.sizeY);
	ImVec2 displaySize(maxSize.x, maxSize.x / dim.width * dim.height);
	if (displaySize.y > maxSize.y)displaySize.y = maxSize.y;
	return ImGui::ImageButton(asset.name.c_str(), (ImTextureRef)atlas.GetImageView(),
		displaySize, mins, maxs);
		
}


void AssetSelectionPopup(const char* id, uint32_t* hash) {
	static std::string search = "";
	if (ImGui::BeginPopup("Asset Selection", ImGuiWindowFlags_MenuBar)) {

		if (ImGui::BeginMenuBar()) {
			ImGui::Text("Search:");
			ImGui::SameLine();
			ImGui::InputText("", &search);
			ImGui::EndMenuBar();
		}


		ImGui::BeginTable("Assets", 6);
		for (const auto& [assetHash, asset] : imageAssetMap) {
			if (search.size() && !caseInsensitiveSearch(asset.name, search))
				continue;

			ImGui::TableNextColumn();
			if(AtlasImageButton(asset.name.c_str(),asset)) {
				*hash = assetHash;
				ImGui::CloseCurrentPopup();
			}
			ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + 100.f);
			ImGui::Text("%s", asset.name.c_str());
			ImGui::PopTextWrapPos();
		}
		ImGui::EndTable();
		ImGui::EndPopup();
	}
}

bool Slider2D(const char* id,float* xVal,float* yVal) {
	bool clicked = false,hovered = false,held = false;

	if (ImPlot::BeginPlot(id, ImVec2(150, 150), ImPlotFlags_CanvasOnly)) {
		const uint32_t axisFlags = ImPlotAxisFlags_NoTickMarks;
		ImPlot::SetupAxes(nullptr, nullptr, axisFlags, axisFlags);
		ImPlot::SetupAxesLimits(-1,1,-1,1);
		double x = *xVal;
		double y = *yVal;
		ImPlot::DragPoint(0, &x, &y, ImVec4(0, 0.9f, 0, 1), 7, 0, &clicked, &hovered, &held);
		*xVal = static_cast<float>(x);
		*yVal = static_cast<float>(y);
		ImPlot::EndPlot();
		if (ImGui::IsItemHovered()) {
			return true;
		}
	}
	return false;
}







void MappingCreationPopup(const char* id, float currentX, Mapping& map) {
	if(ImGui::BeginPopup(id)) {
		map.ShowEditUi(currentX);
		ImGui::EndPopup();
	}
}