#pragma once
#include <string>
#include "Imgui/imgui.h"
#include "Imgui/imgui_stdlib.h"
#include "ThirdParty/rapidjson/rapidjson.h"
#include "ThirdParty/rapidjson/prettywriter.h"
#include "ThirdParty/rapidjson/ostreamwrapper.h"
#include "ThirdParty/rapidjson/istreamwrapper.h"
#include <SDL3/SDL.h>

class Settings {
	struct RuiSize_t {
		int width;
		int height;
	};
private:

	bool visible;
	bool changed;


	std::string titanfall2Path;
	std::string customRpakPath;

	int ruiWidth;
	int ruiHeight;

public:

	Settings():ruiWidth(1920),ruiHeight(1080),titanfall2Path(""),changed(true),visible(false) {
		LoadFromFile();
		if(!titanfall2Path.size())
			visible = true;
	}

	std::string GetTitanfall2Path() {
		return titanfall2Path;
	}
	std::string GetCustomRpakPath() {
		return customRpakPath;
	}
	bool HasChanged() {
		bool ret = changed;
		changed = false;
		return ret;
	}

	RuiSize_t GetRuiSize() {
		return RuiSize_t{ruiWidth,ruiHeight};
	}
	

	void Open() {
		visible = true;
	}

	void ShowSettingsWindow() {
		if(!visible)return;
		ImGui::Begin("Settings",&visible,ImGuiWindowFlags_NoDocking); 

		ImGui::SeparatorText("GamePath");
		ImGui::InputText("Titanfall 2 Path",&titanfall2Path);
		ImGui::SameLine();
		if(ImGui::Button("Browse")) {
			SDL_ShowOpenFolderDialog(
				[](void* userdata, const char* const* filelist, int filter) {
					auto* pathString = static_cast<std::string*>(userdata);

					if (!filelist) {
						SDL_Log("Folder dialog error: %s", SDL_GetError());
						return;
					}

					if (!filelist[0]) {
						SDL_Log("Folder selection cancelled");
						return;
					}

					*pathString = filelist[0];
				},
				&titanfall2Path,
				static_cast<SDL_Window*>(g_renderFramework->GetWindow()),
				titanfall2Path.empty() ? nullptr : titanfall2Path.c_str(),
				false
			);
		}

		ImGui::InputText("Custom Rpak Path", &customRpakPath);
		ImGui::SameLine();
		if (ImGui::Button("Browse##customRpak")) {
			SDL_ShowOpenFolderDialog(
				[](void* userdata, const char* const* filelist, int filter) {
					auto* pathString = static_cast<std::string*>(userdata);

					if (!filelist) {
						SDL_Log("Folder dialog error: %s", SDL_GetError());
						return;
					}

					if (!filelist[0]) {
						SDL_Log("Folder selection cancelled");
						return;
					}

					*pathString = filelist[0];
				},
				&customRpakPath,
				static_cast<SDL_Window*>(g_renderFramework->GetWindow()),
				customRpakPath.empty() ? nullptr : customRpakPath.c_str(),
				false
			);
		}

		ImGui::SeparatorText("Rui Size");

		ImGui::InputInt("Width", &ruiWidth, 1, 50);

		ImGui::InputInt("Height", &ruiHeight, 1, 50);

		
		ImGui::End();
		if (!visible) {
			changed = true;
			SaveToFile();
		}

	}
#undef GetObject
	void LoadFromFile() {
		std::ifstream file{"settings.json"};
		if(!file.good())
			return;
		rapidjson::IStreamWrapper fileWrap(file);
		rapidjson::Document doc;
		doc.ParseStream(fileWrap);
		if (!doc.IsObject()) {
			file.close();
			return;
		}
		rapidjson::GenericObject root = doc.GetObject();

		if (root.HasMember("RuiWidth") && root["RuiWidth"].IsInt()) {
			ruiWidth = root["RuiWidth"].GetInt();
		}
		if (root.HasMember("RuiHeight") && root["RuiHeight"].IsInt()) {
			ruiHeight = root["RuiHeight"].GetInt();
		}
		if (root.HasMember("GamePath") && root["GamePath"].IsString()) {
			titanfall2Path = root["GamePath"].GetString();
		}
		if (root.HasMember("CustomRpakPath") && root["CustomRpakPath"].IsString()) {
			customRpakPath = root["CustomRpakPath"].GetString();
		}
		file.close();
	}

	void SaveToFile() {
		std::ofstream out("settings.json");
		if(!out.good())return;
		rapidjson::OStreamWrapper outWrap(out);
		rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(outWrap);
		writer.StartObject();

		writer.Key("RuiWidth");
		writer.Int(ruiWidth);

		writer.Key("RuiHeight");
		writer.Int(ruiHeight);

		writer.Key("GamePath");
		writer.String(titanfall2Path);

		writer.Key("CustomRpakPath");
		writer.String(customRpakPath);

		writer.EndObject();

		out.close();
	}


};