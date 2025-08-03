# CMakeList.txt : CMake project for SERE, include source and define
# project specific logic here.
#

# Add source to this project's executable.

add_subdirectory(Thirdparty/nativefiledialog-extended)

set(CMAKE_INCLUDE_CURRENT_DIR,ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})



add_executable (SERE
	"SERE.cpp"
	"SERE.rc"
	"ImageAtlas.cpp"
	"FontAtlas.cpp"
	"CustomImGuiWidgets.cpp"

	"RuiNodeEditor/RuiNodeEditor.cpp"
	"RuiNodeEditor/RuiNodeStyle.cpp"
	"RuiNodeEditor/Mapping.cpp"

	"RuiRendering/RenderFunctions.cpp"
	"RuiRendering/RenderManager.cpp"

	"imgui/imgui.cpp"
	"imgui/imgui_demo.cpp"
	"imgui/imgui_draw.cpp"
	"imgui/imgui_tables.cpp"
	"imgui/imgui_widgets.cpp"
	"imgui/imgui_impl_dx11.cpp"
	"imgui/imgui_impl_win32.cpp"
	"imgui/ImNodeFlow.cpp"
	"imgui/implot.cpp"
	"imgui/implot_demo.cpp"
	"imgui/implot_items.cpp"
	"imgui/imgui_stdlib.cpp"

	"Nodes/ArgumentNodes.cpp"
	"Nodes/TransformNodes.cpp"
	"Nodes/RenderJobNodes.cpp"
	"Nodes/ConstantVarNodes.cpp"
	"Nodes/SplitMergeNodes.cpp"
	"Nodes/MathNodes.cpp"
	"Nodes/GlobalNodes.cpp"
	
	"ThirdParty/DDSTextureLoader11.cpp"
	)

target_include_directories(SERE PUBLIC CMAKE_CURRENT_SOURCE_DIR )
target_link_libraries(SERE PUBLIC nfd)

add_custom_command(TARGET SERE POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Assets ${CMAKE_CURRENT_BINARY_DIR}/Assets)
add_custom_command(TARGET SERE POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Imgui/DroidSans.ttf ${CMAKE_CURRENT_BINARY_DIR}/Imgui/DroidSans.ttf)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SERE PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
