#include "CustomImGuiWidgets.h"
#include "Imgui/imgui_stdlib.h"




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