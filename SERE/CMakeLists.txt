# CMakeList.txt : CMake project for SERE, include source and define
# project specific logic here.
#

# Add source to this project's executable.

set(CMAKE_INCLUDE_CURRENT_DIR,ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})



add_executable (SERE
	"SERE.cpp"
	"imgui/imgui.cpp"
	"imgui/imgui_demo.cpp"
	"imgui/imgui_draw.cpp"
	"imgui/imgui_tables.cpp"
	"imgui/imgui_widgets.cpp"
	"imgui/imgui_impl_dx11.cpp"
	"imgui/imgui_impl_win32.cpp"
	"SERE.rc"
	"imgui/ImNodeFlow.cpp"
	"RuiNodeEditor.cpp"
	"imgui/imgui_stdlib.cpp"
	"Nodes/ArgumentNodes.cpp"
	"Nodes/TransformNodes.cpp"
	"Nodes/RenderJobNodes.cpp"
	"RenderManager.cpp"
	"ImageAtlas.cpp"
	"ThirdParty/yyjson.cpp"
	"ThirdParty/DDSTextureLoader11.cpp"
	"FontAtlas.cpp"
	"Nodes/ConstantVarNodes.cpp"
	"Nodes/SplitMergeNodes.cpp"
	"Nodes/MathNodes.cpp" "Nodes/GlobalNodes.h" "Nodes/GlobalNodes.cpp" "RenderFunctions.cpp" "RenderFunctions.h" "IntrinUtil.h")

target_include_directories(SERE PUBLIC CMAKE_CURRENT_SOURCE_DIR )#${PROJECT_SOURCE_DIR}/include)
#target_link_libraries(SERE PUBLIC Zep::Zep)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SERE PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
