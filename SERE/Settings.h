#pragma once
#include <string>
#include "Imgui/imgui.h"
#include "Imgui/imgui_stdlib.h"
#include "ThirdParty/nativefiledialog-extended/src/include/nfd.hpp"
#include "ThirdParty/rapidjson/rapidjson.h"
#include "ThirdParty/rapidjson/prettywriter.h"
#include "ThirdParty/rapidjson/ostreamwrapper.h"
#include "ThirdParty/rapidjson/istreamwrapper.h"

class Settings {
	struct RuiSize_t {
		int width;
		int height;
	};
private:

	bool visible;
	bool changed;


	std::string titanfall2Path;

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
			NFD::UniquePath path;
			if (NFD::PickFolder(path, (nfdchar_t*)titanfall2Path.c_str()) == NFD_OKAY) {
				titanfall2Path = std::string(path.get());
			}
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

		writer.EndObject();

		out.close();
	}


};