# CMakeList.txt : CMake project for SERE, include source and define
# project specific logic here.
#

# Add source to this project's executable.

add_subdirectory(ThirdParty/nativefiledialog-extended)

set(CMAKE_INCLUDE_CURRENT_DIR,ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})



add_executable (SERE
	"SERE.cpp"
	"SERE.rc"
	"ImageAtlas.cpp"
	"FontAtlas.cpp"
	"CustomImGuiWidgets.cpp"

	"RuiNodeEditor/RuiNodeEditor.cpp"
	"RuiNodeEditor/Mapping.cpp"

	"RuiRendering/RenderFunctions.cpp"
	"RuiRendering/RenderManager.cpp"

	"Imgui/imgui.cpp"
	"Imgui/imgui_demo.cpp"
	"Imgui/imgui_draw.cpp"
	"Imgui/imgui_tables.cpp"
	"Imgui/imgui_widgets.cpp"
	"Imgui/imgui_impl_dx11.cpp"
	"Imgui/imgui_impl_win32.cpp"
	"Imgui/ImNodeFlow.cpp"
	"Imgui/implot.cpp"
	"Imgui/implot_demo.cpp"
	"Imgui/implot_items.cpp"
	"Imgui/imgui_stdlib.cpp"

	"Nodes/ArgumentNodes.cpp"
	"Nodes/TransformNodes.cpp"
	"Nodes/RenderJobNodes.cpp"
	"Nodes/ConstantVarNodes.cpp"
	"Nodes/SplitMergeNodes.cpp"
	"Nodes/MathNodes.cpp"
	"Nodes/GlobalNodes.cpp"
	
	"ThirdParty/DDSTextureLoader11.cpp"
	"RuiNodeEditor/RuiBaseNode.cpp"
	"RuiNodeEditor/RuiExportPrototype.cpp"
	"Nodes/ConditionalNodes.cpp"

	"RenderFrameworks/RenderFramework_Dx11.cpp"	
	"PakLoading/asset.cpp"
	"PakLoading/cpakfile.cpp"
	"PakLoading/patchapi.cpp"
	"PakLoading/texture.cpp"
	
	
	"PakLoading/utils.cpp"

 "Settings.h" "RenderFrameworks/RenderFramework.cpp")

target_include_directories(SERE PUBLIC CMAKE_CURRENT_SOURCE_DIR )
target_link_libraries(SERE PUBLIC nfd)

add_custom_command(TARGET SERE POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Assets ${CMAKE_CURRENT_BINARY_DIR}/Assets)
add_custom_command(TARGET SERE POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Imgui/DroidSans.ttf ${CMAKE_CURRENT_BINARY_DIR}/Imgui/DroidSans.ttf)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SERE PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
