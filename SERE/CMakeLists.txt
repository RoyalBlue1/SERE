# CMakeList.txt : CMake project for SERE, include source and define
# project specific logic here.
#

# Add source to this project's executable.
set(SDL_SHARED OFF)
set(SDL_STATIC ON)

if(WIN32 AND NOT DEFINED ENV{CI})
	add_subdirectory(Thirdparty/SDL EXCLUDE_FROM_ALL)
	add_subdirectory(Thirdparty/glew)
elseif(LINUX)
	find_package(SDL3 REQUIRED)
	find_package(GLEW REQUIRED)
	find_package(TBB REQUIRED)
else()
	add_subdirectory(Thirdparty/glew)
	find_package(SDL3 REQUIRED)
endif()
find_package(OpenGL REQUIRED COMPONENTS OpenGL)

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

	"defs.h"
	"Imgui/imgui.cpp"
	"Imgui/imgui_demo.cpp"
	"Imgui/imgui_draw.cpp"
	"Imgui/imgui_tables.cpp"
	"Imgui/imgui_widgets.cpp"
	"Imgui/imgui_impl_sdl3.cpp"
	"Imgui/imgui_impl_opengl3.cpp"
	"Imgui/imgui_impl_opengl3_loader.h"
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
	"Nodes/FunctionNodes.cpp"
	
	"RuiNodeEditor/RuiBaseNode.cpp"
	"RuiNodeEditor/RuiBasePin.cpp"
	"RuiNodeEditor/RuiExportPrototype.cpp"
	"Nodes/ConditionalNodes.cpp"
	"RenderFrameworks/RenderFramework.cpp"
	"RenderFrameworks/RenderFramework_OGL3.cpp"
	"RenderFrameworks/RenderFramework_OGL3.h"
		
	"PakLoading/asset.cpp"
	"PakLoading/cpakfile.cpp"
	"PakLoading/patchapi.cpp"
	"PakLoading/texture.cpp"
	
	
	"PakLoading/utils.cpp"

 	"Settings.h")

target_include_directories(SERE PUBLIC CMAKE_CURRENT_SOURCE_DIR )
target_link_libraries(SERE PRIVATE SDL3::SDL3)
target_link_libraries(SERE PUBLIC ${OPENGL_LIBRARIES})


add_custom_command(TARGET SERE POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Assets ${CMAKE_CURRENT_BINARY_DIR}/Assets)
add_custom_command(TARGET SERE POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Imgui/DroidSans.ttf ${CMAKE_CURRENT_BINARY_DIR}/imgui/DroidSans.ttf)

if(WIN32)
	target_link_libraries(SERE PRIVATE libglew_static)
	target_sources(SERE PRIVATE
	"Imgui/imgui_impl_dx11.cpp"
	"ThirdParty/DDSTextureLoader11.cpp"
	"RenderFrameworks/RenderFramework_Dx11.cpp"
	)
else()
	target_link_libraries(SERE PRIVATE GLEW::GLEW TBB::tbb)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SERE PROPERTY CXX_STANDARD 20)
endif()
target_compile_options(SERE PRIVATE -mssse3)

# TODO: Add tests and install targets if needed.
