#include "Imgui/imgui.h"
#include "ImageAtlas.h"
#include <string>


void AssetSelectionPopup(const char* id, uint32_t* hash);
bool AtlasImageButton(const char* id, uint32_t hash,ImVec2 maxSize = {100.f,200.f});
bool AtlasImageButton(const char* id, const Asset_t& asset,ImVec2 maxSize = {100.f,200.f});
bool Slider2D(const char* id,float* xVal,float* yVal);