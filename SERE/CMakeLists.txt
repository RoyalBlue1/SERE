# CMakeList.txt : CMake project for SERE, include source and define
# project specific logic here.
#

# Add source to this project's executable.

set(CMAKE_INCLUDE_CURRENT_DIR,ON)


add_executable (SERE
	"SERE.cpp"
	"imgui.cpp"
	"imgui_demo.cpp"
	"imgui_draw.cpp"
	"imgui_tables.cpp"
	"imgui_widgets.cpp"
	"imgui_impl_dx11.cpp"
	"imgui_impl_win32.cpp"
	"SERE.rc")

target_include_directories(SERE PUBLIC CMAKE_CURRENT_SOURCE_DIR)




if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SERE PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
