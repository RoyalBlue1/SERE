#include "CustomImGuiWidgets.h"
#include "Imgui/imgui_stdlib.h"

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
	return ImGui::ImageButton(asset.name.c_str(), atlas.imageResourceView,
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
			if (asset.name.find(search) == std::string::npos)
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







void MappingCreationPopup(const char* id, Mapping& map) {
	if(ImGui::BeginPopup(id)) {
		ImGui::Checkbox("isCubic",&map.cubicSpline);
		ImGui::SameLine();
		if(ImGui::Button("Add Value")) {
			map.AddValue();
		}
		ImGui::SameLine();
		if(ImGui::Button("Remove Value")) {
			map.RemoveValue();
		}
		if(ImPlot::BeginPlot("test",ImVec2(600,400))) {
			ImPlotRect range =  ImPlot::GetPlotLimits();
			
			for (size_t i = 0;i<map.values.size();i++) {
				double x = map.values[i].x;
				double y = map.values[i].y;
				ImPlot::DragPoint(map.values[i].id,&x, &y, ImVec4(0.2f, 0.8f, 0.f, 1.f));
				map.values[i].x = static_cast<float>(x);
				map.values[i].y = static_cast<float>(y);
				if (map.cubicSpline) {
					x = map.values[i].x + range.Size().x * 0.02;
					y = map.values[i].y + map.values[i].dir*.2f;
					ImPlot::DragPoint(map.values[i].id+1, &x, &y, ImVec4(0.7f, 0.2f, 0.f, 1.f), 4.f,ImPlotDragToolFlags_Delayed);
					float dir = y - map.values[i].y;
					map.values[i].dir = dir*5.f;
				}
				

			}
			map.sort();
			
			
			std::vector<ImVec2> points;
			for (float x = range.Min().x; x < range.Max().x; x += (range.Size().x / 200.f)) {
				points.push_back({x,map.MapVar(x)});
			}

			ImPlot::PlotLine("line",&points[0].x,&points[0].y,points.size(),0,0,sizeof(ImVec2));

			ImPlot::EndPlot();
		}


		ImGui::EndPopup();
	}
}